# UHAL Component Catalog

This repository is a reusable firmware component catalog based on a Unified Hardware Abstraction Layer (UHAL). Its goal is to assemble tested components into a new product instead of rewriting chip drivers, protocols, and product logic for every MCU or board.

```text
Application -> Sht3x -> II2c -> Stm32H5I2c -> STM32 HAL
Application -> Sht3x -> II2c -> Esp32C6I2c -> ESP-IDF
```

When porting from STM32H5 to ESP32-C6, the application and `Sht3x` remain unchanged. Only platform adapters and board configuration change.

## Goals

- Reuse logic across MCUs, boards, and RTOS environments.
- Isolate vendor SDKs, registers, DMA, IRQs, and pin mapping in platform code.
- Test device and protocol logic on the host with fake adapters.
- Make a new product primarily an exercise in selecting and composing components.

## Dependency Rules

```mermaid
flowchart TB
    Product[Product / Application]
    Logic[Reusable Logic]
    Contract[UHAL Contracts]
    Adapter[Platform Adapters]
    LowLevel[Low-Level Peripheral Drivers]
    Vendor[Vendor SDK / RTOS / Registers]

    Product --> Logic
    Product --> Adapter
    Logic --> Contract
    Adapter --> Contract
    Adapter --> LowLevel
    LowLevel --> Vendor
```

```text
product -> reusable_logic -> uhal_interfaces <- platform_adapter -> low_level -> vendor_sdk
```

1. `devices`, `protocols`, and `services` may depend only on `components/uhal/core`, `components/uhal/interfaces`, or pure libraries.
2. Only code under `platform` may include STM32 HAL/LL, ESP-IDF, FreeRTOS, or register headers.
3. The product composition root is the only place that creates concrete adapters and injects dependencies.
4. Reusable logic must not use `#ifdef STM32` or `#ifdef ESP32`.
5. Contracts are small capability interfaces; do not create a large `IHal` interface.

## Architecture Layers

### Layer 1: Low-Level Platform Drivers

The closest catalog to hardware and the vendor SDK. It may know peripheral instances, pins, clocks, DMA requests, ISRs, and vendor error types.

- [STM32H5](components/platform/stm32/stm32h5): RCC, GPIO, DMA, I2C, SPI, UART, ADC, FDCAN, timers, security, and other peripherals.
- [ESP32-C6 ESP-IDF](components/platform/esp32c6/esp_idf): GPIO, GDMA, I2C, SPI, UART, TWAI, ADC, PWM, radio, storage, security, and other subsystems.

The current catalog files are placeholders. Implement only the peripherals required by a real board and use case.

### Board Configuration

`products/<product>/board/` owns PCB-specific facts: macros, pinout, clock tree, peripheral instances, baud rates, DMA mapping, interrupt priority, power enables, and startup configuration.

Board configuration is not a reusable platform driver. Two products may use the same adapter while routing the peripheral to different pins.

### Layer 2: UHAL Contracts

[components/uhal/interfaces](components/uhal/interfaces) defines platform-neutral capabilities:

| Contract | Capability |
|---|---|
| `II2c`, `ISpi`, `IUart`, `ICan` | I2C, SPI, UART, and CAN/CAN FD transport |
| `IGpio`, `IAdc`, `IPwm` | Digital I/O, analog read, and PWM output |
| `IClock`, `IStorage`, `IWatchdog` | Time, persistent storage, and system health |

Each object represents a configured resource. For example, `IGpio` does not receive a pin number on each call, `IAdc` does not receive a channel number on each read, and `IUart` does not receive a baud rate on each write.

### Layer 3: Platform Adapters

Adapters implement a UHAL contract with a layer-1 driver or vendor SDK and map vendor errors to `uhal::Status`.

```cpp
class Stm32H5I2c final : public uhal::II2c {
public:
    uhal::Status write(std::uint8_t address,
                        const std::uint8_t* data,
                        std::size_t size) override;
    uhal::Status read(std::uint8_t address,
                       std::uint8_t* data,
                       std::size_t size) override;
};
```

[STM32H5 adapters](components/platform/stm32/stm32h5/adapters) and [ESP32-C6 adapters](components/platform/esp32c6/esp_idf/adapters) contain skeletons for I2C, SPI, UART, CAN, GPIO, ADC, PWM, clock, storage, and watchdog. [Fake adapters](components/platform/fake) implement contracts for host tests.

Adapters must not contain device CRC, Modbus frames, or product policy. They translate UHAL capabilities to vendor APIs only.

ESP-IDF versioning is recorded in [esp_idf_v6.md](components/platform/esp32c6/esp_idf/compatibility/esp_idf_v6.md), not in every driver filename. Products pin an exact ESP-IDF release; API migrations use a compatibility shim or manifest instead of copying drivers.

### Layer 4: Reusable Logic

Reusable logic must not include a vendor SDK.

- [devices](components/devices): chip-specific register protocol, commands, CRC, conversion, retry, and state. `sht3x` is the working sample.
- [protocols](components/protocols): wire and application protocol framing, parsing, checksum, timeout, and retry. `modbus-rtu` is the working sample.
- [services](components/services): reusable policies such as logging, sampling, calibration, telemetry, offline queues, OTA, diagnostics, security, and power management.
- [libraries](components/libraries): pure algorithms independent from UHAL and SDKs, including CRC, serialization, state machines, ring buffers, filters, units, and retry/backoff.

The IoT catalog defines ownership boundaries and planned dependencies. Only `sht3x` and `modbus-rtu` currently contain implementation.

### Layer 5: Product Composition Root

[products](products) contains complete firmware products. `products/<product>/app/main.cpp` creates concrete adapters and injects them into reusable logic. `board/` owns PCB configuration.

```cpp
Stm32H5I2c sensor_bus{board_i2c_config};
Stm32H5Uart modbus_uart{board_uart_config};

devices::Sht3x environment_sensor{sensor_bus};
protocols::ModbusRtuMaster modbus{modbus_uart};
```

Changing platforms replaces `Stm32H5*` with `Esp32C6*`; drivers, protocols, and use cases remain unchanged.

## Project Tree

```text
.
├── CMakeLists.txt
├── README.md
├── Theory.md
├── components/
│   ├── uhal/                             # Layer 2 platform-neutral contracts
│   │   ├── core/                         # Status, common data types, and enums
│   │   └── interfaces/                   # II2c, IUart, IGpio, and other ports
│   ├── platform/
│   │   ├── fake/                         # Host-test adapters
│   │   ├── stm32/stm32h5/
│   │   │   ├── low_level/                # Layer 1: STM32 HAL/LL and IRQ access
│   │   │   └── adapters/                 # Layer 3: UHAL implementations
│   │   └── esp32c6/esp_idf/
│   │       ├── low_level/                # Layer 1: ESP-IDF access
│   │       └── adapters/                 # Layer 3: UHAL implementations
│   ├── devices/                          # Layer 4 chip drivers and IoT catalog
│   ├── protocols/                        # Layer 4 protocol catalog
│   ├── services/                         # Layer 4 product-independent policies
│   └── libraries/                        # Layer 4 pure algorithms
├── products/
│   └── env_monitor/                      # Layer 5 sample product
│       ├── app/main.cpp
│       └── board/
├── config/                               # Shared toolchain and CMake configuration
└── tests/
    ├── unit/                             # Host tests with fake adapters
    └── integration/                      # Board, wiring, timing, and transceiver tests
```

## Design Patterns

| Pattern | Use | Purpose |
|---|---|---|
| Ports and Adapters | Contracts are ports; STM32, ESP32, and fake implementations are adapters | Separates logic from hardware and SDKs |
| Dependency Injection | Constructors receive interface references | Makes dependencies explicit and testable |
| Composition Root | `products/<product>/app/main.cpp` | Controls the object graph in one place |
| Adapter | `Stm32H5I2c : II2c`, `Esp32C6I2c : II2c` | Maps vendor APIs to UHAL |
| Strategy | Interface implementation selected during composition | Changes platform without changing logic |
| Test Double | `FakeI2c`, `FakeUart` | Supports host testing without a board |
| Limited Facade | Small UHAL capabilities hide vendor SDKs | Simplifies APIs without creating a God HAL |

Do not use a Service Locator or Singleton as the primary dependency mechanism. They hide dependencies and make testing harder. A Factory is useful only when an adapter must be selected at runtime.

## SOLID Assessment

| Principle | Current status | Evidence and required discipline |
|---|---|---|
| SRP | Structurally aligned | Low-level code controls peripherals, adapters map APIs, devices understand chips, protocols understand wire formats, and products compose objects. |
| OCP | Extension-oriented | New platform adapters and devices can be added without changing existing device logic. |
| LSP | Not proven yet | Fake, STM32, and ESP32 adapters need shared contract tests for timeout, NACK, invalid input, and partial transfers. |
| ISP | Strong | I2C, UART, SPI, GPIO, and clock capabilities are separated instead of merged into `IHal`. |
| DIP | Strong in design | SHT3x depends on `II2c`, Modbus depends on `IUart`, and reusable logic does not link vendor SDKs. |

## Implementation Workflow

Use complete vertical slices instead of implementing every placeholder first:

1. Select an exact board, MCU, and use case, such as SHT3x on STM32H563.
2. Configure the board I2C, pins, and clocks.
3. Implement the required layer-1 I2C capability.
4. Write `Stm32H5I2c : II2c` and map vendor errors to `Status`.
5. Extend `FakeI2c` to capture requests and script responses or errors.
6. Complete SHT3x command, read, CRC, conversion, and error behavior.
7. Run host unit tests, adapter contract tests, and board integration tests.
8. Assemble the adapter and device at the composition root.

For RS-485 half-duplex, create a transport adapter around `IUart` and `IGpio` to control DE/RE. Modbus core must not know GPIO details.

## Testing

| Test | Environment | Verifies |
|---|---|---|
| Unit | Host | Frames, CRC, parsing, conversion, retry, and error paths |
| Contract | Host or target | Shared UHAL semantics for each adapter |
| Integration | Real board | Pinout, timing, pull-ups, DMA, IRQ, and transceiver behavior |
| Product | Board or SIL | Use cases, scheduling, recovery, and watchdog behavior |

## Build and Test

```powershell
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
.\build\products\env_monitor\env_monitor.exe
```

Remove generated artifacts after validation:

```powershell
Remove-Item -Recurse -Force build
```

## Checklist

- Reusable logic does not include a vendor SDK.
- Dependencies are explicit in `target_link_libraries`.
- Products use constructor injection to compose adapters and logic.
- Reusable logic has host unit tests.
- Board and MCU details live only in `platform/` or `products/<product>/board/`.

---

## Quick Start: Integration Guide

This guide uses the repository's NUCLEO-L476RG reference product as the starting point for a real STM32CubeIDE project. It uses CMSIS/LL in Layer 1, UHAL adapters in Layer 3, and keeps application logic independent of STM32 headers.

### 1. Install the Required Tools

Install STM32CubeIDE and the STM32Cube L4 firmware package. Connect a NUCLEO-L476RG board through the ST-LINK USB connector.

The reference board mapping is:

| Function | MCU resource | Board connection |
| --- | --- | --- |
| CLI | USART2, PA2 TX / PA3 RX, 115200-8-N-1 | ST-LINK Virtual COM port |
| Status LED | PA5 | LD2 green LED |
| Scheduler tick | SysTick, 1 ms | Cortex-M4 exception |

Open the Windows `STMicroelectronics STLink Virtual COM Port` at 115200 baud, 8 data bits, no parity, one stop bit, and no flow control.

### 2. Create the STM32CubeIDE Project

1. Start STM32CubeIDE.
2. Select **File > New > STM32 Project**.
3. In the Board Selector, choose **NUCLEO-L476RG**.
4. Name the project, for example `my_l476rg_product`.
5. Select the STM32CubeIDE toolchain and finish project generation.
6. Open the generated `.ioc` file.

Do not create a Static Library project. Choose a normal STM32 executable project so CubeIDE creates startup assembly, linker scripts, interrupt files, and a flashable ELF.

### 3. Configure CubeMX

In the `.ioc` configuration:

1. Set **SYS > Debug** to **Serial Wire**. This preserves PA13 and PA14 for ST-LINK SWD.
2. Enable **USART2 > Asynchronous**.
3. Confirm PA2 is `USART2_TX` and PA3 is `USART2_RX`.
4. Set USART2 to 115200 baud, 8 data bits, no parity, one stop bit, and no hardware flow control.
5. Set PA5 to `GPIO_Output`. It drives LD2.
6. Keep **SysTick** enabled as the HAL/LL time base.
7. Generate code.

For an onboard ST-LINK Virtual COM connection, PA2/PA3 must remain connected through the NUCLEO board solder bridges. Do not use a separate USB CDC stack for this reference product.

### 4. Add the Catalog to Your Firmware Repository

Use the catalog as a Git submodule or copy it into an `external/` directory:

```bash
git submodule add <your-catalog-url> external/agile-firmware-framework
git submodule update --init --recursive
```

The important reference sources are:

```text
external/agile-firmware-framework/
  components/uhal/
  components/services/environment_monitor/
  components/platform/stm32/stm32l4/nucleo_l476rg/
  products/env_monitor/board/nucleo_l476rg/
```

### 5. Copy or Link the Reference Source Files

For a first integration, copy these sources into your CubeIDE project. Later, replace copies with a CMake subdirectory or linked resources if desired.

| Catalog source | CubeIDE destination | Layer |
| --- | --- | --- |
| `components/uhal/core/include/uhal/*` | `Core/Inc/uhal/` | 2 |
| `components/uhal/interfaces/include/uhal/*` | `Core/Inc/uhal/` | 2 |
| `components/services/environment_monitor/include/services/*` | `Core/Inc/services/` | 4 |
| `components/services/environment_monitor/src/*` | `Core/Src/` | 4 |
| `components/platform/stm32/stm32l4/nucleo_l476rg/low_level/include/stm32l4/*` | `Core/Inc/stm32l4/` | 1 |
| `components/platform/stm32/stm32l4/nucleo_l476rg/low_level/src/*` | `Core/Src/` | 1 |
| `components/platform/stm32/stm32l4/nucleo_l476rg/adapters/include/stm32l4/*` | `Core/Inc/stm32l4/` | 3 |
| `components/platform/stm32/stm32l4/nucleo_l476rg/adapters/src/*` | `Core/Src/` | 3 |

Refresh the CubeIDE project after adding `.cpp` files. Verify that the new files appear in the project explorer and are compiled with the C++ compiler, not the C compiler.

### 6. Replace the Generated Application Entry Point

Keep CubeIDE's startup assembly and linker script. Replace only the generated application body with the reference composition root below. Use a `.cpp` file for the application entry point and ensure the generated C `main.c` is excluded or renamed; a project must contain exactly one `main()`.

```cpp
#include <services/EnvironmentMonitor.hpp>
#include <stm32l4/NucleoL476rgAdapters.hpp>
#include <stm32l4/NucleoL476rgLowLevel.hpp>

int main() {
    stm32l4::nucleo_l476rg::low_level::initialize();

    stm32l4::nucleo_l476rg::adapters::StLinkVirtualCom cli_uart;
    stm32l4::nucleo_l476rg::adapters::Ld2 status_led;
    stm32l4::nucleo_l476rg::adapters::Clock clock;
    services::EnvironmentMonitor monitor{cli_uart, status_led, clock, 0x4C476000U};

    while (true) {
        monitor.run_once();
    }
}
```

This is Layer 5. It is allowed to see concrete adapter classes. It must not contain register writes, UART polling, or application policy.

### 7. Wire the SysTick Interrupt

In the generated interrupt source, replace the SysTick body with:

```cpp
extern "C" void SysTick_Handler() {
    stm32l4::nucleo_l476rg::low_level::systick_interrupt_handler();
}
```

The interrupt must report only to Layer 1. Do not call `EnvironmentMonitor`, publish business events, write telemetry, or call blocking APIs from an ISR.

### 8. Configure Build Settings

For CubeIDE Managed Build, add these include directories to both the C and C++ compiler settings:

```text
Core/Inc
external/agile-firmware-framework/components/uhal/core/include
external/agile-firmware-framework/components/uhal/interfaces/include
external/agile-firmware-framework/components/services/environment_monitor/include
external/agile-firmware-framework/components/platform/stm32/stm32l4/nucleo_l476rg/low_level/include
external/agile-firmware-framework/components/platform/stm32/stm32l4/nucleo_l476rg/adapters/include
```

Compile the catalog `.cpp` files as C++17 or later. Retain the NUCLEO-L476RG defaults generated by CubeIDE:

```text
-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
```

Do not add STM32 HAL calls back into `EnvironmentMonitor`. The reference low-level driver uses CMSIS/LL register primitives, so it does not require `HAL_UART_Transmit`, `HAL_GPIO_WritePin`, or `HAL_GetTick`.

### 9. Build, Flash, and Verify

1. Select **Project > Clean**.
2. Select **Project > Build Project**.
3. Connect the NUCLEO board through ST-LINK USB.
4. Select **Run > Debug As > STM32 MCU C/C++ Application** or use **Run** to flash.
5. Open the ST-LINK Virtual COM terminal.

Expected behavior:

```text
Temperature: 23 C
Temperature: 26 C
```

The first telemetry line is immediate. Later lines occur every five seconds. The simulated temperature changes every second; LD2 is on when the current temperature is above 25 C.

Send these commands followed by Enter:

```text
help
status
```

### 10. Add the Next Peripheral Without Breaking the Architecture

For I2C, SPI, DMA, ADC, or a real sensor, always extend the same vertical path:

1. Add a board-specific hardware API under `low_level/`.
2. Add a Layer 3 adapter implementing the matching UHAL interface.
3. Add or extend a fake adapter and host contract test.
4. Write Layer 4 device/protocol/service code using only the UHAL interface.
5. Construct and inject the new adapter in the Layer 5 composition root.

Never include STM32 headers in Layer 4. Never let an ISR include or call a Layer 4 service. This is what keeps the product testable and makes another board or MCU a replacement of Layers 1, 3, and 5 rather than a rewrite of business logic.

For a full explanation of the reference product execution flow, see [NUCLEO-L476RG.md](docs/NUCLEO-L476RG.md).