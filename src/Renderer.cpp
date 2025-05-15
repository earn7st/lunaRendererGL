#include <iostream>

#include "libs.h"
#include "ErrorHandler.h"

#include "Renderer.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"

Renderer::Renderer(const char* title,
         const int& WINDOW_WIDTH,
         const int& WINDOW_HEIGHT,
         const int& GL_VERSION_MAJOR,
         const int& GL_VERSION_MINOR)
:   WINDOW_WIDTH(WINDOW_WIDTH), WINDOW_HEIGHT(WINDOW_HEIGHT),
    GL_VERSION_MAJOR(GL_VERSION_MAJOR), GL_VERSION_MINOR(GL_VERSION_MINOR)
{
    this->window = nullptr;
    this->framebufferWidth = this->WINDOW_WIDTH;
    this->framebufferHeight = this->WINDOW_HEIGHT;
    
    this->InitGLFW();
    this->InitWindow(title);
    this->InitGLAD();
    this->InitOpenGLOptions();
    this->InitCamera();
    this->InitCallbacks();
    this->InitImGui();
    this->InitShadow();
    
    this->lastX = this->WINDOW_WIDTH  / 2;
    this->lastY = this->WINDOW_HEIGHT / 2;

}

Renderer::~Renderer()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

// Public
// Accessors
int Renderer::GetWindowShouldClose() { return glfwWindowShouldClose(this->window); }

// Modifiers
void Renderer::SetWindowShouldClose() { glfwSetWindowShouldClose(this->window, GL_TRUE); }

void Renderer::SetShadowMapShader(Shader &shader) { this->shadowMapShader = &shader; }
// Functions
void Renderer::Render(Scene &scene)
{

    for (int i = 0; i < scene.models.size(); i++)
    {
        this->imgui.AddModelAttribute(scene.models[i]);
    }
    
    while(!this->GetWindowShouldClose())
    {
        
        glfwPollEvents();
        
        float currentFrame = static_cast<float>(glfwGetTime());
        this->deltaTime = currentFrame - this->lastFrame;
        this->lastFrame = currentFrame;
        
        this->ProcessInput(this->window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            this->imgui.Sleep();
            continue;
        }
        
        this->imgui.NewFrame();
        this->imgui.DrawCameraWidget(&this->camera);
        
        if(this->ENABLE_SHADOW)
        {
            // Pass 1: Render Shadow Map
            DirectionalLight &main_light = *scene.directional_lights[0];
            glm::mat4 lightVP = main_light.CalcLightVP(10, 0.001, 30);
            this->ConfigureShadowViewport();
            this->BindShadowFramebuffer();
            glClear(GL_DEPTH_BUFFER_BIT);
            for (int j = 0; j < scene.models.size(); j++)
            {
                Model& model = *scene.models[j];
                Shader* shader = model.GetShader();
                
                model.SetShader(this->shadowMapShader);
                (*this->shadowMapShader).Use();
                (*this->shadowMapShader).SetMat4("lightVP", lightVP);
                
                model.Draw(main_light);
                    
                model.SetShader(shader);
            }
            
            // Pass 2: Draw models in scene
            
            this->ConfigureScreenViewport();
            this->BindScreenFramebuffer();
            for (int j = 0; j < scene.models.size(); j++)
            {
                Model& model = *scene.models[j];
                Shader* shader = model.GetShader();
                
                this->BindShadowTexture();
                shader->Use();
                shader->SetInt("shadowMap", this->shadowMapTextureUnit);
                shader->SetMat4("lightVP", lightVP);
                
                scene.SendDirectionalLightsToShader(model.GetShader());
                
                // ImGui
                this->imgui.DrawModelWidget(&model);
                imgui.UpdateModelAttributes();
                model.Draw(this->camera);
            }
            
        }
        else
        {
            for (int j = 0; j < scene.models.size(); j++)
            {
                Model &model = *scene.models[j];
                scene.SendDirectionalLightsToShader(model.GetShader());
                
                // ImGui
                this->imgui.DrawModelWidget(&model);
                imgui.UpdateModelAttributes();
                model.Draw(this->camera);
            }
        }
        
       
        
        imgui.Render();
        
        /*
        // Bind Textures (for meshes not from models)
        scene.BindTextures();
        //Draw Meshes in this Scene
        for (int i = 0; i < scene.meshes.size(); i++)
        {
            Mesh &mesh = *scene.meshes[i];
            scene.SendLightsToShader(mesh.GetShader());
            mesh.Draw(this->camera);
        }
        scene.UnbindTextures();
        */
        
        glfwSwapBuffers(window);
    }
    
}

void Renderer::ConfigureShadowViewport() { glViewport(0, 0, this->SHADOW_WIDTH, this->SHADOW_HEIGHT); }
void Renderer::ConfigureScreenViewport() { glViewport(0, 0, this->WINDOW_WIDTH, this->WINDOW_HEIGHT); }
void Renderer::BindShadowFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, this->shadowMapFBO); }
void Renderer::BindScreenFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void Renderer::BindShadowTexture()
{
    if(this->shadowMapTextureUnit == -1) return ;
     
    glCall(glActiveTexture(GL_TEXTURE0 + this->shadowMapTextureUnit));
    glCall(glBindTexture(GL_TEXTURE_2D, this->shadowMapTexID));
}
  
// Private
// Init
void Renderer::InitGLFW()
{
    if(glfwInit() == GLFW_FALSE)
    {
        std::cout << "ERROR::GLFW_INIT_FAILED" << std::endl;
        glfwTerminate();
    }
}

void Renderer::InitWindow(const char* title)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, this->resizable);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    this->window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title, NULL, NULL);
    
    if (this->window == nullptr)
    {
        std::cout << "ERROR::GLFW_INIT_WINDOW_FAILED" << std::endl;
    }
    
    glfwMakeContextCurrent(this->window);
    
}

void Renderer::InitGLAD()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
    }
}

void Renderer::InitOpenGLOptions()
{
    glEnable(GL_DEPTH_TEST);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::InitCamera()
{
    camera.SetAspectRatio(float(WINDOW_WIDTH)/float(WINDOW_HEIGHT));
}

void Renderer::InitCallbacks()
{
    CallbackWrapper::SetRenderer(this);
    
    glfwSetFramebufferSizeCallback(this->window, CallbackWrapper::framebuffer_size_callback);
    glfwSetKeyCallback(this->window, CallbackWrapper::key_callback);
    glfwSetCursorPosCallback(this->window, CallbackWrapper::mouse_pos_callback);
    glfwSetMouseButtonCallback(this->window, CallbackWrapper::mouse_button_callback);
    glfwSetScrollCallback(this->window, CallbackWrapper::scroll_callback);
}

void Renderer::InitImGui()
{
    this->imgui = MyImGui(this->window, this->glsl_version);
}

void Renderer::InitShadow()
{
    // Shadow FBO
    glGenFramebuffers(1, &this->shadowMapFBO);
    
    // Shadow Texture
    glGenTextures(1, &this->shadowMapTexID);
    glBindTexture(GL_TEXTURE_2D, this->shadowMapTexID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, this->shadowMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->shadowMapTexID, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
}

// Private Class : CallbackWrapper
// definition for static variable
Renderer* Renderer::CallbackWrapper::renderer = nullptr;
void Renderer::CallbackWrapper::SetRenderer(Renderer *r) { CallbackWrapper::renderer = r; }

void Renderer::CallbackWrapper::framebuffer_size_callback(GLFWwindow *window, int width, int height) { renderer->FramebufferSizeCallback(window, width, height); }

void Renderer::CallbackWrapper::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) { renderer->KeyCallback(window, key, scancode, action, mods); }

void Renderer::CallbackWrapper::mouse_pos_callback(GLFWwindow *window, double xpos, double ypos) { renderer->MousePosCallback(window, xpos, ypos); }

void Renderer::CallbackWrapper::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) { renderer->MouseButtonCallback(window, button, action, mods); }

void Renderer::CallbackWrapper::scroll_callback(GLFWwindow *window, double xoffset, double yoffset) { renderer->ScrollCallback(window, xoffset, yoffset); }

// Callback Implementations
void Renderer::FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    this->camera.SetAspectRatio(float(width)/float(height));
}

void Renderer::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_V && action == GLFW_PRESS)
    {
        this->camera.SwitchMode(window);
        this->firstMouse = true;
        return ;
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        this->SetWindowShouldClose();
        return ;
    }
}

void Renderer::MousePosCallback(GLFWwindow* window, double xpos, double ypos)
{
    if(this->camera.GetMode() == EDIT && this->camera.GetDragState() == true)
    {
        if(this->firstMouse)
        {
            this->lastX = xpos;
            this->lastY = ypos;
            this->firstMouse = false;
        }
        float xoffset = xpos - this->lastX;
        float yoffset = this->lastY - ypos;
        this->lastX = xpos;
        this->lastY = ypos;
        
        this->camera.ProcessMouseDrag(-xoffset, -yoffset);
    }
    
    if((this->camera.GetMode() == EDIT && this->camera.GetSpinState() == true) || this->camera.GetMode() == FPS)
    {
        if(this->firstMouse)
        {
            this->lastX = xpos;
            this->lastY = ypos;
            this->firstMouse = false;
        }
        
        float xoffset = xpos - this->lastX;
        float yoffset = this->lastY - ypos;
            
        this->lastX = xpos;
        this->lastY = ypos;
            
        this->camera.ProcessMouseSpin(xoffset, yoffset);
    }
}

void Renderer::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if (this->camera.GetMode() == EDIT)
    {
        
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && this->imgui.GetWantCaptureMouse() == false)
        {
            this->camera.SetDragState(true);
        }
        else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        {
            this->camera.SetDragState(false);
            this->firstMouse = true;
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        {
            this->camera.SetSpinState(true);
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        {
            this->camera.SetSpinState(false);
            this->firstMouse = true;
        }
    }
}

void Renderer::ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    this->camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void Renderer::ProcessInput(GLFWwindow* window)
{
    if(this->camera.GetMode() == FPS)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            this->camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            this->camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            this->camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            this->camera.ProcessKeyboard(RIGHT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
            this->camera.ProcessKeyboard(DOWN, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            this->camera.ProcessKeyboard(UP, deltaTime);
    }
}


    
