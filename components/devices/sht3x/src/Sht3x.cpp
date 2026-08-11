#include <devices/Sht3x.hpp>

namespace devices {

uhal::Status Sht3x::start_single_shot_measurement() {
    constexpr std::uint8_t address = 0x44;
    constexpr std::uint8_t command[] = {0x24, 0x00};
    return bus_.write(address, command, sizeof(command));
}

} // namespace devices
