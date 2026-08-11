#pragma once

#include <uhal/II2c.hpp>

namespace devices {

class Sht3x {
public:
    explicit Sht3x(uhal::II2c& bus) : bus_(bus) {}

    uhal::Status start_single_shot_measurement();

private:
    uhal::II2c& bus_;
};

} // namespace devices
