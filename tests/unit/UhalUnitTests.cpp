#include <cstdint>
#include <devices/Sht3x.hpp>
#include <protocols/ModbusRtuMaster.hpp>
#include <uhal/fake/FakeI2c.hpp>
#include <uhal/fake/FakeUart.hpp>

int main() {
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
