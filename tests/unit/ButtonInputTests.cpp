#include <cstdint>
#include <libraries/ButtonInput.hpp>

namespace {
bool clean_short_press() {
    libraries::ButtonInput button;
    if (button.update(true, 10U) != libraries::ButtonEvent::none) return false;
    if (button.update(true, 35U) != libraries::ButtonEvent::press_started) return false;
    if (button.update(false, 200U) != libraries::ButtonEvent::none) return false;
    return button.update(false, 225U) == libraries::ButtonEvent::short_press && !button.is_active();
}
bool rejects_bounce() {
    libraries::ButtonInput button;
    button.update(true, 10U);
    button.update(false, 15U);
    button.update(true, 20U);
    if (button.update(true, 45U) != libraries::ButtonEvent::press_started) return false;
    button.update(false, 100U);
    button.update(true, 105U);
    button.update(false, 110U);
    if (button.update(false, 135U) != libraries::ButtonEvent::short_press) return false;
    return button.update(false, 200U) == libraries::ButtonEvent::none;
}
bool classifies_hold_durations() {
    libraries::ButtonInput button;
    button.update(true, 0U);
    button.update(true, 25U);
    button.update(false, 3025U);
    if (button.update(false, 3050U) != libraries::ButtonEvent::none) return false;

    button.update(true, 4000U);
    button.update(true, 4025U);
    button.update(false, 9025U);
    if (button.update(false, 9050U) != libraries::ButtonEvent::commissioning_press) return false;

    button.update(true, 10000U);
    button.update(true, 10025U);
    button.update(false, 20025U);
    return button.update(false, 20050U) == libraries::ButtonEvent::factory_reset_press;
}
bool handles_timer_wrap() {
    libraries::ButtonInput  button;
    constexpr std::uint32_t start = 0xFFFFFF00U;
    button.update(true, start);
    if (button.update(true, start + 25U) != libraries::ButtonEvent::press_started) return false;
    button.update(false, 0x20U);
    return button.update(false, 0x39U) == libraries::ButtonEvent::short_press;
}
}  // namespace
int main() {
    return clean_short_press() && rejects_bounce() && classifies_hold_durations() && handles_timer_wrap()
               ? 0
               : 1;
}
