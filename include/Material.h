#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "libs.h"

#include "Shader.h"

class Material
{
private:
    glm::vec3 ambient   = glm::vec3(0.2f);
    glm::vec3 diffuse   = glm::vec3(1.f);
    glm::vec3 specular  = glm::vec3(1.f);
    bool has_base_color = false;
    GLint baseColorTex  = 0;
    bool has_diffuse    = false;
    GLint diffuseTex    = 0;
    bool has_specular   = false;
    GLint specularTex   = 0;
    bool has_normals     = false;
    GLint normalsTex     = 0;
    float shininess     = 32;

public:
    Material() {};
    Material(glm::vec3 ambient,
             glm::vec3 diffuse,
             glm::vec3 specular,
             bool has_diffuse,
             GLint diffuseTex,
             bool has_specular,
             GLint specularTex,
             float shininess)
    : ambient(ambient), diffuse(diffuse), specular(specular), has_diffuse(has_diffuse), diffuseTex(diffuseTex), has_specular(has_specular), specularTex(specularTex), shininess(shininess){};
    ~Material() {};
    
    // Functions
    void SendToShader(Shader* shader);
    
    // Accessors
    GLint GetDiffuseTex();
    GLint GetSpecularTex();
    
    // Modifiers
    void SetDiffuse(glm::vec3 &diffuseColor);
    
    void SetBaseColorTex(unsigned int &textureUnit);
    void SetDiffuseTex(unsigned int &textureUnit);
    void SetSpecularTex(unsigned int &textureUnit);
    void SetNormalsTex(unsigned int &textureUnit);
    
    void Print();
    
};

#endif
