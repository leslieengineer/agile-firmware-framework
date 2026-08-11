# Device Component Template

Copy this directory only when adding a driver for a concrete chip or chip family, such as an IMU, ADC, EEPROM, display controller, or environmental sensor.

```text
<chip-name>/
├── CMakeLists.txt
├── README.md
├── include/devices/<ChipName>.hpp
└── src/<ChipName>.cpp
```

The public constructor receives only the UHAL capabilities the chip needs, such as `II2c`, `ISpi`, `IGpio`, or `IClock`. Register addresses, CRC, state machine, conversion, and retry rules belong in this component. Vendor SDK headers, pin numbers, and DMA configuration do not.
