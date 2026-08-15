#include <services/DiagnosticsService.hpp>
#include <services/HealthMonitor.hpp>
#include <services/TimeSyncService.hpp>
namespace {
class TimeSource final : public services::ITimeSource {
public:
    uhal::Status request_sync() override {
        requested = true;
        return uhal::Status::ok;
    }
    void poll(std::uint32_t) override {}
    bool has_result() const override {
        return ready;
    }
    uhal::Status take_utc(std::uint32_t& v) override {
        v     = 1700000000U;
        ready = false;
        return uhal::Status::ok;
    }
    bool requested = false;
    bool ready     = false;
};
class SystemTime final : public services::ISystemTime {
public:
    uhal::Status set_utc(std::uint32_t v) override {
        utc = v;
        return uhal::Status::ok;
    }
    std::uint32_t utc_now() const override {
        return utc;
    }
    std::uint32_t utc = 0U;
};
class Watchdog final : public uhal::IWatchdog {
public:
    uhal::Status feed() override {
        ++feeds;
        return uhal::Status::ok;
    }
    int feeds = 0;
};
class Recovery final : public services::IRecoveryAction {
public:
    uhal::Status request_reboot(std::uint16_t) override {
        ++reboots;
        return uhal::Status::ok;
    }
    int reboots = 0;
};
bool time() {
    TimeSource                source;
    SystemTime                system;
    services::TimeSyncService service{source, system};
    service.on_connectivity_up(0U);
    service.poll(0U);
    if (!source.requested) return false;
    source.ready = true;
    service.poll(1U);
    return service.quality() == services::TimeQuality::synced && service.utc_now() == 1700000000U;
}
bool diagnostics() {
    services::DiagnosticsService d;
    d.increment(1U, 2U);
    d.set_gauge(1U, -5);
    for (int i = 0; i < 10; ++i) d.record_fault(static_cast<std::uint16_t>(i), 1U, 0U);
    return d.counter(1U) == 2U && d.gauge(1U) == -5 &&
           d.fault_count() == services::kFaultRingCapacity;
}
bool health() {
    Watchdog                watchdog;
    Recovery                recovery;
    services::HealthMonitor health{watchdog, recovery};
    health.add_source("control", 100U, true);
    health.check_in(0U, 50U);
    if (health.poll(100U) != uhal::Status::ok || watchdog.feeds != 1) return false;
    return health.poll(700000U) == uhal::Status::not_ready && recovery.reboots == 1;
}
}  // namespace
int main() {
    return time() && diagnostics() && health() ? 0 : 1;
}
