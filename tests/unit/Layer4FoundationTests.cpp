#include <libraries/Backoff.hpp>
#include <libraries/ByteReader.hpp>
#include <libraries/ByteWriter.hpp>
#include <libraries/Crc32.hpp>
#include <libraries/Deadline.hpp>
#include <libraries/FixedRingBuffer.hpp>
#include <libraries/JsonScanner.hpp>
#include <libraries/JsonWriter.hpp>
#include <services/Limits.hpp>
#include <uhal/Status.hpp>

namespace {

bool ring_buffer_works() {
    libraries::FixedRingBuffer<int, 2U> buffer;
    int                                 value = 0;
    return buffer.try_push(10) && buffer.try_push(20) && !buffer.try_push(30) &&
           buffer.peek(value) && value == 10 && buffer.drop_oldest() && buffer.try_pop(value) &&
           value == 20 && buffer.empty();
}

bool retry_and_deadline_work() {
    libraries::BackoffPolicy backoff;
    const bool schedule = backoff.next_delay_ms() == 30000U && backoff.next_delay_ms() == 60000U &&
                          backoff.next_delay_ms() == 120000U &&
                          backoff.next_delay_ms() == 240000U &&
                          backoff.next_delay_ms() == 300000U && backoff.next_delay_ms() == 300000U;
    const libraries::Deadline deadline{0xFFFFFFF0U, 32U};
    return schedule && !deadline.expired(0x0000000FU) && deadline.expired(0x00000010U);
}

bool byte_serialization_works() {
    std::uint8_t          buffer[8]{};
    libraries::ByteWriter writer{buffer, sizeof(buffer)};
    if (!writer.write_u16_le(0x1234U) || !writer.write_u32_le(0x89ABCDEFU)) return false;
    libraries::ByteReader reader{buffer, writer.size()};
    std::uint16_t         value16 = 0U;
    std::uint32_t         value32 = 0U;
    return reader.read_u16_le(value16) && reader.read_u32_le(value32) && value16 == 0x1234U &&
           value32 == 0x89ABCDEFU;
}

bool json_serialization_works() {
    std::uint8_t          buffer[64]{};
    libraries::JsonWriter writer{buffer, sizeof(buffer)};
    if (!writer.begin_object() || !writer.field_bool("on", true) || !writer.field_u32("seq", 42U) ||
        !writer.field_string("id", "node-1") || !writer.end_object())
        return false;
    libraries::JsonScanner scanner{buffer, writer.size()};
    bool                   on       = false;
    std::uint32_t          sequence = 0U;
    char                   id[8]{};
    return scanner.get_bool("on", on) && on && scanner.get_u32("seq", sequence) &&
           sequence == 42U && scanner.get_string("id", id, sizeof(id)) && id[0] == 'n' &&
           id[5] == '1';
}

bool crc_and_status_work() {
    constexpr std::uint8_t input[] = {1U, 2U, 3U, 4U};
    return libraries::crc32(input, sizeof(input)) == 0xB63CFBCDU &&
           uhal::Status::not_ready != uhal::Status::ok && services::kPayloadCapacity == 256U;
}

}  // namespace

int main() {
    return ring_buffer_works() && retry_and_deadline_work() && byte_serialization_works() &&
                   json_serialization_works() && crc_and_status_work()
               ? 0
               : 1;
}
