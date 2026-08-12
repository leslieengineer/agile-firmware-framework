#include <cstdint>
#include <cstring>
#include <devices/Sht3x.hpp>
#include <libraries/EventBus.hpp>
#include <libraries/StateMachine.hpp>
#include <protocols/FrameCodec.hpp>
#include <protocols/ModbusRtuMaster.hpp>
#include <services/EnvironmentMonitor.hpp>
#include <uhal/fake/FakeClock.hpp>
#include <uhal/fake/FakeGpio.hpp>
#include <uhal/fake/FakeI2c.hpp>
#include <uhal/fake/FakeUart.hpp>

namespace {

enum class TestTopic : std::uint8_t { temperature };
enum class TestState : std::uint8_t { idle, running };
enum class TestEvent : std::uint8_t { start };

struct TestContext {
    bool action_called = false;
};

void mark_started(TestContext& context) {
    context.action_called = true;
}

bool verify_reusable_primitives() {
    const auto result = uhal::Result<std::uint16_t>::success(42U);
    if (!result.is_ok() || result.value() != 42U) {
        return false;
    }

    libraries::EventBus<TestTopic, std::int16_t, 1, 2> bus;
    libraries::EventQueue<std::int16_t, 1>             display_queue;
    if (!bus.subscribe(TestTopic::temperature, display_queue) ||
        !bus.publish(TestTopic::temperature, 27)) {
        return false;
    }
    std::int16_t temperature = 0;
    if (!display_queue.try_pop(temperature) || temperature != 27) {
        return false;
    }

    constexpr std::array<libraries::StateMachine<TestState, TestEvent, TestContext, 1>::Transition,
                         1>
        transitions{{{TestState::idle, TestEvent::start, TestState::running, mark_started}}};
    TestContext                                                   context{};
    libraries::StateMachine<TestState, TestEvent, TestContext, 1> machine{TestState::idle,
                                                                          transitions};
    if (!machine.process(TestEvent::start, context) || !context.action_called ||
        machine.state() != TestState::running) {
        return false;
    }

    using Codec = protocols::FrameCodec<8>;
    Codec::Frame frame{};
    frame.opcode       = 0x42;
    frame.payload[0]   = 0x12;
    frame.payload[1]   = 0x34;
    frame.payload_size = 2;
    std::uint8_t encoded[Codec::kMaximumFrame]{};
    const auto   encoded_size = Codec::encode(frame, encoded, sizeof(encoded));
    if (!encoded_size.is_ok()) {
        return false;
    }
    const auto decoded = Codec::decode(encoded, encoded_size.value());
    if (!decoded.is_ok() || decoded.value().opcode != frame.opcode ||
        decoded.value().payload_size != frame.payload_size || decoded.value().payload[1] != 0x34) {
        return false;
    }
    encoded[4] ^= 0x01U;
    return Codec::decode(encoded, encoded_size.value()).status() == uhal::Status::io_error;
}

bool verify_environment_monitor() {
    uhal::fake::FakeUart         uart;
    uhal::fake::FakeGpio         led;
    uhal::fake::FakeClock        clock;
    services::EnvironmentMonitor monitor{uart, led, clock, 0x12345678U};

    monitor.run_once();
    constexpr char expected_telemetry[] = "Temperature: 23 C";
    if (uart.last_write_size() < sizeof(expected_telemetry) - 1U ||
        std::strncmp(uart.last_write(), expected_telemetry, sizeof(expected_telemetry) - 1U) != 0) {
        return false;
    }
    clock.advance_ms(1000U);
    monitor.run_once();

    uhal::GpioLevel level = uhal::GpioLevel::low;
    return led.get(level) == uhal::Status::ok &&
           (level == uhal::GpioLevel::low || level == uhal::GpioLevel::high);
}

}  // namespace

int main() {
    if (!verify_reusable_primitives()) {
        return 1;
    }
    if (!verify_environment_monitor()) {
        return 1;
    }

    uhal::fake::FakeI2c i2c;
    devices::Sht3x      sensor{i2c};
    if (sensor.start_single_shot_measurement() != uhal::Status::ok) {
        return 1;
    }

    uhal::fake::FakeUart       uart;
    protocols::ModbusRtuMaster modbus{uart};
    constexpr std::uint8_t     pdu[] = {0x03, 0x00, 0x00, 0x00, 0x01};
    if (modbus.send_request(0x01, pdu, sizeof(pdu)) != uhal::Status::ok) {
        return 1;
    }

    return modbus.send_request(0x01, nullptr, 0) == uhal::Status::invalid_argument ? 0 : 1;
}
