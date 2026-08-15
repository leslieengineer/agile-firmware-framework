#include <services/IndicationService.hpp>
#include <services/NetworkManager.hpp>
#include <services/ProvisioningService.hpp>
namespace {
class Link final : public services::INetworkLink {
public:
    uhal::Status start() override {
        ++starts;
        state_ = start_result == uhal::Status::ok ? services::LinkState::connecting
                                                  : services::LinkState::failed;
        return start_result;
    }
    uhal::Status stop() override {
        state_ = services::LinkState::down;
        return uhal::Status::ok;
    }
    void                poll(std::uint32_t) override {}
    services::LinkState state() const override {
        return state_;
    }
    services::LinkQuality quality() const override {
        return {};
    }
    services::LinkKind kind() const override {
        return kind_;
    }
    services::LinkState state_       = services::LinkState::down;
    services::LinkKind  kind_        = services::LinkKind::wifi;
    uhal::Status        start_result = uhal::Status::ok;
    int                 starts       = 0;
};
class Config final : public services::IConfigStore {
public:
    uhal::Status get_blob(const char*, std::uint8_t*, std::size_t, std::size_t&) const override {
        return uhal::Status::not_found;
    }
    uhal::Status set_blob(const char*, const std::uint8_t*, std::size_t) override {
        saved = true;
        return uhal::Status::ok;
    }
    uhal::Status get_u32(const char*, std::uint32_t&) const override {
        return uhal::Status::not_found;
    }
    uhal::Status set_u32(const char*, std::uint32_t) override {
        return uhal::Status::ok;
    }
    uhal::Status get_bool(const char*, bool&) const override {
        return uhal::Status::not_found;
    }
    uhal::Status set_bool(const char*, bool) override {
        return uhal::Status::ok;
    }
    uhal::Status erase(const char*) override {
        return uhal::Status::ok;
    }
    uhal::Status commit() override {
        return uhal::Status::ok;
    }
    bool saved = false;
};
class Portal final : public services::IProvisioningPortal {
public:
    uhal::Status open(const services::PortalConfig&) override {
        opened = true;
        return uhal::Status::ok;
    }
    uhal::Status close() override {
        opened = false;
        return uhal::Status::ok;
    }
    void poll(std::uint32_t) override {}
    bool has_credentials() const override {
        return ready;
    }
    uhal::Status take_credentials(services::NetworkCredentials& out) override {
        out.ssid[0] = 'x';
        ready       = false;
        return uhal::Status::ok;
    }
    bool opened = false;
    bool ready  = false;
};
class Validator final : public services::IProvisioningValidator {
public:
    uhal::Status validate(const services::NetworkCredentials&) override {
        return result;
    }
    uhal::Status result = uhal::Status::ok;
};
class Indicator final : public services::IIndicator {
public:
    uhal::Status apply(const services::IndicationFrame& frame) override {
        last = frame;
        ++calls;
        return uhal::Status::ok;
    }
    services::IndicationFrame last{};
    int                       calls = 0;
};
bool network() {
    Link wifi;
    Link cell;
    cell.kind_ = services::LinkKind::cellular;
    services::NetworkManager manager;
    manager.add_link(wifi);
    manager.add_link(cell);
    manager.poll(0U);
    if (wifi.starts != 1) return false;
    wifi.state_ = services::LinkState::up;
    manager.poll(1U);
    return manager.state() == services::ConnectivityState::online_primary;
}
bool provisioning() {
    Portal                        portal;
    Validator                     validator;
    Config                        config;
    services::ProvisioningService service{portal, validator, config};
    if (service.start(0U) != uhal::Status::ok) return false;
    portal.ready = true;
    service.poll(1U);
    return service.state() == services::ProvisioningState::provisioned && config.saved &&
           !portal.opened;
}
bool indication() {
    Indicator                   indicator;
    services::IndicationService service{indicator};
    services::IndicationFrame   low{{0U, 1U, 0U}, services::IndicationPattern::solid, 0U};
    services::IndicationFrame   fault{{1U, 0U, 0U}, services::IndicationPattern::fast_blink, 100U};
    service.request(services::IndicationSource::load_state, low, 0U);
    service.request(services::IndicationSource::fault, fault, 0U);
    return service.poll(0U) == uhal::Status::ok && indicator.last.color.red == 1U &&
           indicator.calls == 1;
}
}  // namespace
int main() {
    return network() && provisioning() && indication() ? 0 : 1;
}
