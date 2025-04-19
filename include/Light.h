#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "libs.h"

#include "Shader.h"

class Light
{
public:
    Light();
    Light(const std::string &name, glm::vec3 light_color);
    ~Light() = default;
    
protected:
    std::string name;
    glm::vec3 color = glm::vec3(1.f);
};

// Directional
class DirectionalLight : public Light
{
public:
    DirectionalLight(const std::string &name, glm::vec3 light_direction, glm::vec3 light_color);
    ~DirectionalLight() = default;
    
    // Functions
    void SendToShader(Shader* shader, int index) const;
    
private:
    glm::vec3 direction;
};

// Point

// Spotlight

#endif

