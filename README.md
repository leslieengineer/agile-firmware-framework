# Agile Firmware Framework

Reusable embedded C++ component catalog built around UHAL contracts. Products select and compose tested capabilities instead of copying drivers and business policy.

## Architecture

```text
product -> reusable logic -> UHAL contracts <- platform adapters -> low-level -> vendor SDK
```

- Layer 1: `components/platform/<platform>/.../low_level`
- Layer 2: `components/uhal/core` and `components/uhal/interfaces`
- Layer 3: `components/platform/<platform>/.../adapters`
- Layer 4: `components/devices`, `protocols`, `services`, `libraries`
- Layer 5: product repository composition and board configuration

See [architecture layers](docs/architecture/layers.md).

## Catalog

See [component catalog](docs/catalog.md) for implementation status, dependencies and tests. Placeholder capabilities live in [roadmap docs](docs/roadmap/) rather than the active source tree.

## Integration and learning

- [ESP-IDF integration](docs/integration/esp-idf.md)
- [STM32CubeIDE integration](docs/integration/stm32-cubeide.md)
- [Design patterns and SOLID](docs/architecture/design-patterns.md)
- [Testing strategy](docs/testing.md)
- Products import only selected capabilities through explicit bridge components.
- Public headers use `include/`; implementations use `src/`.
- Source lists and dependencies are explicit; no blanket globbing or global facade header.

## Build and test

```powershell
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Dependency rules

- Layer 4 depends only on UHAL contracts, shared value types and pure libraries.
- Vendor SDK/RTOS/register headers stay in platform low-level/adapters or product runtime infrastructure.
- Board pins, polarity, credentials and persistence schemas stay in products.
- The product composition root creates concrete adapters and injects dependencies.
- No service locator or hidden mutable singleton API.
- Fixed-capacity resources and explicit failure semantics are required.

## Roadmap

- [Services](docs/roadmap/services.md)
- [Protocols](docs/roadmap/protocols.md)
- [Devices](docs/roadmap/devices.md)
- [Libraries](docs/roadmap/libraries.md)
