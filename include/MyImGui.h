#ifndef _MY_IMGUI_H_
#define _MY_IMGUI_H_

#include "libs.h"

#include "Camera.h"
#include "Mesh.h"
#include "Model.h"

struct ImGui_Model_Attribute
{
    Model* model;
    Transform transform;
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

    void AddModelAttribute(Model *model);
    
    void UpdateShadowAttributes();
    void UpdateModelAttributes();
    
    bool GetWantCaptureMouse();
    
    void DrawCameraWidget(Camera* camera);
    void DrawModelWidget(Model* model);
    
    
private:

    const char* glsl_version;
    GLFWwindow* window;
    
    bool wantCaptureMouse = false;
    bool shadow = false;
    std::vector<ImGui_Model_Attribute> model_attributes;
    
    
};

#endif
