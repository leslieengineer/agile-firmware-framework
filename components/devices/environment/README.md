# Environment Sensors

Future chip-specific components: temperature/humidity, pressure, air-quality, light, soil moisture, and gas sensors. Typical devices include BME280/BME680, BMP390, SCD4x, VEML7700, and SHT4x.

Implement one concrete chip family per child component; inject `II2c` or `ISpi` and keep conversion/calibration logic here.
