#pragma once

#include <uhal/IClock.hpp>
#include <uhal/IGpio.hpp>
#include <uhal/IUart.hpp>

class Stm32L4Clock final : public uhal::IClock {
public:
    std::uint32_t now_ms() const override;
    void          sleep_ms(std::uint32_t duration_ms) override;
};

class Stm32L4Gpio final : public uhal::IGpio {
public:
    uhal::Status set(uhal::GpioLevel level) override;
    uhal::Status get(uhal::GpioLevel& level) override;
};

class Stm32L4Uart final : public uhal::IUart {
public:
    uhal::Status write(const std::uint8_t* data, std::size_t size) override;
    uhal::Status read(std::uint8_t* data, std::size_t capacity, std::size_t& received,
                      std::uint32_t timeout_ms) override;
};