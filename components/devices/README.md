# Device Components

Device components are layer 4 reusable logic for one chip or chip family. A device component owns register protocol, command bytes, CRC, conversion, retry policy, and device state. It receives only required UHAL interfaces through its constructor.

`sht3x/` is the first real sample: it owns SHT3x command bytes while an `II2c` adapter owns electrical timing and the physical I2C peripheral. Use `_template/` as the structure for the next concrete chip component.

