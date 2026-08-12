# STM32H5 Platform

`low_level/` is architecture layer 1. It contains STM32 HAL/LL or CMSIS register access, RCC, DMA, IRQ, and peripheral initialization. Each peripheral has a public header and an implementation source placeholder, but no peripheral is implemented or built yet.

The catalog spans the STM32H5 family. Peripheral availability, instance count, pins, DMA request mapping, memory size, and optional blocks vary by exact part number. Before implementation, select the target MCU in a product board directory and verify every enabled module against its datasheet and reference manual.

## Rules

- Keep vendor HAL/LL calls inside `low_level/`.
- Implement a UHAL adapter such as `Stm32H5I2c` in `adapters/` only after the corresponding low-level peripheral module exists.
- DMA is a shared controller capability. A UART/SPI/I2C adapter requests DMA through `dma/`; it does not duplicate DMA code.
- The separate `spi_dma`, `uart_dma`, and `i2c_dma` folders document integration paths; they do not represent independent STM32 peripherals.
- Add a module to a product CMake target only when the selected STM32H5 SKU supports it.
