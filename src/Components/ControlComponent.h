#pragma once

#include "Component.h"
#include <GLM/glm.hpp>

struct ControlComponent : public Component<ControlComponent> {
    enum class ControlType {
        FREECAM, //For space shooters, 7 degrees of freedom
    };
    
    ControlType control_type;
    glm::vec3 movement;
    int roll;
};