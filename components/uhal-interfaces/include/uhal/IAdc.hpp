#pragma once

#include <cstdint>
#include <uhal/Status.hpp>

namespace uhal {

class IAdc {
public:
    virtual ~IAdc() = default;

    virtual Status read_raw(std::uint16_t& sample) = 0;
};

}  // namespace uhal
