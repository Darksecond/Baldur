#pragma once

#include "../Component.h"
#include "../Spatial.h"

struct SphereGeomComponent : public Component<SphereGeomComponent> {
    Spatial offset;
    double radius;
};