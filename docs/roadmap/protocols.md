# Protocol Roadmap

Implemented or partially implemented protocols are `frame`, `modbus-rtu`, and `mqtt` contract policy.

Deferred protocols and triggers:

| Protocol | Promotion trigger |
|---|---|
| AT command | Specific modem/module BOM and UART ownership |
| BLE GATT | Product selects BLE application profile |
| CANopen | CAN device dictionary and interoperability requirement |
| CoAP | UDP constrained-node use case |
| HTTP client | Product API and TLS policy are defined |
| LoRaWAN | Radio/module and regional certification are selected |
| Modbus TCP | Ethernet/IP industrial endpoint is required |
| MQTT-SN | Non-IP gateway requirement |
| NMEA | GNSS/UART product is selected |

Matter is deferred and is not represented as a framework protocol.
