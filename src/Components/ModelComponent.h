#pragma once

#include "Component.h"

#include "../Resources/Mesh.h"
#include "../Resources/material.h"

struct ModelComponent : public Component<ModelComponent> {
    std::shared_ptr<Ymir::Mesh> mesh;
    std::shared_ptr<material> material;
};