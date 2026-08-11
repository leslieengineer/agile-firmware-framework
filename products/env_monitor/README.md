# Environment Monitor Product Sample

This sample is the composition root. It creates `FakeI2c`, `FakeUart`, `Sht3x`, and `ModbusRtuMaster`, then invokes each component through its public API.

For a real board, replace only the fake objects in `app/main.cpp` with STM32 or ESP32-C6 adapter objects configured by the `board/` directory. The SHT3x and Modbus components remain unchanged.
