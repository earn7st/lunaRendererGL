#include "libs.h"

#include "Light.h"

// Light Base
Light::Light(const std::string &name, glm::vec3 light_color) : name(name), color(light_color) { }


// Directional
DirectionalLight::DirectionalLight(const std::string &name, glm::vec3 light_direction, glm::vec3 light_color)
: direction(light_direction), Light(name, light_color) { }

void DirectionalLight::SendToShader(Shader* shader, int index) const
{
    (*shader).Use();
    (*shader).SetVec3(this->name + "[" + std::to_string(index) + "]" + ".color", this->color);
    (*shader).SetVec3(this->name + "[" + std::to_string(index) + "]" + ".direction", this->direction);
}


