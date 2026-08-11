# Positioning Devices

Future chip-specific components: GNSS receivers, cellular positioning modems, and ranging modules. Typical devices include u-blox GNSS receivers and L76K.

Use `IUart` or `ISpi` for transport. NMEA/UBX parser logic may be a reusable protocol component when it is independent of a particular module.
