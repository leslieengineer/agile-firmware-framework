#pragma once

#include <cstddef>
#include <cstdint>
#include <uhal/Status.hpp>

namespace uhal {

class IStorage {
public:
    virtual ~IStorage() = default;

    virtual Status read(std::uint32_t offset, std::uint8_t* data, std::size_t size)        = 0;
    virtual Status write(std::uint32_t offset, const std::uint8_t* data, std::size_t size) = 0;
    virtual Status erase(std::uint32_t offset, std::size_t size)                           = 0;
};

}  // namespace uhal
