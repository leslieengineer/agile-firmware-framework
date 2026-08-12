#pragma once

#include <uhal/IGpio.hpp>

namespace uhal::fake {

class FakeGpio final : public IGpio {
public:
    Status set(GpioLevel level) override {
        level_ = level;
        return Status::ok;
    }

    Status get(GpioLevel& level) override {
        level = level_;
        return Status::ok;
    }

private:
    GpioLevel level_ = GpioLevel::low;
};

}  // namespace uhal::fake
