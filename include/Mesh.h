#ifndef _MESH_H_
#define _MESH_H_

#include "libs.h"

#include "Camera.h"
#include "Shader.h"
#include "Vertex.h"
#include "Primitives.h"
#include "Material.h"

struct Transform
{
    glm::vec3 scale = glm::vec3(1.f);
    glm::vec3 rotationOrigin;
    glm::quat rotationQuat;
    glm::vec3 rotation = glm::vec3(0.f);
    glm::vec3 translation = glm::vec3(0.f);
};

class Mesh
{
private:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    
    Shader *shader;
    
    // Material
    Material *material;
    
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    
    Transform transform;
    glm::mat4 modelMatrix = glm::mat4(1.f);
    
    void InitVAO();
    
    void UpdateModelMatrix();
    void UpdateMVP(const Camera &camera);
    
public:
    
    Mesh() {};
    Mesh(const float* data, const int& data_count);
    Mesh(const float* data, const int& data_count, const GLint* indices, const int& indices_count);
    Mesh(const Primitive& primitive);
    Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices);
    
    ~Mesh() = default;
    
    size_t GetVerticesCount() const;
    size_t GetIndicesCount() const;
    unsigned int GetVAO() const;
    Shader* GetShader();
    Material* GetMaterial();
    
    void SetShader(Shader *shader);
    void SetMaterial(Material *material);
    void SetTransform(Transform &transform);
    
    void Draw(const Camera& camera);
    
    void PrintInfo();
    
};

#endif
