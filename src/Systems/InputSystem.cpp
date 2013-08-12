#include "InputSystem.h"

#include <GLEW/glew.h>
#include <GLFW/glfw.h>
#include <iostream>

#include "MouseComponent.h"

InputSystem::InputSystem(World* world) : System(world) {
    systems::registerSystem(this);
}

void InputSystem::init() {
    glfwDisable(GLFW_MOUSE_CURSOR);
}

void InputSystem::shutdown() {
    glfwEnable(GLFW_MOUSE_CURSOR);
}

bool InputSystem::step(int pass, double delta) {
    if(pass != 0) return true;
    
    //KEYBOARD
    if(glfwGetKey('W') == GLFW_PRESS) events::sendEvent(Event{.type = EventType::KEY_PRESSED, .key = 'W'});
    if(glfwGetKey('S') == GLFW_PRESS) events::sendEvent(Event{.type = EventType::KEY_PRESSED, .key = 'S'});
    if(glfwGetKey('A') == GLFW_PRESS) events::sendEvent(Event{.type = EventType::KEY_PRESSED, .key = 'A'});
    if(glfwGetKey('D') == GLFW_PRESS) events::sendEvent(Event{.type = EventType::KEY_PRESSED, .key = 'D'});
    if(glfwGetKey('Z') == GLFW_PRESS) events::sendEvent(Event{.type = EventType::KEY_PRESSED, .key = 'Z'});
    if(glfwGetKey('X') == GLFW_PRESS) events::sendEvent(Event{.type = EventType::KEY_PRESSED, .key = 'X'});
    if(glfwGetKey('Q') == GLFW_PRESS) events::sendEvent(Event{.type = EventType::KEY_PRESSED, .key = 'Q'});
    if(glfwGetKey('E') == GLFW_PRESS) events::sendEvent(Event{.type = EventType::KEY_PRESSED, .key = 'E'});
    
    if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) events::sendEvent(Event{.type = EventType::KEY_PRESSED, .key = GLFW_MOUSE_BUTTON_LEFT});
    if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) events::sendEvent(Event{.type = EventType::KEY_PRESSED, .key = GLFW_MOUSE_BUTTON_RIGHT});
    //END KEYBOARD
    
    //MOUSE
    int mouse_x, mouse_y;
    glfwGetMousePos(&mouse_x, &mouse_y);
    
    auto mouse_components = world()->components<MouseComponent>();
    for(auto mouse_component : mouse_components) {
            mouse_component->x = mouse_x;
            mouse_component->y = mouse_y;
    }
    //END MOUSE
    
    //MOUSE RESET
    glfwSetMousePos(0, 0);
    //END MOUSE RESET
    
    //ESC KEY DETECTION
    if(glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS) return false;
    else return true;
    //END ESC KEY DETECTION
}

void InputSystem::receive(const Event& event) {
}

void InputSystem::build() {
}
