#include "Application.hpp"

#include "EnvironmentMonitor.hpp"
#include "Stm32L4Adapters.hpp"

namespace {

EnvironmentMonitor* monitor = nullptr;

}  // namespace

extern "C" void application_init(void) {
    static Stm32L4Uart        application_uart;
    static Stm32L4Gpio        application_led;
    static Stm32L4Clock       application_clock;
    static EnvironmentMonitor application_monitor{application_uart, application_led,
                                                  application_clock, 0x4C476000U};

    monitor = &application_monitor;
}

extern "C" void application_run_once(void) {
    if (monitor != nullptr) {
        monitor->run_once();
    }
}