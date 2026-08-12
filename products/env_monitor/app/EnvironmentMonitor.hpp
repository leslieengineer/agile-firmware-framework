#pragma once

#include <cstddef>
#include <cstdint>
#include <uhal/IClock.hpp>
#include <uhal/IGpio.hpp>
#include <uhal/IUart.hpp>

class EnvironmentMonitor final {
public:
    EnvironmentMonitor(uhal::IUart& cli_uart, uhal::IGpio& status_led, uhal::IClock& clock,
                       std::uint32_t random_seed);

    void run_once();

private:
    static constexpr std::uint32_t kCliPeriodMs     = 10;
    static constexpr std::uint32_t kSensorPeriodMs  = 1000;
    static constexpr std::size_t   kCommandCapacity = 32;

    void         run_cli_task();
    void         run_sensor_task();
    void         process_command();
    void         write_text(const char* text);
    std::uint8_t next_temperature_celsius();

    uhal::IUart&    cli_uart_;
    uhal::IGpio&    status_led_;
    uhal::IClock&   clock_;
    std::uint32_t   random_state_;
    std::uint32_t   last_cli_ms_         = 0;
    std::uint32_t   last_sensor_ms_      = 0;
    std::uint8_t    temperature_celsius_ = 23;
    uhal::GpioLevel led_level_           = uhal::GpioLevel::low;
    char            command_[kCommandCapacity]{};
    std::size_t     command_length_ = 0;
};