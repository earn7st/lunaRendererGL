#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <iostream>

#include "libs.h"

#include "MyImGui.h"
#include "Camera.h"
#include "Scene.h"

extern const std::string PROJECT_PATH;

class Renderer
{
public:
// Constructors & Detructors
    Renderer(
            const char* title,
            const int& WINDOW_WIDTH,
            const int& WINDOW_HEIGHT,
            const int& GL_VERSION_MAJOR,
            const int& GL_VERSION_MINOR);
    virtual ~Renderer();
    
// Accessors
    int GetWindowShouldClose();
    
// Modifiers
    void SetWindowShouldClose();
    
// Functions
    void ProcessInput(GLFWwindow *window);
    
    void Render(Scene &scene);
    
// Static functions
private:
//Variables
    // Window
    GLFWwindow* window;
    const int WINDOW_WIDTH;
    const int WINDOW_HEIGHT;
    int framebufferWidth;
    int framebufferHeight;
    bool resizable = true;
    
    //OpenGL Context
    const int GL_VERSION_MAJOR;
    const int GL_VERSION_MINOR;
    
    // Camera
    Camera camera;
    
    // Input Settings
    float lastX;
    float lastY;
    bool firstMouse = true;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    
    // ImGui
    MyImGui imgui;
    const char* glsl_version = "#version 150";
    
    // Callback Wrapper
    class CallbackWrapper
    {
    public:
        CallbackWrapper() = delete;
        CallbackWrapper(const CallbackWrapper&) = delete;
        CallbackWrapper(CallbackWrapper&&) = delete;
        ~CallbackWrapper() = delete;
        
        static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos);
        static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
        static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
        
        static void SetRenderer(Renderer *renderer);
        
    private:
        static Renderer* renderer;
    };

// Private Functions
    // Inits
    void InitGLFW();
    void InitWindow(const char* title);
    void InitGLAD();
    void InitOpenGLOptions();
    void InitCamera();
    void InitCallbacks();
    void InitImGui();
    
    // Callback Implementations
    void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void MousePosCallback(GLFWwindow* window, double xpos, double ypos);
    void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    
// Static Variables
    
};

#endif
