#include <iostream>

#include "libs.h"
#include "ErrorHandler.h"

#include "Camera.h"
#include "Mesh.h"

Mesh::Mesh(const float* data, const int& data_count)
{
    for (size_t i = 0; i < data_count; i += 8)
    {
        Vertex vertex(data + i);
        this->vertices.push_back(vertex);
    }
    InitVAO();
}

Mesh::Mesh(const float* data, const int& data_count, const GLint* indices, const int& indices_count)
{
    for (size_t i = 0; i < data_count; i += 8)
    {
        Vertex vertex(data + i);
        this->vertices.push_back(vertex);
    }
    for (size_t i = 0; i < indices_count; i ++)
    {
        this->indices.push_back(indices[i]);
    }
    InitVAO();
}

Mesh::Mesh(const Primitive& primitive)
{
    for (size_t i = 0; i < primitive.GetVerticesCount(); i++)
    {
        this->vertices.push_back(primitive.GetVertex(i));
    }
    for (size_t i = 0; i < primitive.GetIndicesCount(); i ++)
    {
        this->indices.push_back(primitive.GetIndex(i));
    }
    InitVAO();
}

Mesh::~Mesh()
{
    glCall(glDeleteVertexArrays(1, &this->VAO));
    glCall(glDeleteBuffers(1, &this->VBO));
    glCall(glDeleteBuffers(1, &this->EBO));
}

void Mesh::InitVAO()
{
    glCall(glGenVertexArrays(1, &this->VAO));
    glCall(glBindVertexArray(this->VAO));
    
    glCall(glGenBuffers(1, &this->VBO));
    glCall(glBindBuffer(GL_ARRAY_BUFFER, this->VBO));
    glCall(glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), this->vertices.data(), GL_STATIC_DRAW));
    
    glCall(glGenBuffers(1, &this->EBO));
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO));
    glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), this->indices.data(), GL_STATIC_DRAW));
    
    // position
    glCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position)));
    glCall(glEnableVertexAttribArray(0));
    
    // normal
    glCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal)));
    glCall(glEnableVertexAttribArray(1));

    // texcoord
    glCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord)));
    glCall(glEnableVertexAttribArray(2));
    
    glCall(glBindVertexArray(0));
}

inline size_t Mesh::GetVerticesCount() const
{
    return this->vertices.size();
}

inline size_t Mesh::GetIndicesCount() const
{
    return this->indices.size();
}

void Mesh::SetShader(Shader *shader)
{
    this->shader = shader;
}

void Mesh::SetMaterial(Material *material)
{
    this->material = material;
}

void Mesh::UpdateModelMatrix()
{
    // Scale
    glm::mat4 scale = glm::scale(glm::mat4(1.f), this->transform.scale);
    
    // Translate
    glm::mat4 translation = glm::translate(glm::mat4(1.f), this->transform.translation);
    
    // Rotate
    // Using Euler
    glm::mat4 rotationX = glm::rotate(glm::mat4(1.f), glm::radians(this->transform.rotation.x), glm::vec3(1.f, 0.f, 0.f));
    glm::mat4 rotationY = glm::rotate(glm::mat4(1.f), glm::radians(this->transform.rotation.y), glm::vec3(0.f, 1.f, 0.f));
    glm::mat4 rotationZ = glm::rotate(glm::mat4(1.f), glm::radians(this->transform.rotation.z), glm::vec3(0.f, 0.f, 1.f));
    glm::mat4 rotation = rotationZ * rotationY * rotationX;
    
    // Using Quat
    
    // Compute modelMatrix
    this->modelMatrix = translation * scale * rotation;
}

void Mesh::UpdateMVP(const Camera& camera)
{
    this->UpdateModelMatrix();
    (*this->shader).SetMat4("model", this->modelMatrix);
    (*this->shader).SetMat4("view", camera.GetViewMatrix());
    (*this->shader).SetMat4("proj", camera.GetProjMatrix());
}

void Mesh::Draw(const Camera& camera)
{
    // Set Transform Matrix
    this->UpdateMVP(camera);
    
    // Set Material
    (*this->material).SendToShader((*this->shader));
    
    // Bind VAO
    glCall(glBindVertexArray(VAO));
    
    // Renderer
    if (this->indices.empty())
    {
        glCall(glDrawArrays(GL_TRIANGLES, 0, (GLsizei)this->vertices.size() * sizeof(Vertex)));
    } else{
        glCall(glDrawElements(GL_TRIANGLES, (GLsizei)this->indices.size(), GL_UNSIGNED_INT, (GLvoid*)0));
    }

}
