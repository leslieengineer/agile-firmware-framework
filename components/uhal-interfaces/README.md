# UHAL Interfaces

This is the contract layer between reusable logic and hardware. Each interface describes one capability required by a device driver, protocol, or application. Logic may include files in this directory and `uhal-core`; platform adapters implement the interfaces with STM32 HAL/LL, ESP-IDF, or host fakes.

## Current Contracts

| Contract | Capability | Example consumer |
|---|---|---|
| `II2c` | I2C read and write | SHT3x, EEPROM, IMU |
| `ISpi` | Full-duplex SPI transfer | Flash, ADC, display |
| `IUart` | UART byte transport | Modbus RTU, GNSS |
| `ICan` | CAN/CAN FD frame transport | CANopen, DroneCAN |
| `IGpio` | One configured digital pin | Sensor reset, chip select, RS-485 enable |
| `IAdc` | One configured analog channel | Battery monitor, analog sensor |
| `IPwm` | One configured PWM channel | LED dimming, motor, servo output |
| `IClock` | Time source and delay | Sensor conversion time, retry policy |
| `IStorage` | Byte-addressable persistent storage | Settings, calibration, event log |
| `IWatchdog` | Watchdog feed | System health service |

## Design Rules

1. A contract describes a **capability**, not a vendor peripheral. Use `ICan`, not `IFdcan` or `ITwai`.
2. Each object represents a resource already configured by the board or product. `IGpio` does not receive a pin number; `IAdc` does not receive a channel number; `IPwm` does not receive a timer instance.
3. Pin mapping, baud rates, clocks, DMA streams or channels, interrupt priority, and SDK handles belong in the platform adapter or `products/<product>/board/`.
4. Extend an interface only after a real use case exists. Do not add asynchronous transfer, DMA, or configuration methods to every interface merely because the peripheral supports them.
5. `Status` must not expose `HAL_StatusTypeDef` or `esp_err_t`. An adapter maps vendor errors to `uhal::Status`.

## Example

```cpp
class DriverWithReset {
public:
    DriverWithReset(uhal::II2c& bus, uhal::IGpio& reset, uhal::IClock& clock)
        : bus_(bus), reset_(reset), clock_(clock) {}

private:
    uhal::II2c& bus_;
    uhal::IGpio& reset_;
    uhal::IClock& clock_;
};
```

`DriverWithReset` does not know whether the reset pin is GPIOA.4 or GPIO8, and it does not know whether I2C is implemented by STM32H5 or ESP32-C6. The product composition root creates the board-specific adapters and injects them through the constructor.
