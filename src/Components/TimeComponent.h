#pragma once

#include "Component.h"

struct TimeComponent : public Component<TimeComponent> {
    int time;
};