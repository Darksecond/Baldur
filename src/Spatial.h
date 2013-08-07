#pragma once

#include <GLM/glm.hpp>
#include <GLM/ext.hpp>

class Spatial {
public:
    Spatial(const glm::vec3 pos = glm::vec3());
    glm::mat4 matrix();
    void translate(const glm::vec3& amount);
    void look_at(const Spatial& spatial);
    void rotate(glm::vec3 axis, float angle);
    
    glm::vec3 forward() const;
    glm::vec3 right() const;
    glm::vec3 up() const;
private:
    glm::vec3 _position;
    glm::quat _rotation;
    glm::vec3 _scale;
};