#include "libs.h"
#include "ErrorHandler.h"

#include "Material.h"
#include "Shader.h"

Material::~Material() { }

void Material::SendToShader(Shader& shader)
{
    shader.Use();
    shader.SetVec3("material.ambient", this->ambient);
    shader.SetVec3("material.diffuse", this->diffuse);
    shader.SetVec3("material.specular", this->specular);
    shader.SetInt("material.diffusetTex", this->diffuseTex);
    shader.SetInt("material.specularTex", this->specularTex);
    shader.SetFloat("material.shininess", this->shininess);
}

void Material::SetDiffuse(glm::vec3 diffuseColor)
{
    this->diffuse = diffuseColor;
}
