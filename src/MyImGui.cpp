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

void MyImGui::AddModelAttribute(Model* model)
{
    ImGui_Model_Attribute attribute;
    attribute.model = model;
    attribute.transform = model->GetTransform();
    this->model_attributes.push_back(attribute);
}


bool MyImGui::GetWantCaptureMouse()
{
    return this->wantCaptureMouse;
}

void MyImGui::UpdateModelAttributes()
{
    for (int i = 0; i < this->model_attributes.size(); i++)
    {
        Model* model = this->model_attributes[i].model;
        model->SetTransform(this->model_attributes[i].transform);
    }
}

void MyImGui::DrawCameraWidget(Camera *camera)
{
    ImGui::Begin("Tool");
    
    ImGuiIO& io = ImGui::GetIO();
    this->wantCaptureMouse = io.WantCaptureMouse;
    
    if (camera->GetMode() == EDIT) { ImGui::Text("CameraMode[V]: EDIT"); }
    else if(camera->GetMode() == FPS) { ImGui::Text("CameraMode[V]: FPS"); } 
    
    ImGui::End();
}

void MyImGui::DrawModelWidget(Model *model)
{
    
    ImGui_Model_Attribute* attribute = nullptr;
    
    for (int i = 0; i < this->model_attributes.size(); i++)
    {
        if (this->model_attributes[i].model == model)
        {
            attribute = &this->model_attributes[i];
            break;
        }
    }
    
    ImGui::Begin(attribute->model->GetName().c_str()); // Add model's name!!

    ImGuiIO &io = ImGui::GetIO();
    this->wantCaptureMouse = io.WantCaptureMouse;
    
    // Scale
    ImGui::Text("Scale");
    ImGui::SliderFloat("X##1", &(attribute->transform.scale[0]), 0.1, 5);
    ImGui::SliderFloat("Y##1", &(attribute->transform.scale[1]), 0.1, 5);
    ImGui::SliderFloat("Z##1", &(attribute->transform.scale[2]), 0.1, 5);
    
    // Translation
    ImGui::Text("Translation");
    ImGui::InputFloat("X##2", &(attribute->transform.translation[0]));
    ImGui::InputFloat("Y##2", &(attribute->transform.translation[1]));
    ImGui::InputFloat("Z##2", &(attribute->transform.translation[2]));
    
    // Rotation
    ImGui::Text("Rotation");
    ImGui::SliderFloat("X##3", &(attribute->transform.rotation[0]), -180, 180);
    ImGui::SliderFloat("Y##3", &(attribute->transform.rotation[1]), -180, 180);
    ImGui::SliderFloat("Z##3", &(attribute->transform.rotation[2]), -180, 180);
    
    ImGui::End();
}

