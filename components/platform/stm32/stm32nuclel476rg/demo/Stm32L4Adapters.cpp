#include "Stm32L4Adapters.hpp"

#include "Stm32L4LowLayer.hpp"


std::uint32_t Stm32L4Clock::now_ms() const {
    return stm32l4_low_layer_now_ms();
}

void Stm32L4Clock::sleep_ms(std::uint32_t duration_ms) {
    const std::uint32_t start_ms = stm32l4_low_layer_now_ms();
    while (static_cast<std::uint32_t>(stm32l4_low_layer_now_ms() - start_ms) < duration_ms) {
    }
}

uhal::Status Stm32L4Gpio::set(uhal::GpioLevel level) {
    stm32l4_low_layer_led_set(level == uhal::GpioLevel::high ? 1 : 0);
    return uhal::Status::ok;
}

uhal::Status Stm32L4Gpio::get(uhal::GpioLevel& level) {
    level = stm32l4_low_layer_led_is_on() != 0 ? uhal::GpioLevel::high : uhal::GpioLevel::low;
    return uhal::Status::ok;
}

uhal::Status Stm32L4Uart::write(const std::uint8_t* data, std::size_t size) {
    return stm32l4_low_layer_uart_write(data, size, 100U) == 0 ? uhal::Status::ok
                                                               : uhal::Status::timeout;
}

uhal::Status Stm32L4Uart::read(std::uint8_t* data, std::size_t capacity, std::size_t& received,
                               std::uint32_t timeout_ms) {
    received = 0;
    if (capacity == 0) {
        return uhal::Status::invalid_argument;
    }

    if (timeout_ms != 0U) {
        const std::uint32_t start_ms = stm32l4_low_layer_now_ms();
        while (stm32l4_low_layer_uart_read(data) == 0) {
            if (static_cast<std::uint32_t>(stm32l4_low_layer_now_ms() - start_ms) >= timeout_ms) {
                return uhal::Status::timeout;
            }
        }
    } else if (stm32l4_low_layer_uart_read(data) == 0) {
        return uhal::Status::timeout;
    }

    received = 1;
    return uhal::Status::ok;
}