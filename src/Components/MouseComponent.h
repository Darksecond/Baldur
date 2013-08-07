#pragma once

#include "Component.h"

/**
 * MouseComponent keeps the current mouse position, this is relative.
 * We don't send messages, because that would mean sending a message every frame.
 */
struct MouseComponent : public Component<MouseComponent> {
    int x;
    int y;
};