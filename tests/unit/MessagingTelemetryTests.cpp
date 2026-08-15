#include <protocols/MqttSessionPolicy.hpp>
#include <services/OfflineQueue.hpp>
#include <services/TelemetryService.hpp>
namespace {
class Transport final : public services::IMessageTransport {
public:
    uhal::Status connect(const services::SessionDescriptor&) override {
        return uhal::Status::ok;
    }
    uhal::Status disconnect() override {
        return uhal::Status::ok;
    }
    uhal::Status publish(const services::MessageEnvelope& m) override {
        ++published;
        last_size = m.payload.size;
        return result;
    }
    uhal::Status subscribe(const char*, services::QosLevel) override {
        return uhal::Status::ok;
    }
    void poll(std::uint32_t) override {}
    bool is_connected() const override {
        return connected;
    }
    void         set_handler(services::IMessageHandler&) override {}
    bool         connected = false;
    int          published = 0;
    std::size_t  last_size = 0U;
    uhal::Status result    = uhal::Status::ok;
};
bool topics() {
    protocols::MqttTopics topics{"site", "node"};
    services::TopicBuffer out{};
    return topics.build(out, protocols::MqttTopicKind::state, "relay") == uhal::Status::ok &&
           out.value[0] == 'a';
}
bool offline_replay() {
    Transport                  transport;
    services::OfflineQueue     queue;
    protocols::MqttTopics      topics{"site", "node"};
    services::TelemetryService telemetry{transport, queue, topics};
    if (telemetry.publish_bool("relay", true, 0U) != uhal::Status::ok || queue.size() != 1U)
        return false;
    transport.connected = true;
    return telemetry.replay_one() == uhal::Status::ok && queue.size() == 0U &&
           transport.published == 1 && transport.last_size > 0U;
}
bool capacity() {
    services::OfflineQueue    queue;
    std::uint8_t              payload[1] = {1U};
    services::MessageEnvelope message{};
    message.payload = {payload, 1U};
    for (std::size_t i = 0U; i < services::kOfflineQueueDepth; ++i)
        if (queue.enqueue(message, false) != uhal::Status::ok) return false;
    return queue.enqueue(message, false) == uhal::Status::no_resources &&
           queue.enqueue(message, true) == uhal::Status::ok && queue.dropped_count() == 1U;
}
bool session_policy() {
    protocols::MqttSessionPolicy policy;
    services::SessionDescriptor  wifi{};
    services::SessionDescriptor  cellular{};
    return policy.build(wifi, "node", false, true) == uhal::Status::ok &&
           policy.build(cellular, "node", true, false) == uhal::Status::ok &&
           wifi.keepalive_s == 60U && wifi.clean_session && cellular.keepalive_s == 120U &&
           !cellular.clean_session;
}
}  // namespace
int main() {
    return topics() && offline_replay() && capacity() && session_policy() ? 0 : 1;
}
