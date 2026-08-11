# SHT3x Sample Device Driver

`Sht3x` demonstrates a device driver that depends only on `uhal::II2c`. `start_single_shot_measurement()` sends the SHT3x high-repeatability command `0x24 0x00` to default address `0x44`.

Implement the next steps in this component: wait using an injected clock interface, read six bytes, verify both CRC values, and convert raw values to temperature and humidity. Keep all those rules here, never in the STM32 or ESP32 adapter.
