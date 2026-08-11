# Reusable Pure Libraries

This catalog contains algorithms and utility code with no dependency on UHAL, a platform adapter, an RTOS, or a vendor SDK. A device, protocol, service, and host test may all use the same library unchanged.

Create a concrete library only when its behavior is reused. Each library must have its own unit tests because it can run entirely on the host.
