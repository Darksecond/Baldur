#pragma once

#include <libconfig/libconfig.hh>

class World;
class Entity;
typedef Entity* EntityHandle;

class EntityParser {
public:
    EntityHandle parse(World* world, const char* identifier, const char* name);
private:
    void parse_ttl(World* world, EntityHandle entity, const libconfig::Setting& setting);
    void parse_spatial(World* world, EntityHandle entity, const libconfig::Setting& setting);
    void parse_model(World* world, EntityHandle entity, const libconfig::Setting& setting);
    void parse_camera(World* world, EntityHandle entity, const libconfig::Setting& setting);
    void parse_time(World* world, EntityHandle entity, const libconfig::Setting& setting);
    void parse_control(World* world, EntityHandle entity, const libconfig::Setting& setting);
    void parse_rigidBody(World* world, EntityHandle entity, const libconfig::Setting& setting);
};