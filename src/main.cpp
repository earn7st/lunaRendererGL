#include <iostream>

#include "libs.h"
#include "ErrorHandler.h"

#include "Renderer.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Light.h"

const std::string PROJECT_PATH = "/Users/earn/workspace/cg/lunaRendererGL/";

int main()
{
    
    Renderer myRenderer("lunaRendererGL", 800, 600, 3, 3);
    
    Scene scene;
    
    // Shaders
    Shader dShader(PROJECT_PATH + "resources/shaders/default.vs", PROJECT_PATH + "resources/shaders/default.fs");
    
    // Lights
    DirectionalLight directional_light("directional", glm::vec3(-1.f, 0.5f, 1.f), glm::vec3(1.f, 1.f, 1.f));
    
    // Primitives
    Cube cube;

    // Meshes
    Mesh cube_mesh(cube);
    
    // Textures
    Texture woodensteel_diffuse(PROJECT_PATH + "resources/textures/container2.png", GL_TEXTURE_2D, 0);
    Texture woodensteel_specular(PROJECT_PATH + "resouces/textures/container2_specular.png", GL_TEXTURE_2D, 1);
    
    // Materials
    Material material(glm::vec3(0.2f), glm::vec3(1.0f), glm::vec3(1.0f), 0, 1, 128);
    
    // Add to Scene
    // ------------
    // Lights
    scene.AddDirectionalLight(&directional_light);
    
    // Shaders
    scene.AddShader(&dShader);
    
    // Meshes
    scene.AddMesh(&cube_mesh);
    
    // Textures
    scene.AddTexture(&woodensteel_diffuse);
    scene.AddTexture(&woodensteel_specular);
    
    // Materials
    scene.AddMaterial(&material);
    
    // Mesh Settings
    cube_mesh.SetShader(&dShader);
    cube_mesh.SetMaterial(&material);
    
    // Render
    myRenderer.Render(scene);
    
    return 0;
}

