# NUCLEO-L476RG Platform Slice

This directory is a reference implementation of the hardware-dependent half of the five-layer architecture.

| Directory | Layer | Responsibility |
| --- | --- | --- |
| `low_level/` | 1 | CMSIS/LL register access for PLL, SysTick, LD2, GPIOA, and USART2. |
| `adapters/` | 3 | UHAL implementations that delegate only to `low_level/`. |

## Interrupt Rule

`SysTick_Handler` calls only `low_level::systick_interrupt_handler()`. A peripheral ISR must not include or invoke a Layer 4 service. When asynchronous data is needed, Layer 1 writes to a bounded buffer or invokes a registered function-pointer callback; Layer 3 converts that signal into the adapter's synchronization mechanism.

## Extension Pattern

To add I2C:

1. Add a configured I2C peripheral API in `low_level/`.
2. Add an `II2c` implementation in `adapters/`.
3. Add a fake I2C behavior and contract test.
4. Inject the adapter from a product board composition root.

Do not add STM32 register headers to a reusable device, protocol, or service.
