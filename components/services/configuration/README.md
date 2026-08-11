# Configuration Service Template

Use this directory when multiple products need platform-neutral configuration rules: default values, validation, schema migration, and serialization policy.

The service receives `uhal::IStorage&`; it must not call STM32 flash, ESP-IDF NVS, or a filesystem directly. A board/product decides which storage adapter is injected.
