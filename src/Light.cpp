#include "libs.h"

#include "Light.h"

// Light Base
Light::Light(const std::string &name, glm::vec3 light_color, glm::vec3 position) : name(name), color(light_color), position(position) { }

// Directional
DirectionalLight::DirectionalLight(const std::string &name, glm::vec3 light_color, glm::vec3 position, glm::vec3 focalPoint, glm::vec3 lightUp)
: Light(name, light_color, position), focalPoint(focalPoint), lightUp(lightUp) { }

glm::vec3 DirectionalLight::GetPosition() const { return this->position; }
glm::vec3 DirectionalLight::GetFocalPoint() const { return this->focalPoint; }
glm::vec3 DirectionalLight::GetLightUp() const { return this->lightUp; }

void DirectionalLight::SendToShader(Shader* shader, int index) const
{
    (*shader).Use();
    (*shader).SetVec3(this->name + "[" + std::to_string(index) + "]" + ".color", this->color);
    (*shader).SetVec3(this->name + "[" + std::to_string(index) + "]" + ".direction", glm::normalize(this->position - this->focalPoint));
}

glm::mat4 DirectionalLight::CalcLightVP(float frustum_size, float zNear, float zFar) const
{
    // view
    glm::mat4 view = glm::lookAt(this->position, this->focalPoint, this->lightUp);

    // projection
    float r = frustum_size, l = -r, t = frustum_size, b = -t;

    glm::mat4 proj = glm::ortho(l, r, b, t, zNear, zFar);

    return proj * view;
}
