#include <services/EnvironmentMonitor.hpp>
#include <stm32l4/NucleoL476rgAdapters.hpp>
#include <stm32l4/NucleoL476rgLowLevel.hpp>

int main() {
    stm32l4::nucleo_l476rg::low_level::initialize();

    stm32l4::nucleo_l476rg::adapters::StLinkVirtualCom cli_uart;
    stm32l4::nucleo_l476rg::adapters::Ld2              status_led;
    stm32l4::nucleo_l476rg::adapters::Clock            clock;
    services::EnvironmentMonitor monitor{cli_uart, status_led, clock, 0x4C476000U};

    while (true) {
        monitor.run_once();
    }
}
