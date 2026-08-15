# Testing Strategy

| Level | Environment | Verifies |
|---|---|---|
| Unit | Host | State machines, CRC, parsing, conversion, policy and errors |
| Contract | Host/target | Shared UHAL semantics across fake and real adapters |
| Integration | Real board | Pinout, timing, pulls, DMA, IRQ and transceiver behavior |
| Product | Board/SIL | Composition, scheduling, degraded operation and recovery |

Every implemented component requires a host-test target. Platform behavior that cannot be represented truthfully by a fake requires a target integration test. Hardware tests must state board, wiring, firmware artifact, reset/flash procedure and expected evidence.

Commands:

```powershell
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```
