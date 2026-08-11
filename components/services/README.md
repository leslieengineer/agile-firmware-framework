# Shared Services

Services are layer 4 reusable policies that are not tied to a single chip or wire protocol. They can coordinate cross-cutting concerns while remaining independent from an MCU SDK.

- `logging/`: public contract is `ILogger.hpp`; platform owns the output implementation.
- `configuration/`: future reusable configuration and validation policy over `IStorage`.
- `health_monitor/`: future health/recovery/watchdog policy over `IClock` and `IWatchdog`.
- `scheduler/`: future reusable scheduling policy; a product-specific main loop stays in the product.

Create a service only when at least two products or use cases need the policy. This prevents `services/` from becoming a miscellaneous utility folder.


