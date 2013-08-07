#pragma once

#include "Component.h"
#include "Spatial.h"

struct SpatialComponent : public Component<SpatialComponent> {
    Spatial spatial;
};