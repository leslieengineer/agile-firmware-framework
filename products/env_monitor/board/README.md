# Environment Monitor Board Configuration

This directory owns concrete facts about one PCB: pin mapping, I2C/UART instances, baud rate, clock, DMA selection, and power-enable GPIOs.

Create one child directory per real board, such as `nucleo_l476rg/`. Its startup and peripheral configuration may be platform-specific; device and protocol logic must stay in `components/`.

