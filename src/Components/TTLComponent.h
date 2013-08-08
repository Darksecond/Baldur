#pragma once

#include "Component.h"

struct TTLComponent : Component<TTLComponent> {
    double ttl;
};