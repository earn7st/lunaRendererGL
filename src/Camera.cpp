#include "libs.h"
#include "camera.h"

#include <iostream>

Camera::Camera()
{
    UpdateCameraVectors();
}

Camera::Camera(const glm::vec3& position,
        const glm::vec3& front,
        const glm::vec3& up,
        const float& aspectRatio)
: position(position), front(front), up(up), aspectRatio(aspectRatio)
{
    UpdateCameraVectors();
}

Camera::Camera(const float& aspectRatio)
    : aspectRatio(aspectRatio)
{
    UpdateCameraVectors();
}

Camera_Mode Camera::GetMode() const { return mode; }
glm::mat4 Camera::GetViewMatrix() const { return glm::lookAt(position, position + front, up); }
glm::mat4 Camera::GetProjMatrix() const { return glm::perspective(glm::radians(this->FOV), this->aspectRatio, this->nearPlane, this->farPlane); }
float Camera::GetFOV() const { return FOV; }
bool Camera::GetDragState() const { return dragState; }
bool Camera::GetSpinState() const { return spinState; }

void Camera::SetAspectRatio(const float &aspectRatio) { this->aspectRatio = aspectRatio; }
void Camera::SetDragState(const bool& flag) { dragState = flag; }
void Camera::SetSpinState(const bool& flag) { spinState = flag; }

void Camera::SwitchMode(GLFWwindow* window)
{
    if (mode == EDIT)
    {
        mode = FPS;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
    else if(mode == FPS)
    {
        mode = EDIT;
        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    glm::vec3 parallel_front = glm::normalize(glm::cross(worldUp, right));
    if (direction == FORWARD)
        position += parallel_front * velocity;
    if (direction == BACKWARD)
        position -= parallel_front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;
    if (direction == UP)
        position += worldUp * velocity;
    if (direction == DOWN)
        position -= worldUp * velocity;
}

void Camera::ProcessMouseDrag(float xoffset, float yoffset)
{
    xoffset *= dragVelocity;
    yoffset *= dragVelocity;
    
    position += right * xoffset;
    position += up * yoffset;
}

void Camera::ProcessMouseSpin(double xoffset, double yoffset)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;
    
    yaw += xoffset;
    pitch += yoffset;
    
    if (constrainPitch)
    {
        if (pitch > 89.0f)
           pitch = 89.0f;
        if (pitch < -89.0f)
           pitch = -89.0f;
    }
    
    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    FOV -= yoffset;
    if (FOV < 1.0f)
        FOV = 1.0f;
    if (FOV > 45.0f)
        FOV = 45.0f;
    
    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    
    front = normalize(direction);
    
    right = glm::normalize(glm::cross(front, worldUp));
    up    = glm::normalize(glm::cross(right, front));
}
