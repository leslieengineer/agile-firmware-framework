#pragma once

#include <array>
#include <cstring>
#include <uhal/IUart.hpp>

namespace uhal::fake {

class FakeUart final : public IUart {
public:
    static constexpr std::size_t kCaptureCapacity = 96U;

    Status write(const std::uint8_t* data, std::size_t size) override {
        if (data == nullptr && size != 0U) {
            return Status::invalid_argument;
        }
        if (size >= kCaptureCapacity) {
            return Status::invalid_argument;
        }

        std::memcpy(last_write_.data(), data, size);
        last_write_[size] = '\0';
        last_write_size_  = size;
        return Status::ok;
    }

    Status read(std::uint8_t*, std::size_t, std::size_t& received, std::uint32_t) override {
        received = 0;
        return Status::timeout;
    }

    const char* last_write() const {
        return reinterpret_cast<const char*>(last_write_.data());
    }

    std::size_t last_write_size() const {
        return last_write_size_;
    }

private:
    std::array<std::uint8_t, kCaptureCapacity> last_write_{};
    std::size_t                                last_write_size_ = 0U;
};

}  // namespace uhal::fake
