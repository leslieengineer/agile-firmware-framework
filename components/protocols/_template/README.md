# Protocol Component Template

Copy this directory only when adding a reusable wire/application protocol, such as NMEA, CANopen, MQTT-SN, a proprietary frame format, or a bootloader protocol.

```text
<protocol-name>/
├── CMakeLists.txt
├── README.md
├── include/protocols/<ProtocolName>.hpp
└── src/<ProtocolName>.cpp
```

The protocol owns framing, encoding, parsing, CRC/checksum, timeout/retry policy, and protocol error handling. It receives a narrow UHAL transport such as `IUart` or `ICan`; it must not know pin mapping, UART handles, RS-485 GPIO numbers, or vendor APIs.
