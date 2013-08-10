#include "EntityParser.h"

#include <libconfig/libconfig.hh>
#include <string>

#include "../World.h"
#include "../Entity.h"
#include "../Resources/resource_factory.h"

EntityHandle EntityParser::parse(World* world, const char* identifier, const char* name) {
    std::string contents = resource_factory::instance().manifests().read(identifier)->string();
    
    libconfig::Config config;
    config.setAutoConvert(true);
    config.readString(contents);
    
    EntityHandle entity = world->createEntity(name);
    
    libconfig::Setting& components = config.lookup("components");
    for(int i = 0; i < components.getLength(); ++i) {
        libconfig::Setting& component = components[i];
        if(strcmp(component.getName(), "ttl") == 0) {
            parse_ttl(world, entity, component);
        } else if(strcmp(component.getName(), "spatial") == 0) {
            parse_spatial(world, entity, component);
        } else if(strcmp(component.getName(), "model") == 0) {
            parse_model(world, entity, component);
        } else if(strcmp(component.getName(), "camera") == 0) {
            parse_camera(world, entity, component);
        } else if(strcmp(component.getName(), "time") == 0) {
            parse_time(world, entity, component);
        } else if(strcmp(component.getName(), "control") == 0) {
            parse_control(world, entity, component);
        }
    }
    
    return entity;
}

/*
    ttl = 20;
 */
#include "../Components/TTLComponent.h"
void EntityParser::parse_ttl(World* world, EntityHandle entity, const libconfig::Setting& setting) {
    TTLComponent* ttl_component = world->createComponent<TTLComponent>(entity);
    ttl_component->ttl = setting;
}

/*
    spatial = {
        position = { x = 3; y = 0; z = 0; };
    };
*/
#include "../Components/SpatialComponent.h"
void EntityParser::parse_spatial(World* world, EntityHandle entity, const libconfig::Setting& setting) {
    SpatialComponent* component = world->createComponent<SpatialComponent>(entity);
    
    if(setting.exists("position.x") && setting.exists("position.y") && setting.exists("position.z")) {
        float x = setting["position"]["x"];
        float y = setting["position"]["y"];
        float z = setting["position"]["z"];
        component->spatial.translate(glm::vec3(x,y,z));
    }
}

/*
    model = {
        mesh = "car.obj";
        material = {
            diffuse = "wooden-crate.jpg";
        };
    };
 */
#include "../Components/ModelComponent.h"
#include "../Resources/resource_factory.h"
void EntityParser::parse_model(World* world, EntityHandle entity, const libconfig::Setting& setting) {
    ModelComponent* component = world->createComponent<ModelComponent>(entity);
    
    component->mesh = resource_factory::instance().resource<Ymir::Mesh>(setting["mesh"], "mesh");
    component->material = std::make_shared<material>(resource_factory::instance().resource<Ymir::Texture>(setting["material"]["diffuse"], "texture"));
}

/*
    camera = {
        FoV = 45;
    };
*/
#include "../Components/CameraComponent.h"
void EntityParser::parse_camera(World* world, EntityHandle entity, const libconfig::Setting& setting) {
    CameraComponent* component = world->createComponent<CameraComponent>(entity);
    
    component->FoV = setting["FoV"];
}

/*
    time = 263;
 */
#include "../Components/TimeComponent.h"
void EntityParser::parse_time(World* world, EntityHandle entity, const libconfig::Setting& setting) {
    TimeComponent* component = world->createComponent<TimeComponent>(entity);
    
    component->time = setting;
}

/*
    control = "freecam";
*/
#include "../Components/ControlComponent.h"
void EntityParser::parse_control(World* world, EntityHandle entity, const libconfig::Setting& setting) {
    ControlComponent* component = world->createComponent<ControlComponent>(entity);
    
    if(strcmp(setting, "freecam") == 0) {
        component->control_type = ControlComponent::ControlType::FREECAM;
    }
}