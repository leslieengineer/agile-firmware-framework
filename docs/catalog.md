# Component Catalog

| Component | Kind | Status | Main API | Dependencies | Host test |
|---|---|---|---|---|---|
| button | Library | Ready | `ButtonInput` | None | Yes |
| fixed ring buffer | Library | Ready | `FixedRingBuffer` | None | Yes |
| retry/deadline | Library | Ready | `BackoffPolicy`, `Deadline` | None | Yes |
| serialization | Library | Ready | bounded byte/JSON readers/writers | None | Yes |
| CRC/EventBus/StateMachine | Library | Ready | headers under `libraries/include` | None | Yes |
| sht3x | Device | Partial | `Sht3x` | `II2c` | Basic |
| frame | Protocol | Ready | `FrameCodec` | CRC/UHAL core | Yes |
| modbus-rtu | Protocol | Partial | `ModbusRtuMaster` | `IUart` | Basic |
| mqtt | Protocol policy | Ready | topics/session | messaging types | Yes |
| binary_switch | Service | Ready | `BinarySwitchService` | `IGpio`, state-store port | Yes |
| configuration | Service | Ready | `ConfigService`, `IConfigStore` | `IStorage`, serialization | Yes |
| security_policy | Service | Ready | `SecurityPolicy` | verifier port | Yes |
| network_manager | Service | Beta | `NetworkManager` | network-link ports, retry | Yes |
| provisioning | Service | Beta | `ProvisioningService` | portal/validator/config ports | Yes |
| indication | Service | Ready | `IndicationService` | indicator port | Yes |
| messaging | Contract | Ready | envelope/transport/handler ports | UHAL core | Yes |
| offline_queue | Service | Beta | `OfflineQueue` | ring buffer/messaging | Yes |
| telemetry | Service | Beta | `TelemetryService` | messaging/queue/MQTT | Yes |
| command_dispatcher | Service | Ready | `CommandDispatcher` | messaging/auth/handlers | Yes |
| time_sync | Service | Beta | `TimeSyncService` | time ports/retry | Yes |
| diagnostics | Service | Ready | `DiagnosticsService` | ring buffer/types | Yes |
| health_monitor | Service | Beta | `HealthMonitor` | watchdog/recovery ports | Yes |
| ota_manager | Policy only | Beta | `OtaService` | abstract firmware/crypto/boot ports | Yes |
| environment_monitor | Sample | Sample | `EnvironmentMonitor` | UART/GPIO/clock | Yes |

Status meanings:

- Ready: API and host behavior are usable.
- Beta: API exists and is host-tested; target adapter/integration hardening remains.
- Partial: only part of the wire/device behavior is implemented.
- Policy only: no vendor storage/transport/boot implementation is provided.
