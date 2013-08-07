#include "Spatial.h"

Spatial::Spatial(const glm::vec3 pos) : _position(pos), _scale(1.0) {
}

glm::mat4 Spatial::matrix() {
    glm::mat4 rotation_matrix = glm::mat4_cast(_rotation);
    glm::mat4 translation_matrix = glm::translate(_position);
    glm::mat4 scale_matrix = glm::scale(_scale);
    
    glm::mat4 model_matrix = translation_matrix * rotation_matrix * scale_matrix;
    return model_matrix;
}

void Spatial::translate(const glm::vec3& amount) {
    _position += amount;
}

void Spatial::look_at(const Spatial& spatial) {
    glm::mat4 look_at = glm::lookAt(_position, spatial._position, glm::vec3(0,1,0));
    _rotation = glm::quat_cast(glm::inverse(look_at));
}

void Spatial::rotate(glm::vec3 axis, float angle) {
    _rotation = glm::rotate(_rotation, angle, axis);
}

glm::vec3 Spatial::forward() const {
    return glm::normalize(glm::vec3(glm::vec4(glm::mat4_cast(_rotation) * glm::vec4(0,0,-1,0))));
}

glm::vec3 Spatial::right() const {
    return glm::normalize(glm::vec3(glm::vec4(glm::mat4_cast(_rotation) * glm::vec4(1,0,0,0))));
}

glm::vec3 Spatial::up() const {
    return glm::normalize(glm::vec3(glm::vec4(glm::mat4_cast(_rotation) * glm::vec4(0,1,0,0))));
}