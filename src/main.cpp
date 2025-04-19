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
#include "Model.h"

const std::string PROJECT_PATH = "/Users/earn/workspace/cg/lunaRendererGL/";

// Renderer
Renderer myRenderer("lunaRendererGL", 1920, 1080, 3, 3);

// Lights
DirectionalLight directional_light("directional", glm::vec3(-1.f, 1.0f, 1.f), glm::vec3(1.f, 1.f, 1.f));

// Primitives
Cube cube;

// Meshes
Mesh cube_mesh(cube);

// Textures
Texture woodensteel_diffuse(PROJECT_PATH + "resources/textures/container2.png", GL_TEXTURE_2D, TEXTURE_TYPE_DIFFUSE, 0);
Texture woodensteel_specular(PROJECT_PATH + "resouces/textures/container2_specular.png", GL_TEXTURE_2D, TEXTURE_TYPE_SPECULAR, 1);

// Materials
Material material(glm::vec3(0.2f), glm::vec3(1.0f), glm::vec3(1.0f), true, 0, true, 1, 128);

// Shaders
Shader dShader(PROJECT_PATH + "resources/shaders/default.vs", PROJECT_PATH + "resources/shaders/default.fs");

// Model
Model bunny(PROJECT_PATH + "resources/models/bunny/bunny.obj");
// Model spidey(PROJECT_PATH + "resources/models/ultimate_spider_man/scene.gltf");
// Model dragon(PROJECT_PATH + "resources/models/dragon_stand/dragon.obj");
//Model deva_head(PROJECT_PATH + "resources/models/2011.147_colossal_head_of_a_deva/scene.gltf");

int main()
{
    Scene scene;
    
    // Model Setting
    bunny.SetShader(&dShader);
    
    // Add to Scene
    // ------------
    // Lights
    scene.AddDirectionalLight(&directional_light);
    
    // Models
    scene.AddModel(&bunny);
    
    // Render
    myRenderer.Render(scene);
    
    return 0;
}

