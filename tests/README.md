# Tests

Tests verify reusable behavior before a component reaches a board. Unit tests run on the host using fake adapters; integration tests run with a real board, wiring, and transceiver.

Keep a test close to the boundary it verifies. A Modbus frame test belongs in `unit/`, while an RS-485 direction-pin timing check belongs in `integration/`.
