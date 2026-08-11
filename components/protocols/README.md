# Protocol Components

Protocol components are layer 4 reusable logic for a wire or application protocol. They own framing, parser, checksum/CRC, timeout/retry policy, and protocol-specific errors. Their transport is injected as a narrow UHAL contract.

`modbus-rtu/` is the first real sample: it owns slave addressing and will own CRC/response parsing, while `IUart` owns byte transport. Use `_template/` as the structure for a new protocol.

