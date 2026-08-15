#include <services/BinarySwitchService.hpp>
#include <uhal/fake/FakeGpio.hpp>

namespace {

class VolatileStateStore final : public services::IBinaryStateStore {
public:
    uhal::Status load(services::BinaryState& state) override {
        state = state_;
        return uhal::Status::ok;
    }

    uhal::Status save(const services::BinaryState& state) override {
        state_ = state;
        return uhal::Status::ok;
    }

private:
    services::BinaryState state_{};
};

}  // namespace

int main() {
    uhal::fake::FakeGpio relay;
    uhal::fake::FakeGpio indicator;
    VolatileStateStore store;
    services::BinarySwitchService service{relay, indicator, store};

    return service.restore() == uhal::Status::ok &&
                   service.toggle() == uhal::Status::ok && service.is_on()
               ? 0
               : 1;
}
