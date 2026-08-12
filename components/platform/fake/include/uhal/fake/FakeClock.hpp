#pragma once

#include <cstdint>
#include <uhal/IClock.hpp>

namespace uhal::fake {

class FakeClock final : public IClock {
public:
    std::uint32_t now_ms() const override {
        return now_ms_;
    }

    void sleep_ms(std::uint32_t duration_ms) override {
        now_ms_ += duration_ms;
    }

    void advance_ms(std::uint32_t duration_ms) {
        now_ms_ += duration_ms;
    }

private:
    std::uint32_t now_ms_ = 0U;
};

}  // namespace uhal::fake
