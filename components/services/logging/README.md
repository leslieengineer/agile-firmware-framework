# Logging Service

The public logging contract is currently [ILogger.hpp](../include/services/ILogger.hpp). A platform-specific implementation, such as a UART, RTT, USB, or network logger, belongs under `components/platform/<target>/` and is injected by the product composition root.

Do not make device or protocol code create a logger globally. Accept `ILogger&` only where logging is a real dependency.
