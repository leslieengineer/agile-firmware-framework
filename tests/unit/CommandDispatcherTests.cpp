#include <services/CommandDispatcher.hpp>
#include <services/OfflineQueue.hpp>
namespace {
class Transport final : public services::IMessageTransport {
public:
    uhal::Status connect(const services::SessionDescriptor&) override {
        return uhal::Status::ok;
    }
    uhal::Status disconnect() override {
        return uhal::Status::ok;
    }
    uhal::Status publish(const services::MessageEnvelope&) override {
        ++published;
        return uhal::Status::ok;
    }
    uhal::Status subscribe(const char*, services::QosLevel) override {
        return uhal::Status::ok;
    }
    void poll(std::uint32_t) override {}
    bool is_connected() const override {
        return connected;
    }
    void set_handler(services::IMessageHandler&) override {}
    bool connected = true;
    int  published = 0;
};
class Authorizer final : public services::ICommandAuthorizer {
public:
    uhal::Status authorize(const services::CommandRequest&) const override {
        return result;
    }
    uhal::Status result = uhal::Status::ok;
};
class Handler final : public services::ICommandHandler {
public:
    const char* name() const override {
        return "switch.set";
    }
    uhal::Status handle(const services::CommandRequest& r,
                        services::CommandResponse&      out) override {
        ++calls;
        out.state = r.value;
        return uhal::Status::ok;
    }
    int calls = 0;
};
bool dispatch_and_idempotency() {
    Transport                   transport;
    services::OfflineQueue      queue;
    Authorizer                  auth;
    Handler                     handler;
    protocols::MqttTopics       topics{"site", "node"};
    services::CommandDispatcher dispatcher{transport, queue, auth, topics};
    dispatcher.add_handler(handler);
    constexpr char            json[] = "{\"id\":\"1\",\"name\":\"switch.set\",\"value\":true}";
    services::MessageEnvelope message{};
    message.payload = {reinterpret_cast<const std::uint8_t*>(json), sizeof(json) - 1U};
    dispatcher.on_message(message);
    dispatcher.on_message(message);
    return handler.calls == 1 && transport.published == 2;
}
bool denied() {
    Transport              transport;
    services::OfflineQueue queue;
    Authorizer             auth;
    auth.result = uhal::Status::denied;
    Handler                     handler;
    protocols::MqttTopics       topics{"site", "node"};
    services::CommandDispatcher dispatcher{transport, queue, auth, topics};
    dispatcher.add_handler(handler);
    constexpr char            json[] = "{\"id\":\"2\",\"name\":\"switch.set\"}";
    services::MessageEnvelope message{};
    message.payload = {reinterpret_cast<const std::uint8_t*>(json), sizeof(json) - 1U};
    dispatcher.on_message(message);
    return handler.calls == 0 && transport.published == 1;
}
}  // namespace
int main() {
    return dispatch_and_idempotency() && denied() ? 0 : 1;
}
