#include <chrono>
#include <cstdint>
#include <thread>
#include <uhal/fake/FakeGpio.hpp>
#include <uhal/fake/FakeUart.hpp>

#include "EnvironmentMonitor.hpp"

namespace {

class HostClock final : public uhal::IClock {
public:
    std::uint32_t now_ms() const override {
        const auto elapsed = std::chrono::steady_clock::now() - start_time_;
        return static_cast<std::uint32_t>(
            std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count());
    }

    void sleep_ms(std::uint32_t duration_ms) override {
        std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms));
    }

private:
    const std::chrono::steady_clock::time_point start_time_ = std::chrono::steady_clock::now();
};

}  // namespace

int main() {
    uhal::fake::FakeUart cli_uart;
    uhal::fake::FakeGpio status_led;
    HostClock            clock;
    EnvironmentMonitor   monitor{cli_uart, status_led, clock, 0x4C476000U};

    while (true) {
        monitor.run_once();
        clock.sleep_ms(1);
    }
}
