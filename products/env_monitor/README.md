# Environment Monitor Product Sample

This product demonstrates one complete vertical slice of the five-layer architecture.

| Entry point | Purpose |
| --- | --- |
| `app/main.cpp` | Host composition root. It injects fake UHAL adapters and is built by default. |
| `board/nucleo_l476rg/main.cpp` | STM32 composition root. It initializes Layer 1, creates Layer 3 adapters, and injects them into the Layer 4 service. |
| `board/nucleo_l476rg/interrupts.cpp` | Board ISR bridge. SysTick reports only to Layer 1. |

The reusable `services::EnvironmentMonitor` service reads no board headers. It produces simulated temperature telemetry through `IUart`, controls `IGpio`, and schedules work through `IClock`.

## STM32 Firmware Build

The host configuration does not require STM32Cube files. To build the NUCLEO-L476RG image, use an ARM Cortex-M4 toolchain and configure:

```text
ENV_MONITOR_BUILD_STM32L476RG=ON
STM32L4_CMSIS_DEVICE_INCLUDE_DIR=<Cube CMSIS device include directory>
STM32L4_CMSIS_CORE_INCLUDE_DIR=<Cube CMSIS core include directory>
STM32L476RG_STARTUP_SOURCE=<startup_stm32l476xx.s>
STM32L476RG_LINKER_SCRIPT=<STM32L476RG flash linker script>
```

USART2 on PA2/PA3 is the ST-LINK Virtual COM port at 115200-8-N-1. LD2 is PA5.
