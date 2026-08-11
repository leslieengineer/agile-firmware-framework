# External Storage Devices

Future chip-specific components: SPI NOR flash, EEPROM, FRAM, SD card, and external ADC/DAC. Typical devices include W25Q, AT24C, MB85RC, ADS1115, and MCP4725.

The chip driver uses `II2c` or `ISpi`; a product-facing persistence policy should use `IStorage` through a service.
