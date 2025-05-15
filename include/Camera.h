#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Shader.h"

enum Camera_Mode
{
    EDIT,
    FPS
};

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera
{
public:
    
    Camera();
    Camera(const glm::vec3& position,
           const glm::vec3& front,
           const glm::vec3& up,
           const float& aspectRatio);
    Camera(const float& aspectRatio);
    
    Camera_Mode GetMode() const;
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjMatrix() const;
    float GetFOV() const;
    
    bool GetDragState() const;
    bool GetSpinState() const;
    
    void SetAspectRatio(const float& aspectRatio);
    void SetDragState(const bool& flag);
    void SetSpinState(const bool& flag);
    
    void SwitchMode(GLFWwindow* window);
     
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseDrag(float xoffset, float yoffset);
    void ProcessMouseSpin(double xoffset, double yoffset);
    void ProcessMouseScroll(float yoffset);
    
    void SendPositionToShader(Shader* shader) const;
    
private:
    Camera_Mode mode = EDIT;
    
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 front    = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right;
    const glm::vec3 worldUp  = glm::vec3(0.0f, 1.0f, 0.0f);
    
    float nearPlane = 0.1f;
    float farPlane = 50.0f;
    
    float yaw   = -90.0f;
    float pitch =   0.0f;
    float roll  =   0.0f;
    
    float FOV   = 60.0f;
    float aspectRatio;      // ratio of SCR_WIDTH/SCR_HEIGHT
    bool constrainPitch = true;
    
    float mouseSensitivity = 0.2;
    
    //EDIT
    bool spinState = false;
    bool dragState = false;
    float dragVelocity = 0.005f;
    
    // FPS
    float movementSpeed;
    float velocity = 0.05f;
    
    void UpdateCameraVectors();
};

#endif
