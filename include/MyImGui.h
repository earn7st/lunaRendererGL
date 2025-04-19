#ifndef _MY_IMGUI_H_
#define _MY_IMGUI_H_

#include "libs.h"

#include "Camera.h"
#include "Mesh.h"
#include "Model.h"

struct ImGui_Transform
{
    glm::vec3 scale = glm::vec3(0.f);
    glm::vec3 translation = glm::vec3(0.f);
    glm::vec3 rotation = glm::vec3(0.f);
};

struct ImGui_Material
{
    
};

struct ImGui_Model_Attribute
{
    Model* model;
    ImGui_Transform transform;
    // ImGui_Material material;
};

class MyImGui
{
public:
    MyImGui() = default;
    MyImGui(GLFWwindow* window, const char* glsl_version);
    ~MyImGui() = default;
    
    void Init();
    void Sleep();
    void NewFrame();
    void Render();
    
    void AddModelAttribute(Model* model);
    
    void UpdateModelAttribute();
    
    void DrawCameraWidget(Camera* camera);
    void DrawModelWidget(Model* model);
    
    
private:
    const char* glsl_version;
    GLFWwindow* window;
    
    std::vector<ImGui_Model_Attribute> model_attributes;
    
};

#endif
