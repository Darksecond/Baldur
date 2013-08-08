#pragma once

#include "Component.h"
#include "text.h"

struct HUDTextComponent : public Component<HUDTextComponent> {
    text hud_text;
};