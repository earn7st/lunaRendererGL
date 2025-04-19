#include "libs.h"

#include "Model.h"
#include "Vertex.h"
#include "Texture.h"

Model::Model(const std::string &pFile)
{
    this->directory = pFile.substr(0, pFile.find_last_of('/'));
    
    Assimp::Importer importer;

    const aiScene* ai_scene = importer.ReadFile(pFile,
        aiProcess_CalcTangentSpace       |
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType);
    
    if(!ai_scene || ai_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !ai_scene->mRootNode)
    {
        std:: cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    this->meshes.reserve(ai_scene->mNumMeshes);
    this->materials.reserve(ai_scene->mNumMaterials);
    
    this->LoadMaterials(ai_scene);
        
    this->ProcessNode(ai_scene->mRootNode, ai_scene);

}

void Model::ProcessNode(const aiNode* ai_node, const aiScene* ai_scene)
{
    for (unsigned int i = 0; i < ai_node->mNumMeshes; i++)
    {
        aiMesh* ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[i]];
        this->LoadMesh(ai_mesh, ai_node->mMeshes[i]);
    }
          
    for (unsigned int i = 0; i < ai_node->mNumChildren; i++)
    {
        ProcessNode(ai_node->mChildren[i], ai_scene);
    }
}

void Model::LoadMaterials(const aiScene* ai_scene)
{
    
    for (unsigned int i = 0; i < ai_scene->mNumMaterials; i++)
    {
        const aiMaterial* ai_material = ai_scene->mMaterials[i];
        
        Material material;
        
        // Base Color
        for (unsigned int j = 0; j < ai_material->GetTextureCount(aiTextureType_BASE_COLOR); j++)
        {
            aiString ai_texture_path;
            if(ai_material->GetTexture(aiTextureType_BASE_COLOR, j, &ai_texture_path) == AI_SUCCESS)
            {
                std::string texture_path(ai_texture_path.data);
                std::string full_texture_path = this->directory + "/" + texture_path;
                
                material.SetBaseColorTex(inc_texture_unit);
                
                Texture texture(full_texture_path, GL_TEXTURE_2D, TEXTURE_TYPE_DIFFUSE, this->inc_texture_unit++);
                this->textures.push_back(texture);
            }
        }
        
        // Diffuse
        for (unsigned int j = 0; j < ai_material->GetTextureCount(aiTextureType_DIFFUSE); j++)
        {
            aiString ai_texture_path;
            if(ai_material->GetTexture(aiTextureType_DIFFUSE, j, &ai_texture_path) == AI_SUCCESS)
            {
                std::string texture_path(ai_texture_path.data);
                std::string full_texture_path = this->directory + "/" + texture_path;
                
                material.SetDiffuseTex(inc_texture_unit);
                
                Texture texture(full_texture_path, GL_TEXTURE_2D, TEXTURE_TYPE_DIFFUSE, this->inc_texture_unit++);
                this->textures.push_back(texture);
            }
        }
        
        // Specular
        for (unsigned int j = 0; j < ai_material->GetTextureCount(aiTextureType_SPECULAR); j++)
        {
            aiString ai_texture_path;
            if(ai_material->GetTexture(aiTextureType_SPECULAR, j, &ai_texture_path) == AI_SUCCESS)
            {
                std::string texture_path(ai_texture_path.data);
                std::string full_texture_path = this->directory + "/" + texture_path;
                
                material.SetSpecularTex(inc_texture_unit);
                
                Texture texture(full_texture_path, GL_TEXTURE_2D, TEXTURE_TYPE_SPECULAR, this->inc_texture_unit++);
                this->textures.push_back(texture);
            }
        }
        
        // Normals
        for (unsigned int j = 0; j < ai_material->GetTextureCount(aiTextureType_NORMALS); j++)
        {
            aiString ai_texture_path;
            if(ai_material->GetTexture(aiTextureType_NORMALS, j, &ai_texture_path) == AI_SUCCESS)
            {
                std::string texture_path(ai_texture_path.data);
                std::string full_texture_path = this->directory + "/" + texture_path;
                
                material.SetNormalsTex(inc_texture_unit);
                
                Texture texture(full_texture_path, GL_TEXTURE_2D, TEXTURE_TYPE_SPECULAR, this->inc_texture_unit++);
                this->textures.push_back(texture);
            }
        }
        
        this->materials.push_back(material);
    }
}

void Model::LoadMesh(const aiMesh* ai_mesh, const unsigned int ai_mesh_index)
{

    std::vector<Vertex> Vertices;
    
    for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++)
    {
        const aiVector3D zeroVec3(0.f, 0.f, 0.f);
        
        const aiVector3D& ai_pos       = ai_mesh->mVertices[i];
        const aiVector3D& ai_normal    = ai_mesh->HasNormals() ? ai_mesh->mNormals[i] : zeroVec3;
        const aiVector3D& ai_texcoord  = ai_mesh->HasTextureCoords(0) ? ai_mesh->mTextureCoords[0][i] : zeroVec3;
        
        glm::vec3 pos       = glm::vec3(ai_pos.x, ai_pos.y, ai_pos.z);
        glm::vec3 normal    = glm::vec3(ai_normal.x, ai_normal.y, ai_normal.z);
        glm::vec2 texcoord  = glm::vec2(ai_texcoord.x, ai_texcoord.y);
        
        Vertex vertex(pos, normal, texcoord);
        
        Vertices.push_back(vertex);
    }
    
    std::vector<GLuint> Indices;
    
    for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++)
    {
        const aiFace& ai_face = ai_mesh->mFaces[i];
        assert(ai_face.mNumIndices == 3);
        
        Indices.push_back(ai_face.mIndices[0]);
        Indices.push_back(ai_face.mIndices[1]);
        Indices.push_back(ai_face.mIndices[2]);
    }
     
    Mesh mesh(Vertices, Indices);
    mesh.SetMaterial(&(this->materials[ai_mesh->mMaterialIndex]));
    this->meshes.push_back(mesh);
    
}

// Public Functions
void Model::Draw(const Camera &camera)
{
    
    if(this->shader == nullptr)
    {
        std::cout << "Model does not have a shader for Drawing!!" << std::endl;
        return ;
    }
    
    // this->UpdateMaterialToAllMeshes();
    
    // Bind Textures
    for (unsigned int i = 0; i < this->textures.size(); i++)
    {
        this->textures[i].Bind();
    }
    
    this->UpdateTransformToAllMeshes();
    
    for (unsigned int i = 0; i < this->meshes.size(); i++)
    {
        Mesh &mesh = meshes[i];
        
        if(mesh.GetMaterial() == nullptr)
        {
            std::cout << "Model::Mesh[" << i << "]: Mesh does not have any material for Drawing!!" << std::endl;
            return ;
        }
        
        mesh.Draw(camera);
        
    }
}

void Model::UpdateShaderToAllMeshes()
{
    for (int i = 0; i < this->meshes.size(); i++)
    {
        Mesh &mesh = this->meshes[i];
        mesh.SetShader(this->shader);
    }
}

void Model::UpdateMaterialToAllMeshes(Material* material)
{
    for (int i = 0; i < this->meshes.size(); i++)
    {
        Mesh &mesh = this->meshes[i];
        mesh.SetMaterial(material);
    }
}

// Accessors
size_t Model::GetNumOfMeshes() { return this->meshes.size(); }

size_t Model::GetNumOfMaterials() { return this->materials.size(); }

size_t Model::GetNumOfTextures() { return this->textures.size(); }

Mesh* Model::GetMesh(unsigned int index) { return (&this->meshes[index]); }

Texture* Model::GetTexture(unsigned int index) { return (&this->textures[index]); }

Material* Model::GetMaterial(unsigned int index) { return &this->materials[index]; }

Shader* Model::GetShader() { return this->shader; }

// Modifiers
void Model::SetShader(Shader *shader)
{
    this->shader = shader;
    this->UpdateShaderToAllMeshes();
}

void Model::SetTransform(Transform &transform)
{
    this->transform = transform;
    this->UpdateTransformToAllMeshes();
}

void Model::UpdateTransformToAllMeshes()
{
    for (int i = 0; i < this->meshes.size(); i++)
    {
        Mesh &mesh = this->meshes[i];
        mesh.SetTransform(this->transform);
    }
}

void Model::PrintMeshesInfo()
{
    for (int i = 0; i < this->meshes.size(); i++)
    {
        Mesh &mesh = this->meshes[i];
        
        std::cout << "Mesh[" << i << "] :" << std::endl;
        
        std::cout << "NumOfVertices:" << mesh.GetVerticesCount() << std::endl;
        
        std::cout << "NumOfIndices:" << mesh.GetIndicesCount() << std::endl;
        
        std::cout << "VAO:" << mesh.GetVAO() << std::endl;
        
        std::cout << "Material:" << std::endl;
        mesh.GetMaterial()->Print();
        
        std::cout << "Shader: " << &(*(mesh.GetShader())) << std::endl;
        
    }
}

void Model::PrintMaterialsInfo()
{
    for (int i = 0; i < this->materials.size(); i++)
    {
        std::cout << "Material[" << i << "] :" << std::endl;
        this->materials[i].Print();
    }
}

