#include "RenderSystem.h"

#include "../Resources/resource_factory.h"

#include "../Components/SpatialComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/ModelComponent.h"

#include "render_frame.h"
#include "commands.h"

RenderSystem::RenderSystem(World* world) : System(world) {
    systems::registerSystem(this);
}

void RenderSystem::init() {
    _texture_program = resource_factory::instance().resource<Ymir::Program>("texture", "program");
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
    
    frame.execute();
    
    return true;
}

void RenderSystem::receive(const Event& event) {
}