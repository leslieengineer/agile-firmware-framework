#pragma once

#include <cstddef>
#include <cstdint>

namespace stm32l4::nucleo_l476rg::low_level {

void initialize();
void systick_interrupt_handler();

std::uint32_t now_ms();
bool          uart2_write(const std::uint8_t* data, std::size_t size, std::uint32_t timeout_ms);
bool          uart2_try_read(std::uint8_t& data);
void          ld2_set(bool is_on);
bool          ld2_is_on();

}  // namespace stm32l4::nucleo_l476rg::low_level
