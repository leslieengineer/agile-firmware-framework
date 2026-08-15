#include <services/ConfigService.hpp>
#include <services/SecurityPolicy.hpp>
#include <uhal/IStorage.hpp>

namespace {

class FakeStorage final : public uhal::IStorage {
public:
    FakeStorage() {
        for (auto& byte : data_) byte = 0xFFU;
    }
    uhal::Status read(std::uint32_t offset, std::uint8_t* data, std::size_t size) override {
        if (fail_read || data == nullptr || offset + size > sizeof(data_))
            return uhal::Status::io_error;
        for (std::size_t index = 0U; index < size; ++index) data[index] = data_[offset + index];
        return uhal::Status::ok;
    }
    uhal::Status write(std::uint32_t offset, const std::uint8_t* data, std::size_t size) override {
        if (fail_write || data == nullptr || offset + size > sizeof(data_))
            return uhal::Status::io_error;
        for (std::size_t index = 0U; index < size; ++index) data_[offset + index] = data[index];
        return uhal::Status::ok;
    }
    uhal::Status erase(std::uint32_t offset, std::size_t size) override {
        if (fail_erase || offset + size > sizeof(data_)) return uhal::Status::io_error;
        for (std::size_t index = 0U; index < size; ++index) data_[offset + index] = 0xFFU;
        return uhal::Status::ok;
    }
    void corrupt(std::uint32_t offset) {
        data_[offset] ^= 0x5AU;
    }
    bool fail_read  = false;
    bool fail_write = false;
    bool fail_erase = false;

private:
    std::uint8_t data_[services::ConfigService::storage_bytes()]{};
};

class FakeVerifier final : public services::ISignatureVerifier {
public:
    uhal::Status verify(const std::uint8_t*, std::size_t, const std::uint8_t*, std::size_t,
                        std::uint8_t) override {
        return status;
    }
    uhal::Status status = uhal::Status::ok;
};

bool configuration_round_trip_and_recovery() {
    FakeStorage             storage;
    services::ConfigService first{storage};
    if (first.initialize() != uhal::Status::ok ||
        first.set_bool("enabled", true) != uhal::Status::ok || first.commit() != uhal::Status::ok ||
        first.set_bool("enabled", false) != uhal::Status::ok || first.commit() != uhal::Status::ok)
        return false;
    storage.corrupt(0U);
    services::ConfigService recovered{storage};
    bool                    enabled = false;
    return recovered.initialize() == uhal::Status::ok &&
           recovered.get_bool("enabled", enabled) == uhal::Status::ok && enabled;
}

bool configuration_errors_are_explicit() {
    FakeStorage             storage;
    services::ConfigService config{storage};
    if (config.initialize() != uhal::Status::ok) return false;
    bool value = false;
    if (config.get_bool("missing", value) != uhal::Status::not_found) return false;
    storage.fail_write = true;
    return config.set_bool("x", true) == uhal::Status::ok &&
           config.commit() == uhal::Status::io_error;
}

bool security_policy_enforces_roles_and_credentials() {
    FakeVerifier             verifier;
    services::SecurityPolicy policy{verifier};
    return policy.register_command("switch.set", services::SecurityRole::operator_role) ==
               uhal::Status::ok &&
           policy.authorize("switch.set", services::SecurityRole::guest) == uhal::Status::denied &&
           policy.authorize("switch.set", services::SecurityRole::administrator) ==
               uhal::Status::ok &&
           policy.stage_credentials() == uhal::Status::ok &&
           policy.activate_credentials() == uhal::Status::ok &&
           policy.credential_state() == services::CredentialState::active;
}

bool unsupported_verifier_refuses_image() {
    FakeVerifier verifier;
    verifier.status = uhal::Status::unsupported;
    services::SecurityPolicy policy{verifier};
    const std::uint8_t       digest[32]{};
    const std::uint8_t       signature[64]{};
    return policy.verify_image(digest, sizeof(digest), signature, sizeof(signature), 0U) ==
           uhal::Status::denied;
}

}  // namespace

int main() {
    return configuration_round_trip_and_recovery() && configuration_errors_are_explicit() &&
                   security_policy_enforces_roles_and_credentials() &&
                   unsupported_verifier_refuses_image()
               ? 0
               : 1;
}
