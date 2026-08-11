#pragma once

#include <cstddef>
#include <cstdint>

#include <uhal/Status.hpp>

namespace uhal {

class ISpi {
public:
    virtual ~ISpi() = default;

    virtual Status transfer(const std::uint8_t* transmit_data,
                            std::uint8_t* receive_data,
                            std::size_t size) = 0;
};

} // namespace uhal
