#pragma once

#include <cstdint>
#include <uhal/Status.hpp>

namespace uhal {

class IPwm {
public:
    virtual ~IPwm() = default;

    virtual Status set_duty_per_mille(std::uint16_t duty_per_mille) = 0;
    virtual Status enable()                                         = 0;
    virtual Status disable()                                        = 0;
};

}  // namespace uhal
