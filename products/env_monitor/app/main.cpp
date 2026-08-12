#include <services/EnvironmentMonitor.hpp>
#include <uhal/fake/FakeClock.hpp>
#include <uhal/fake/FakeGpio.hpp>
#include <uhal/fake/FakeUart.hpp>

int main() {
    uhal::fake::FakeUart         cli_uart;
    uhal::fake::FakeGpio         status_led;
    uhal::fake::FakeClock        clock;
    services::EnvironmentMonitor monitor{cli_uart, status_led, clock, 0x4C476000U};

    for (std::uint32_t elapsed_ms = 0U; elapsed_ms < 10000U; ++elapsed_ms) {
        monitor.run_once();
        clock.advance_ms(1U);
    }
    return 0;
}
