# NUCLEO-L476RG Board Configuration

## Hardware Mapping

| Product resource | MCU peripheral and pins | Board connection |
| --- | --- | --- |
| CLI | USART2, PA2 TX / PA3 RX, 115200-8-N-1 | ST-LINK Virtual COM through CN1 USB |
| Status LED | PA5, LD2 | Green user LED |
| Scheduler tick | Cortex-M SysTick, 1 ms | `SysTick_Handler` in `interrupts.cpp` |

## Layer Responsibilities

- `NucleoL476rgLowLevel` is Layer 1. It configures the PLL, SysTick, PA5, PA2/PA3, and USART2 using CMSIS/LL primitives.
- `NucleoL476rgAdapters` is Layer 3. It implements `IUart`, `IGpio`, and `IClock` using the low-level API.
- `main.cpp` is Layer 5. It creates adapters and injects them into the reusable Layer 4 `EnvironmentMonitor` service.

No Layer 4 service includes STM32 headers or calls STM32 LL/CMSIS functions.

## Cross Build Inputs

The STM32 CMake target requires a Cortex-M4 toolchain, startup assembly, linker script, and CMSIS include directories supplied by the chosen STM32Cube package. Set:

- `STM32L4_CMSIS_DEVICE_INCLUDE_DIR`
- `STM32L4_CMSIS_CORE_INCLUDE_DIR`
- `STM32L4_LL_INCLUDE_DIR`
- `STM32L476RG_STARTUP_SOURCE`
- `STM32L476RG_LINKER_SCRIPT`

Then configure with `-DENV_MONITOR_BUILD_STM32L476RG=ON`.
