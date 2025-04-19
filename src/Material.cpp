#include "libs.h"
#include "ErrorHandler.h"

#include "Material.h"
#include "Shader.h"

void Material::SendToShader(Shader* shader)
{
    (*shader).Use();
    (*shader).SetVec3("material.ambient", this->ambient);
    (*shader).SetVec3("material.diffuse", this->diffuse);
    (*shader).SetVec3("material.specular", this->specular);
    if (this->has_base_color) (*shader).SetInt("material.baseColorTex", this->baseColorTex);
    if (this->has_diffuse) (*shader).SetInt("material.diffusetTex", this->diffuseTex);
    if (this->has_specular) (*shader).SetInt("material.specularTex", this->specularTex);
    if (this->has_normals) (*shader).SetInt("material.normals", this->normalsTex);
    (*shader).SetFloat("material.shininess", this->shininess);
}

// Accessors
GLint Material::GetDiffuseTex() { return this->diffuseTex; }

GLint Material::GetSpecularTex() { return this->specularTex; }

// Modifiers
void Material::SetDiffuse(glm::vec3 &diffuseColor) { this->diffuse = diffuseColor; }

void Material::SetBaseColorTex(unsigned int &textureUnit)
{
    this->has_base_color = true;
    this->baseColorTex = textureUnit;
}

void Material::SetDiffuseTex(unsigned int &textureUnit)
{
    this->has_diffuse = true;
    this->diffuseTex = textureUnit;
}

void Material::SetSpecularTex(unsigned int &textureUnit)
{
    this->has_specular = true;
    this->specularTex = textureUnit;
}

void Material::SetNormalsTex(unsigned int &textureUnit)
{
    this->has_normals = true;
    this->normalsTex = textureUnit;
}

void Material::Print()
{
    std::cout << "Ambient:" << this->ambient.x << this->ambient.y << this->ambient.z << std::endl;
    std::cout << "Diffuse:" << this->diffuse.x << this->diffuse.y << this->diffuse.z << std::endl;
    std::cout << "Specular:" << this->specular.x << this->specular.y << this->specular.z << std::endl;
    std::cout << "HasBaseColorTex:" << (this->has_base_color == true ? "True" : "False") << std::endl;
    std::cout << "BaseColorTex:" << this->diffuseTex << std::endl;
    std::cout << "HasDiffuseTex:" << (this->has_diffuse == true ? "True" : "False") << std::endl;
    std::cout << "DiffuseTex:" << this->diffuseTex << std::endl;
    std::cout << "HasSpecularTex:" << (this->has_specular == true ? "True" : "False") << std::endl;
    std::cout << "SpecularTex:" << this->specularTex << std::endl;
    std::cout << "HasNormalTex:" << (this->has_normals == true ? "True" : "False") << std::endl;
    std::cout << "NormalTex:" << this->normalsTex << std::endl;
    std::cout << "Shininess:" << this->shininess << std::endl;
}
