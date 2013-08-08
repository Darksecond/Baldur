#include "RenderSystem.h"

#include "../Resources/resource_factory.h"

#include "../Components/SpatialComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/ModelComponent.h"
#include "../Components/HUDTextComponent.h"

#include "render_frame.h"
#include "commands.h"

RenderSystem::RenderSystem(World* world) : System(world) {
    systems::registerSystem(this);
}

void RenderSystem::init() {
    _texture_program = resource_factory::instance().resource<Ymir::Program>("texture", "program");
    _overlay_program = resource_factory::instance().resource<Ymir::Program>("overlay", "program");
    _text_texture = resource_factory::instance().resource<Ymir::Texture>("Holstein.tga", "texture");
}

void RenderSystem::shutdown() {
}

bool RenderSystem::step(int pass, double delta) {
    if(pass != 16) return true;
    
    render_frame frame;
    
    frame.add<0, render_commands::bind_program>(_texture_program);
    frame.add<0, render_commands::clear>(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    frame.add<0, render_commands::clear_color>(0, 0, 0, 1);
    frame.add<0, render_commands::set_culling>(true);
    frame.add<0, render_commands::set_depth>(true, true);
    
    frame.add<2, render_commands::bind_program>(_overlay_program);
    frame.add<2, render_commands::bind_texture>(_text_texture, GL_TEXTURE0, "myTextureSampler");
    frame.add<2, render_commands::set_blend>(true, GL_FUNC_ADD, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    frame.add<2, render_commands::set_depth>(false, false);
    
    for(Entity* entity : world()->entities()) {
        SpatialComponent* spatial = world()->component<SpatialComponent>(entity);
        CameraComponent* camera = world()->component<CameraComponent>(entity);
        ModelComponent* model = world()->component<ModelComponent>(entity);
        
        if(spatial && camera) { //camera
            glm::mat4 projection_matrix = glm::perspective(camera->FoV,
                                                 SCREEN_SIZE.x / SCREEN_SIZE.y,
                                                 0.1f,
                                                 100.0f);
            
            glm::mat4 view_matrix = glm::inverse(spatial->spatial.matrix());
            
            frame.add<0, render_commands::set_uniform<glm::mat4>>("projection", projection_matrix);
            frame.add<0, render_commands::set_uniform<glm::mat4>>("view", view_matrix);
            
        } else if(spatial && model) { //model
            frame.add<1, render_commands::draw_model>(model->mesh, model->material, spatial->spatial.matrix());
            
        }
    }
    
    for(HUDTextComponent* text : world()->components<HUDTextComponent>()) {
        frame.add<2, render_commands::draw_text>(&text->hud_text);
    }
    
    frame.add<2, render_commands::set_blend>(false);
    frame.add<2, render_commands::set_depth>(true, true);
    
    frame.execute();
    
    return true;
}

void RenderSystem::receive(const Event& event) {
}