#pragma once

#include "../Component.h"
#include <GLM/glm.hpp>

struct RigidBodyComponent : Component<RigidBodyComponent> {
    glm::vec3 velocity;
    double mass;
};