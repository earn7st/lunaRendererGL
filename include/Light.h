#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "libs.h"

#include "Shader.h"

class Light
{
public:
    Light();
    Light(const std::string &name, glm::vec3 light_color, glm::vec3 position);
    ~Light() = default;
    
protected:
    std::string name;
    glm::vec3 position;
    glm::vec3 color = glm::vec3(1.f);
};

// Directional
class DirectionalLight : public Light
{
public:
    DirectionalLight(const std::string &name, glm::vec3 light_color, glm::vec3 position, glm::vec3 focalPoint, glm::vec3 lightUp);
    ~DirectionalLight() = default;
    
    // Functions
    void SendToShader(Shader* shader, int index) const;
    glm::mat4 CalcLightVP(float frustum_size, float zNear, float zFar) const;
    
    glm::vec3 GetPosition() const ;
    glm::vec3 GetFocalPoint() const ;
    glm::vec3 GetLightUp() const ;
    
private:
    glm::vec3 focalPoint = glm::vec3(0.);
    glm::vec3 lightUp = glm::vec3(0, 1, 0);
};

// Point

// Spotlight

#endif

