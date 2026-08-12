#pragma once

#include <uhal/Status.hpp>

namespace uhal {

class IWatchdog {
public:
    virtual ~IWatchdog() = default;

    virtual Status feed() = 0;
};

}  // namespace uhal
