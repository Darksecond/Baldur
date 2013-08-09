#include "TimeLeftSystem.h"

#include <sstream>

#include "../Components/HUDTextComponent.h"
#include "../Components/TimeComponent.h"

TimeLeftSystem::TimeLeftSystem(World* world) : System(world) {
    systems::registerSystem(this);
    
    total_time = 0;
    _text_entity = nullptr;
}

void TimeLeftSystem::init() {
}

void TimeLeftSystem::shutdown() {
    if(_text_entity) {
        world()->destroyEntity(_text_entity);
    }
}

bool TimeLeftSystem::step(int pass, double delta) {
    if(pass != 8) return true;
    
    for(TimeComponent* time : world()->components<TimeComponent>()) {
        if(_text_entity == nullptr) {
            _text_entity = world()->createEntity("TimeLeftSystem Text");
            HUDTextComponent* text = world()->createComponent<HUDTextComponent>(_text_entity);
            text->hud_text.set_size(25);
            text->hud_text.set_text("Time left: 0:0");
            text->hud_text.set_position(glm::vec2(0,600-25));
        }
        total_time += time->time;
        world()->destroyComponent(time->parent, time);
    }
    
    total_time -= delta;
    
    if(total_time <= 0) {
        total_time = 0;
        events::sendEvent(Event{.type = EventType::NO_TIME_LEFT});
    }
    
    int minutes = (int)total_time / 60;
    int seconds = (int)total_time % 60;
    auto text = world()->component<HUDTextComponent>(_text_entity); //we can be sure it only has one text component because we control this entity
    
    std::stringstream time_left_text;
    time_left_text << "Time left: " << minutes << ":" << seconds;
    if(_text_entity != nullptr)
        text->hud_text.set_text(time_left_text.str());
    
    return true;
}

void TimeLeftSystem::receive(const Event& event) {
}

void TimeLeftSystem::build() {
}