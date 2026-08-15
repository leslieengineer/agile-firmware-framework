#pragma once

#include <cstdint>

namespace libraries {

enum class ButtonEvent : std::uint8_t { none, press_started, short_press };

struct ButtonInputConfig {
    std::uint32_t debounce_ms        = 25U;
    std::uint32_t short_press_max_ms = 1000U;
};

class ButtonInput final {
public:
    explicit ButtonInput(ButtonInputConfig config = {});
    ButtonEvent   update(bool raw_pressed, std::uint32_t now_ms);
    bool          is_pressed() const;
    bool          is_active() const;
    std::uint32_t hold_ms(std::uint32_t now_ms) const;

private:
    ButtonInputConfig config_{};
    bool              raw_last_       = false;
    bool              stable_pressed_ = false;
    bool              debouncing_     = false;
    std::uint32_t     last_change_ms_ = 0U;
    std::uint32_t     press_start_ms_ = 0U;
};

}  // namespace libraries
