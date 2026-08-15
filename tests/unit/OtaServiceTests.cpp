#include <services/OtaService.hpp>
namespace {
class Store final : public services::IFirmwareStore {
public:
    uhal::Status begin(std::uint32_t) override {
        return uhal::Status::ok;
    }
    uhal::Status write(std::uint32_t, const std::uint8_t*, std::size_t s) override {
        written += s;
        return uhal::Status::ok;
    }
    uhal::Status finish() override {
        return uhal::Status::ok;
    }
    void abort() override {
        aborted = true;
    }
    std::size_t written = 0U;
    bool        aborted = false;
};
class Boot final : public services::IBootControl {
public:
    uhal::Status activate_staged() override {
        activated = true;
        return uhal::Status::ok;
    }
    uhal::Status rollback() override {
        return uhal::Status::ok;
    }
    bool activated = false;
};
class Fetcher final : public services::IImageFetcher {
public:
    uhal::Status request(std::uint32_t, std::size_t) override {
        return uhal::Status::ok;
    }
    void         poll(std::uint32_t) override {}
    uhal::Status take(std::uint8_t* out, std::size_t cap, std::size_t& size) override {
        if (done) return uhal::Status::busy;
        size = cap < 4U ? cap : 4U;
        for (std::size_t i = 0U; i < size; ++i) out[i] = 0U;
        done = true;
        return uhal::Status::ok;
    }
    bool done = false;
};
class Digest final : public services::IDigestCalculator {
public:
    uhal::Status begin() override {
        return uhal::Status::ok;
    }
    uhal::Status update(const std::uint8_t*, std::size_t) override {
        return uhal::Status::ok;
    }
    uhal::Status finish(std::uint8_t* out, std::size_t size) override {
        for (std::size_t i = 0U; i < size; ++i) out[i] = 0U;
        return uhal::Status::ok;
    }
};
class Verifier final : public services::ISignatureVerifier {
public:
    uhal::Status verify(const std::uint8_t*, std::size_t, const std::uint8_t*, std::size_t,
                        std::uint8_t) override {
        return result;
    }
    uhal::Status result = uhal::Status::ok;
};
bool happy() {
    Store                   store;
    Boot                    boot;
    Fetcher                 fetch;
    Digest                  digest;
    Verifier                verifier;
    services::OtaService    ota{store, boot, fetch, digest, verifier};
    services::ImageManifest m{};
    m.size_bytes = 4U;
    if (ota.offer(m) != uhal::Status::ok) return false;
    ota.poll(0U);
    ota.poll(1U);
    return ota.state() == services::OtaState::ready_to_activate &&
           ota.activate() == uhal::Status::ok && boot.activated;
}
bool refuses_unsupported() {
    Store    store;
    Boot     boot;
    Fetcher  fetch;
    Digest   digest;
    Verifier verifier;
    verifier.result = uhal::Status::unsupported;
    services::OtaService    ota{store, boot, fetch, digest, verifier};
    services::ImageManifest m{};
    m.size_bytes = 4U;
    ota.offer(m);
    ota.poll(0U);
    ota.poll(1U);
    return ota.state() == services::OtaState::failed && store.aborted && !boot.activated;
}
}  // namespace
int main() {
    return happy() && refuses_unsupported() ? 0 : 1;
}
