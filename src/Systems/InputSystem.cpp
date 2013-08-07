#include "InputSystem.h"

#include <GLEW/glew.h>
#include <GLFW/glfw.h>
#include <iostream>

#include "MouseComponent.h"

InputSystem::InputSystem(World* world) : System(world) {
    systems::registerSystem(this);
}

void InputSystem::init() {
    glfwGetMousePos(&mouse_x, &mouse_y);
    //glfwDisable(GLFW_MOUSE_CURSOR);
}

void InputSystem::shutdown() {
}

bool InputSystem::step(int pass, double delta) {
    if(pass != 0) return true;
    
    //KEYBOARD
    if(glfwGetKey('W') == GLFW_PRESS) events::sendEvent(Event{.type = EventType::KEY_PRESSED, .key = 'W'});
    if(glfwGetKey('S') == GLFW_PRESS) events::sendEvent(Event{.type = EventType::KEY_PRESSED, .key = 'S'});
    if(glfwGetKey('A') == GLFW_PRESS) events::sendEvent(Event{.type = EventType::KEY_PRESSED, .key = 'A'});
    if(glfwGetKey('D') == GLFW_PRESS) events::sendEvent(Event{.type = EventType::KEY_PRESSED, .key = 'D'});
    
    if(glfwGetKey(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) events::sendEvent(Event{.type = EventType::KEY_PRESSED, .key = GLFW_MOUSE_BUTTON_LEFT});
    //END KEYBOARD
    
    //MOUSE
    int new_mouse_x, new_mouse_y;
    glfwGetMousePos(&new_mouse_x, &new_mouse_y);
    int delta_x, delta_y;
    delta_x = new_mouse_x - mouse_x;
    delta_y = new_mouse_y - mouse_y;
    mouse_x = new_mouse_x;
    mouse_y = new_mouse_y;
    
    auto mouse_components = world()->components<MouseComponent>();
    for(auto mouse_component : mouse_components) {
        if(delta) {
            mouse_component->x = delta_x;
            mouse_component->y = delta_y;
        } else {
            mouse_component->x = mouse_x;
            mouse_component->y = mouse_y;
        }
    }
    //END MOUSE
    
    //ESC KEY DETECTION
    if(glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS) return false;
    else return true;
    //END ESC KEY DETECTION
}

void InputSystem::receive(const Event& event) {
}
