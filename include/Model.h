#ifndef _MODEL_H_
#define _MODEL_H_

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "libs.h"

#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Light.h"

class Model
{
private:
    std::string directory;
    std::string name;
    
    std::vector<Mesh> meshes;
    std::vector<Material> materials;
    std::vector<Texture> textures;
    
    Transform transform;
    
    Shader* shader;
    
    unsigned int inc_texture_unit;
    
    void ProcessNode(const aiNode* ai_node, const aiScene* ai_scene);
    void LoadMaterials(const aiScene* ai_scene);
    void LoadMesh(const aiMesh* ai_mesh, const unsigned int ai_mesh_index);
    
public:
    Model(const std::string &pFile);
    ~Model() {};
    
    void Draw(const Camera& camera);
    void Draw(const DirectionalLight &light);
    
    void Scale(const glm::vec3 &scale);
    void Translate(const glm::vec3 &translation);
    void Rotate(const glm::vec3 &rotation);
    
    // Accessors
    std::string GetName();
    size_t GetNumOfMeshes();
    size_t GetNumOfMaterials();
    size_t GetNumOfTextures();
    Mesh* GetMesh(unsigned int index);
    Texture* GetTexture(unsigned int index);
    Material* GetMaterial(unsigned int index);
    Shader* GetShader();
    Transform GetTransform();
    
    // Modifers
    void SetShader(Shader *shader);
    void SetTransform(Transform &transform);
    
    void UpdateShaderToAllMeshes();
    void UpdateTransformToAllMeshes();
    void UpdateMaterialToAllMeshes(Material* material); // for customization
    
    void PrintMeshesInfo();
    void PrintMaterialsInfo();

};

#endif
