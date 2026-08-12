# Environment Monitor Product Sample

## Product Requirements

- **Target board:** NUCLEO-L476RG (STM32L476RG).
- **CLI transport:** USB CDC virtual COM port through the board micro-USB connector. The board adapter exposes it as `uhal::IUart`.
- **Status LED:** LD2 (PA5). The board adapter exposes this output as `uhal::IGpio`.
- **Temperature input:** A deterministic pseudo-random value in the inclusive range $[23, 27]$ degrees Celsius, sampled every 1000 ms.
- **Control rule:** The LED is on when temperature is greater than 25 degrees Celsius; otherwise it is off.
- **CLI task:** Polls UART every 10 ms without blocking. `help` lists commands and `status` reports the latest temperature and LED state.

## Architecture

`EnvironmentMonitor` is application logic and depends only on `IUart`, `IGpio`, and `IClock`. The composition root supplies host fakes for the example build. For NUCLEO-L476RG firmware, replace only these fakes with STM32L4 low-layer adapters:

| UHAL port | Board binding | Low-layer responsibility |
| --- | --- | --- |
| `IUart` | USB CDC virtual COM | Configure USB device CDC and bridge non-blocking RX/TX. |
| `IGpio` | LD2, PA5 | Enable GPIOA and configure PA5 as push-pull output. |
| `IClock` | SysTick or HAL tick | Return a wrapping millisecond tick and sleep/yield as appropriate. |

The application runs a cooperative main loop. Neither task uses delays internally, so adding another periodic task does not block CLI input.
