#ifndef _SCENE_H_
#define _SCENE_H_

#include "libs.h"

#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "Light.h"
#include "Shader.h"

class Scene
{
public:
    std::vector<Mesh*> meshes;
    std::vector<Texture*> textures;
    std::vector<Material*> materials;
    std::vector<DirectionalLight*> directional_lights;
    std::vector<Shader*> shaders;
    
    Scene() = default;
    ~Scene() = default;
    
    void AddMesh(Mesh *mesh);
    void AddTexture(Texture *texture);
    void AddMaterial(Material *material);
    void AddDirectionalLight(DirectionalLight *light);
    void AddShader(Shader *shader);
    
    void SendLightsToShaders();
    void BindTextures();
    
private:
    
};

#endif
