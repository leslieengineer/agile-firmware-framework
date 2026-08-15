# Service Roadmap

| Capability | Status | Promotion trigger |
|---|---|---|
| alerting | Deferred to Linux Gateway | Node requires autonomous offline alert/action |
| calibration | Deferred | A real sensor product needs persisted calibration |
| data logger | Gateway-owned | Node offline history requirement cannot be met by OfflineQueue |
| data pipeline | Gateway-owned | Node requires local aggregation before transport |
| filtering | Deferred | A sensor product demonstrates reusable filter policy |
| logging implementation | Product/platform-owned | Existing `ILogger` contract is sufficient |
| power manager | Deferred | Battery/deep-sleep product defines wake and power contracts |
| scheduler | Product runtime-owned | A second product requires reusable scheduling policy |
| sensor sampling | Deferred | A sensor device and timing contract are implemented |

Implemented services remain under `components/services/` and are listed in `docs/catalog.md`.
