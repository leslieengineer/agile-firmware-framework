#pragma once

#include <uhal/IClock.hpp>
#include <uhal/IGpio.hpp>
#include <uhal/IUart.hpp>

namespace stm32l4::nucleo_l476rg::adapters {

class Clock final : public uhal::IClock {
public:
    std::uint32_t now_ms() const override;
    void          sleep_ms(std::uint32_t duration_ms) override;
};

class Ld2 final : public uhal::IGpio {
public:
    uhal::Status set(uhal::GpioLevel level) override;
    uhal::Status get(uhal::GpioLevel& level) override;
};

class StLinkVirtualCom final : public uhal::IUart {
public:
    uhal::Status write(const std::uint8_t* data, std::size_t size) override;
    uhal::Status read(std::uint8_t* data, std::size_t capacity, std::size_t& received,
                      std::uint32_t timeout_ms) override;
};

}  // namespace stm32l4::nucleo_l476rg::adapters
