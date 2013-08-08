#pragma once

#include "Component.h"
#include "Spatial.h"

struct SpatialHierarchyComponent : Component<SpatialHierarchyComponent> {
    Spatial local;
    EntityHandle owner;
};