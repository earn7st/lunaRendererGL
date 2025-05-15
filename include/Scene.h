#ifndef _SCENE_H_
#define _SCENE_H_

#include "libs.h"

#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "Light.h"
#include "Shader.h"
#include "Model.h"

class Scene
{
public:
    std::vector<Mesh*> meshes;
    std::vector<Texture*> textures;
    std::vector<Material*> materials;
    std::vector<DirectionalLight*> directional_lights;
    std::vector<Model*> models;
    
    Scene() = default;
    ~Scene() = default;
    
    size_t GetDirectionalLightCount();
    DirectionalLight* GetDirectionalLight(int i);
    
    void AddMesh(Mesh *mesh);
    void AddTexture(Texture *texture);
    void AddMaterial(Material *material);
    void AddDirectionalLight(DirectionalLight *light);
    void AddModel(Model *model);
    
    void SendDirectionalLightsToShader(Shader* shader);
    
    void BindTextures();
    void UnbindTextures();
    
private:
    
};

#endif
