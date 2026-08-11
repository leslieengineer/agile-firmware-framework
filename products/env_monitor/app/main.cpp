#include <cstdint>

#include <devices/Sht3x.hpp>
#include <protocols/ModbusRtuMaster.hpp>
#include <uhal/fake/FakeI2c.hpp>
#include <uhal/fake/FakeUart.hpp>

int main() {
    uhal::fake::FakeI2c sensor_bus;
    uhal::fake::FakeUart modbus_uart;

    devices::Sht3x environment_sensor{sensor_bus};
    protocols::ModbusRtuMaster modbus{modbus_uart};

    constexpr std::uint8_t request[] = {0x03, 0x00, 0x00, 0x00, 0x01};
    const auto sensor_status = environment_sensor.start_single_shot_measurement();
    const auto modbus_status = modbus.send_request(0x01, request, sizeof(request));

    return sensor_status == uhal::Status::ok && modbus_status == uhal::Status::ok ? 0 : 1;
}
