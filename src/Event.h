#pragma once

class Entity;

enum class EventType {
    ALL,
    ENTITY_CREATED,
    ENTITY_DESTROYED,
    COMPONENT_CREATED,
    COMPONENT_DESTROYED,
};

struct Event {
    EventType type;
    union {
        Entity* entity;
    };
};

class Sink {
public:
    virtual void receive(const Event& event) = 0;
};

namespace events {
    void registerSink(Sink* sink, EventType type_filter = EventType::ALL);
    void sendEvent(const Event& e, bool now = false);
    void runEvents();
}

