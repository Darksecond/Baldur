#pragma once

#include "Component.h"

/**
 * MouseComponent keeps the current mouse position, this can be absolute or relative.
 * We don't send messages, because that would mean sending a message every frame.
 */
struct MouseComponent : public Component<MouseComponent> {
    
    /**
     * This tells the MouseSystem if our coordinates should be absolute or relative.
     * True means values are relative, false means absolute values.
     */
    bool delta;
    
    /**
     * X value, either absolute or relative, depending on delta.
     */
    int x;
    
    /**
     * Y value, either absolute or relative, depending on delta.
     */
    int y;
};