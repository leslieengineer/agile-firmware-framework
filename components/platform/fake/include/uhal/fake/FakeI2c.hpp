#pragma once

#include <uhal/II2c.hpp>

namespace uhal::fake {

class FakeI2c final : public II2c {
public:
    Status write(std::uint8_t, const std::uint8_t*, std::size_t) override {
        return Status::ok;
    }

    Status read(std::uint8_t, std::uint8_t* data, std::size_t size) override {
        for (std::size_t index = 0; index < size; ++index) {
            data[index] = 0;
        }
        return Status::ok;
    }
};

} // namespace uhal::fake
