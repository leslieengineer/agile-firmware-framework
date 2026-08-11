# Health Monitor Service Template

Use this directory for reusable system health policy: periodic checks, fault aggregation, recovery decisions, and watchdog feed conditions.

The service may receive `uhal::IClock&`, `uhal::IWatchdog&`, and an optional logger. It defines policy; a platform adapter performs the actual watchdog operation.
