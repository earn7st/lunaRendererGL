#include "libs.h"

#include "MyImGui.h"

MyImGui::MyImGui(GLFWwindow* window, const char* glsl_version)
: window(window), glsl_version(glsl_version)
{
    this->Init();
}

void MyImGui::Init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    
    ImGui::StyleColorsLight();
    
    ImGui_ImplGlfw_InitForOpenGL(this->window, true);
    ImGui_ImplOpenGL3_Init(this->glsl_version);
}

void MyImGui::Sleep() { ImGui_ImplGlfw_Sleep(10); }

void MyImGui::NewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void MyImGui::Render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MyImGui::AddModelAttribute(Model *model)
{
    ImGui_Model_Attribute attribute;
    attribute.model = model;
    this->model_attributes.push_back(attribute);
}

void MyImGui::DrawCameraWidget(Camera *camera)
{
    ImGui::Begin("Tool");
    if (camera->GetMode() == EDIT)
    {
        ImGui::Text("CameraMode : EDIT");
    }
    else if(camera->GetMode() == FPS)
    {
        ImGui::Text("CameraMode : FPS");
    }
    ImGui::End();
}

void MyImGui::DrawModelWidget(Model *model)
{
    
    ImGui_Model_Attribute attribute;
    
    for (int i = 0; i < this->model_attributes.size(); i++)
    {
        if (this->model_attributes.model == model)
        {
            attribute = this->model_attributes[i];
            break;
        }
    }
    
    ImGui::Begin("Model"); // Add model's name!!
    
    // Scale
    ImGui::Text("Scale: x = %.1f", &attribute.transform.scale.x);
    ImGui::SameLine();
    ImGui::Text("Scale: y = %.1f", &attribute.transform.scale.y);
    ImGui::SameLine();
    ImGui::Text("Scale: z = %.1f", &attribute.transform.scale.z);
    
    // Translation
    ImGui::Text("Translation: x = %.1f", &(attribute.transform.translation.x));
    ImGui::SameLine();
    ImGui::Text("Translation: y = %.1f", &attribute.transform.translation.y);
    ImGui::SameLine();
    ImGui::Text("Translation: z = %.1f", &attribute.transform.translation.z);
    
    // Rotation
    ImGui::Text("Rotation: x = %.1f", &(attribute.transform.rotation.x));
    ImGui::SameLine();
    ImGui::Text("Rotation: y = %.1f", &attribute.transform.rotation.y);
    ImGui::SameLine();
    ImGui::Text("Rotation: z = %.1f", &attribute.transform.rotation.z);
    
    ImGui::End();
}
