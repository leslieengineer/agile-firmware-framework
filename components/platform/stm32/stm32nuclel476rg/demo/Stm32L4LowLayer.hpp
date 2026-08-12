#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void     stm32l4_low_layer_init(void);
void     stm32l4_low_layer_systick_handler(void);
uint32_t stm32l4_low_layer_now_ms(void);
int      stm32l4_low_layer_uart_write(const uint8_t* data, size_t size, uint32_t timeout_ms);
int      stm32l4_low_layer_uart_read(uint8_t* data);
void     stm32l4_low_layer_led_set(int is_on);
int      stm32l4_low_layer_led_is_on(void);

#ifdef __cplusplus
}
#endif