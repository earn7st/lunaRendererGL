#include "libs.h"

#include "Scene.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Light.h"

DirectionalLight* Scene::GetDirectionalLight(int i) { return this->directional_lights[i]; }

void Scene::AddMesh(Mesh *mesh) { this->meshes.push_back(mesh); }

void Scene::AddTexture(Texture *texture) { this->textures.push_back(texture); }

void Scene::AddMaterial(Material *material) { this->materials.push_back(material); }

void Scene::AddDirectionalLight(DirectionalLight *light) { this->directional_lights.push_back(light); }

void Scene::AddModel(Model* model)
{
    this->models.push_back(model);
    for (int i = 0; i < model->GetNumOfMaterials(); i++)
    {
        this->materials.push_back(model->GetMaterial(i));
    }
}

void Scene::SendDirectionalLightsToShader(Shader* shader)
{
    // Directional
    for (int j = 0; j < this->directional_lights.size(); j++)
    {
        DirectionalLight &directional_light = *(this->directional_lights[j]);
        directional_light.SendToShader(shader, j);
    }
}

void Scene::BindTextures()
{
    for (int i = 0; i < this->textures.size(); i++)
    {
        (*this->textures[i]).Bind();
    }
}

void Scene::UnbindTextures()
{
    for (int i = 0; i < this->textures.size(); i++)
    {
        (*this->textures[i]).Unbind();
    }
}
