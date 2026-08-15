# Architecture Layers

```text
Layer 5 product -> Layer 4 reusable logic -> Layer 2 UHAL contracts
Layer 5 product -> Layer 3 adapters -> Layer 1 low-level -> vendor SDK
```

- Layer 1 owns vendor calls, registers and IRQ primitives.
- Layer 2 owns small platform-neutral capability contracts.
- Layer 3 maps configured platform resources to UHAL semantics.
- Layer 4 owns reusable devices, protocols, policies and pure algorithms.
- Layer 5 owns product use cases, board selection, composition, scheduling and product persistence schemas.

Vendor-neutral application and Layer 4 code must not include SDK, RTOS, board or product storage headers. Concrete object creation belongs to the product composition root. Direct synchronous calls are preferred for one-producer/one-consumer control flow; EventBus is reserved for asynchronous multi-subscriber events.
