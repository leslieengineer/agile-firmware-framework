# Display Devices

Future chip-specific components: OLED, LCD controller, e-paper, LED display, and touch controller. Typical devices include SSD1306, ST7789, ILI9341, and GT911.

Use `II2c`, `ISpi`, `IPwm`, and optional `IGpio`. Drawing primitives should be separate from a chip-specific transport driver when reuse is needed.
