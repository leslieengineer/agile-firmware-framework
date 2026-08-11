#pragma once

#include <uhal/Status.hpp>

namespace uhal {

enum class GpioLevel : unsigned char {
    low,
    high,
};

class IGpio {
public:
    virtual ~IGpio() = default;

    virtual Status set(GpioLevel level) = 0;
    virtual Status get(GpioLevel& level) = 0;
};

} // namespace uhal
