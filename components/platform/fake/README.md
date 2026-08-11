# Fake Platform Adapter

`FakeI2c` and `FakeUart` implement the same UHAL contracts as real hardware but return deterministic values. They exist so device and protocol logic can be compiled and tested on a host PC.

Extend these fakes with scripted responses and captured writes when a unit test needs to validate a bus frame. Do not reuse them in firmware builds.
