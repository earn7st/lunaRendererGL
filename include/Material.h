#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "libs.h"

#include "Shader.h"

class Material
{
private:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    GLint diffuseTex;
    GLint specularTex;
    float shininess;

public:
    Material(glm::vec3 ambient,
             glm::vec3 diffuse,
             glm::vec3 specular,
             GLint diffuseTex,
             GLint specularTex,
             float shininess)
    : ambient(ambient), diffuse(diffuse), specular(specular), diffuseTex(diffuseTex), specularTex(specularTex), shininess(shininess){};
    ~Material();
    
    // Functions
    void SendToShader(Shader& shader);
    
    // Modifiers
    void SetDiffuse(glm::vec3 diffuseColor);
    
};

#endif
