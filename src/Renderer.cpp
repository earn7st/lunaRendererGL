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

// Functions
void Renderer::ProcessInput(GLFWwindow* window)
{
    if(camera.GetMode() == FPS)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
            camera.ProcessKeyboard(DOWN, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera.ProcessKeyboard(UP, deltaTime);
    }
}

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
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        ProcessInput(this->window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            this->imgui.Sleep();
            continue;
        }
        
        this->imgui.NewFrame();
        
        this->imgui.DrawCameraWidget(&this->camera);
        
        // Draw Models in this Scene
        for (int i = 0; i < scene.models.size(); i++)
        {
            
            Model &model = *scene.models[i];
            
            scene.SendLightsToShader(model.GetShader());
            
            model.Draw(this->camera);
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
        */
        
        glfwSwapBuffers(window);
    }
    
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
    
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CCW);
    
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
    camera.SetAspectRatio(float(width)/float(height));
}

void Renderer::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_V && action == GLFW_PRESS)
    {
        camera.SwitchMode(window);
        firstMouse = true;
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
    if(camera.GetMode() == EDIT && camera.GetDragState() == true)
    {
        if(firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;
        
        camera.ProcessMouseDrag(xoffset, yoffset);
    }
    
    if((camera.GetMode() == EDIT && camera.GetSpinState() == true) || camera.GetMode() == FPS)
    {
        if(firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
            
        lastX = xpos;
        lastY = ypos;
            
        camera.ProcessMouseSpin(xoffset, yoffset);
    }
}

void Renderer::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if (camera.GetMode() == EDIT)
    {
        /*
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            camera.SetDragState(true);
        }
        else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        {
            camera.SetDragState(false);
            firstMouse = true;
        }
        */
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        {
            camera.SetSpinState(true);
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        {
            camera.SetSpinState(false);
            firstMouse = true;
        }
    }
}

void Renderer::ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}



