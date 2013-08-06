#include "Event.h"

#include <list>
#include <queue>

static std::queue<Event> _events;
static std::list<std::pair<EventType, Sink*>> _sinks;

void events::registerSink(Sink* sink, EventType type_filter) {
    _sinks.push_back(std::make_pair(type_filter, sink));
}

void runEvent(const Event& e) {
    for(auto pair : _sinks) {
        if(pair.first == e.type || pair.first == EventType::ALL)
            pair.second->receive(e);
    }
}

void events::sendEvent(const Event& e, bool now) {
    if(now) {
        runEvent(e);
    } else {
    _events.push(e);
    }
}

void events::runEvents() {
    while(!_events.empty()) {
        const Event& event = _events.front();
        runEvent(event);
        _events.pop();
    }
}