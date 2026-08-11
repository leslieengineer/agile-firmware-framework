# Platform Adapters

Platform code is the only place allowed to include an MCU SDK, register header, RTOS API, or vendor handle. Each adapter implements a UHAL interface and translates `Status` values in both directions.

`fake` is the host sample used by tests. `stm32` and `esp32c6` are intentionally empty integration points until a real board and SDK are selected.
