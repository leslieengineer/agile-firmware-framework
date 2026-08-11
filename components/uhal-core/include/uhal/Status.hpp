#pragma once

#include <cstdint>

namespace uhal {

enum class Status : std::uint8_t {
    ok,
    timeout,
    nack,
    bus_error,
    busy,
    io_error,
    unsupported,
    invalid_argument,
};

}  // namespace uhal
