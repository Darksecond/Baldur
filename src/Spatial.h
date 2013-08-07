#pragma once

#include <GLM/glm.hpp>
#include <GLM/ext.hpp>

class Spatial {
public:
    Spatial(const glm::vec3 pos = glm::vec3());
    glm::mat4 matrix();
    void translate(const glm::vec3& amount);
    void look_at(const Spatial& spatial);
private:
    glm::vec3 _position;
    glm::quat _rotation;
    glm::vec3 _scale;
};