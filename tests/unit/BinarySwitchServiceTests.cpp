#include <services/BinarySwitchService.hpp>
#include <uhal/fake/FakeGpio.hpp>

namespace {

class StubStore final : public services::IBinaryStateStore {
public:
    uhal::Status load(services::BinaryState& state) override {
        if (load_status == uhal::Status::ok) state = stored;
        return load_status;
    }

    uhal::Status save(const services::BinaryState& state) override {
        ++save_count;
        stored = state;
        return save_status;
    }

    services::BinaryState stored{};
    uhal::Status          load_status = uhal::Status::ok;
    uhal::Status          save_status = uhal::Status::ok;
    int                   save_count  = 0;
};

class ConfigurableGpio final : public uhal::IGpio {
public:
    uhal::Status set(uhal::GpioLevel level) override {
        ++set_count;
        level_ = level;
        return set_status;
    }

    uhal::Status get(uhal::GpioLevel& level) override {
        level = level_;
        return uhal::Status::ok;
    }

    uhal::Status set_status = uhal::Status::ok;
    int          set_count  = 0;

private:
    uhal::GpioLevel level_ = uhal::GpioLevel::low;
};

bool level_is(uhal::IGpio& gpio, uhal::GpioLevel expected) {
    uhal::GpioLevel actual{};
    return gpio.get(actual) == uhal::Status::ok && actual == expected;
}

bool restore_applies_stored_on() {
    uhal::fake::FakeGpio relay, indicator;
    StubStore            store;
    store.stored.on = true;
    services::BinarySwitchService service{relay, indicator, store};

    return service.restore() == uhal::Status::ok && service.is_on() &&
           level_is(relay, uhal::GpioLevel::high) && level_is(indicator, uhal::GpioLevel::high);
}

bool restore_load_failure_defaults_off_and_applies() {
    uhal::fake::FakeGpio relay, indicator;
    StubStore            store;
    store.load_status = uhal::Status::io_error;
    services::BinarySwitchService service{relay, indicator, store};

    return service.restore() == uhal::Status::ok && !service.is_on() &&
           level_is(relay, uhal::GpioLevel::low) && level_is(indicator, uhal::GpioLevel::low);
}

bool restore_apply_failure_returns_io_error() {
    ConfigurableGpio     relay;
    uhal::fake::FakeGpio indicator;
    StubStore            store;
    relay.set_status = uhal::Status::io_error;
    services::BinarySwitchService service{relay, indicator, store};

    return service.restore() == uhal::Status::io_error && store.save_count == 0;
}

bool set_true_then_false_persists() {
    uhal::fake::FakeGpio          relay, indicator;
    StubStore                     store;
    services::BinarySwitchService service{relay, indicator, store};

    return service.set(true) == uhal::Status::ok && service.set(false) == uhal::Status::ok &&
           !service.is_on() && store.save_count == 2 && !store.stored.on &&
           level_is(relay, uhal::GpioLevel::low) && level_is(indicator, uhal::GpioLevel::low);
}

bool set_idempotent_still_applies_and_saves() {
    ConfigurableGpio              relay, indicator;
    StubStore                     store;
    services::BinarySwitchService service{relay, indicator, store};

    return service.set(false) == uhal::Status::ok && relay.set_count == 1 &&
           indicator.set_count == 1 && store.save_count == 1;
}

bool toggle_flips_applies_and_saves() {
    uhal::fake::FakeGpio          relay, indicator;
    StubStore                     store;
    services::BinarySwitchService service{relay, indicator, store};

    return service.toggle() == uhal::Status::ok && service.is_on() && store.save_count == 1 &&
           store.stored.on && level_is(relay, uhal::GpioLevel::high) &&
           level_is(indicator, uhal::GpioLevel::high);
}

bool toggle_save_failure_keeps_applied_state() {
    uhal::fake::FakeGpio relay, indicator;
    StubStore            store;
    store.save_status = uhal::Status::io_error;
    services::BinarySwitchService service{relay, indicator, store};

    return service.toggle() == uhal::Status::io_error && service.is_on() && store.save_count == 1 &&
           level_is(relay, uhal::GpioLevel::high) && level_is(indicator, uhal::GpioLevel::high);
}

bool apply_failure_skips_save() {
    ConfigurableGpio     relay;
    uhal::fake::FakeGpio indicator;
    StubStore            store;
    relay.set_status = uhal::Status::io_error;
    services::BinarySwitchService service{relay, indicator, store};

    return service.toggle() == uhal::Status::io_error && service.is_on() && store.save_count == 0;
}

bool both_outputs_are_attempted() {
    ConfigurableGpio relay, indicator;
    StubStore        store;
    relay.set_status     = uhal::Status::io_error;
    indicator.set_status = uhal::Status::io_error;
    services::BinarySwitchService service{relay, indicator, store};

    return service.toggle() == uhal::Status::io_error && relay.set_count == 1 &&
           indicator.set_count == 1 && store.save_count == 0;
}

}  // namespace

int main() {
    return restore_applies_stored_on() && restore_load_failure_defaults_off_and_applies() &&
                   restore_apply_failure_returns_io_error() && set_true_then_false_persists() &&
                   set_idempotent_still_applies_and_saves() && toggle_flips_applies_and_saves() &&
                   toggle_save_failure_keeps_applied_state() && apply_failure_skips_save() &&
                   both_outputs_are_attempted()
               ? 0
               : 1;
}
