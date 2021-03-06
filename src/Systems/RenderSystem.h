#pragma once

#include <GLM/glm.hpp>

#include "System.h"
#include "../Resources/Program.h"
#include "../Resources/Texture.h"

static const glm::vec2 SCREEN_SIZE (800, 600);

class RenderSystem : public System {
public:
    RenderSystem(World* world);
    virtual void init();
    virtual void shutdown();
    virtual bool step(int pass, double delta);
    virtual void receive(const Event& event);
    virtual void build();
private:
    std::shared_ptr<Ymir::Program> _texture_program;
    std::shared_ptr<Ymir::Program> _overlay_program;
    std::shared_ptr<Ymir::Texture> _text_texture;
};