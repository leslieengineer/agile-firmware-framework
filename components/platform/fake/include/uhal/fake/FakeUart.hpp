#pragma once

#include <uhal/IUart.hpp>

namespace uhal::fake {

class FakeUart final : public IUart {
public:
    Status write(const std::uint8_t*, std::size_t) override {
        return Status::ok;
    }

    Status read(std::uint8_t*, std::size_t, std::size_t& received, std::uint32_t) override {
        received = 0;
        return Status::timeout;
    }
};

} // namespace uhal::fake
