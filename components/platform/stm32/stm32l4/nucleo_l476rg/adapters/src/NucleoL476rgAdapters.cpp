#include <stm32l4/NucleoL476rgAdapters.hpp>
#include <stm32l4/NucleoL476rgLowLevel.hpp>

namespace stm32l4::nucleo_l476rg::adapters {

std::uint32_t Clock::now_ms() const {
    return low_level::now_ms();
}

void Clock::sleep_ms(std::uint32_t duration_ms) {
    const std::uint32_t start_ms = low_level::now_ms();
    while (static_cast<std::uint32_t>(low_level::now_ms() - start_ms) < duration_ms) {
    }
}

uhal::Status Ld2::set(uhal::GpioLevel level) {
    low_level::ld2_set(level == uhal::GpioLevel::high);
    return uhal::Status::ok;
}

uhal::Status Ld2::get(uhal::GpioLevel& level) {
    level = low_level::ld2_is_on() ? uhal::GpioLevel::high : uhal::GpioLevel::low;
    return uhal::Status::ok;
}

uhal::Status StLinkVirtualCom::write(const std::uint8_t* data, std::size_t size) {
    if (data == nullptr && size != 0U) {
        return uhal::Status::invalid_argument;
    }
    return low_level::uart2_write(data, size, 100U) ? uhal::Status::ok : uhal::Status::timeout;
}

uhal::Status StLinkVirtualCom::read(std::uint8_t* data, std::size_t capacity, std::size_t& received,
                                    std::uint32_t timeout_ms) {
    received = 0U;
    if (data == nullptr || capacity == 0U) {
        return uhal::Status::invalid_argument;
    }

    const std::uint32_t start_ms = low_level::now_ms();
    do {
        if (low_level::uart2_try_read(*data)) {
            received = 1U;
            return uhal::Status::ok;
        }
    } while (timeout_ms != 0U &&
             static_cast<std::uint32_t>(low_level::now_ms() - start_ms) < timeout_ms);

    return uhal::Status::timeout;
}

}  // namespace stm32l4::nucleo_l476rg::adapters
