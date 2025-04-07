#include "libs.h"

#include "Scene.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Light.h"

void Scene::AddMesh(Mesh *mesh) { this->meshes.push_back(mesh); }

void Scene::AddTexture(Texture *texture) { this->textures.push_back(texture); }

void Scene::AddMaterial(Material *material) { this->materials.push_back(material); }

void Scene::AddDirectionalLight(DirectionalLight *light) { this->directional_lights.push_back(light); }

void Scene::AddShader(Shader *shader) { this->shaders.push_back(shader); }

void Scene::SendLightsToShaders()
{
    // Directional
    for (int i = 0; i < this->shaders.size(); i++)
    {
        Shader &shader = *(this->shaders[i]);
        // shader.SetInt("nrOfDirectionalLight", int(this->directional_lights.size()) );
        for (int j = 0; j < this->directional_lights.size(); j++)
        {
            DirectionalLight &directional_light = *(this->directional_lights[j]);
            directional_light.SendToShader(shader, j);
        }
    }
}

void Scene::BindTextures() { for (int i = 0; i < this->textures.size(); i++) { (*this->textures[i]).Bind(); } }
