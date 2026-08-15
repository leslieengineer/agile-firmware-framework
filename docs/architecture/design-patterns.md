# Design Patterns and SOLID

| Pattern | Use |
|---|---|
| Ports and Adapters | UHAL contracts separate reusable logic from MCU SDKs |
| Constructor Injection | Composition root supplies required capabilities explicitly |
| Composition Root | Product owns concrete object graph and lifetime |
| Adapter | Platform class maps Layer 1/vendor results to UHAL semantics |
| Strategy | Product selects interchangeable transport/storage/platform implementation |
| Test Double | Host fakes exercise reusable logic without hardware |
| Limited Facade | Small capability interfaces hide vendor APIs without creating a broad `IHal` |

Do not use a service locator or hidden mutable Singleton API. A factory is justified only for runtime selection among already-defined strategies.

SOLID guidance:

- SRP: low-level controls hardware, adapters translate, protocols own wire format, devices own chip behavior, services own policy, products compose.
- OCP: add an adapter/device/service rather than changing unrelated layers.
- LSP: real and fake adapters must share status, timeout and partial-operation semantics.
- ISP: use separate GPIO, UART, I2C, clock, storage and watchdog capabilities.
- DIP: reusable logic depends on contracts; product composition selects implementations.
