#include <stm32l4/NucleoL476rgLowLevel.hpp>

extern "C" void SysTick_Handler() {
    stm32l4::nucleo_l476rg::low_level::systick_interrupt_handler();
}
