# STM32H5 UHAL Adapters

This is architecture layer 3. Each adapter implements a UHAL contract by using the STM32H5 low-level driver catalog and STM32Cube HAL/LL.

For example, `i2c/Stm32H5I2c` implements `uhal::II2c`; it owns the configured I2C instance and converts STM32 status values to `uhal::Status`.

Do not add chip protocol logic here. SHT3x CRC belongs in `components/devices/sht3x`; Modbus framing belongs in `components/protocols/modbus-rtu`.
