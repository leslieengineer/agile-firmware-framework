# Connectivity Modules

Future chip-specific components: cellular modems, LoRa transceivers, Wi-Fi coprocessors, and satellite modems. Typical devices include SIM7080, BG95, SX1262, and ESP-AT modules.

The module driver manages the hardware command set and reset pins. Reusable AT parsing or LoRaWAN protocol logic belongs in `components/protocols`.
