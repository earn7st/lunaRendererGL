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
DirectionalLight directional_light("directional", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0, 5, 10), glm::vec3(0.), glm::vec3(0, 1, 0));

// Shaders
Shader shadowMapShader(PROJECT_PATH + "resources/shaders/shadowMap.vs", PROJECT_PATH + "resources/shaders/shadowMap.fs");
Shader diffuseShader(PROJECT_PATH + "resources/shaders/diffuse.vs", PROJECT_PATH + "resources/shaders/diffuse.fs");
Shader singleColorShader(PROJECT_PATH + "resources/shaders/singleColor.vs", PROJECT_PATH + "resources/shaders/singleColor.fs");

// Model
Model bunny(PROJECT_PATH + "resources/models/bunny/bunny.obj");
Model checker_floor(PROJECT_PATH + "resources/models/checkered_tile_floor/scene.gltf");
//Model dragon(PROJECT_PATH + "resources/models/dragon_stand/dragon.obj");

int main()
{
    Scene scene;
    
    // Model Settings
    bunny.SetShader(&singleColorShader);
    bunny.Translate(glm::vec3(-1, 0, 0));
    
    /*
    dragon.SetShader(&singleColorShader);
    dragon.Translate(glm::vec3(1, 0.5, 0));
    dragon.Rotate(glm::vec3(0, 90.0, 0));
    dragon.Scale(glm::vec3(2.0, 2.0, 2.0));
    */
     
    checker_floor.SetShader(&diffuseShader);
    checker_floor.Scale(glm::vec3(0.05));
    checker_floor.Rotate(glm::vec3(-90.0, 0.0, 0.0));
    checker_floor.Translate(glm::vec3(0.0, -0.5, 0.0));
    
    // Add to Scene
    // ------------
    // Lights
    scene.AddDirectionalLight(&directional_light);
   
    // Models
    scene.AddModel(&bunny);
    scene.AddModel(&checker_floor);
    //scene.AddModel(&dragon);
    
    // Render
    myRenderer.SetShadowMapShader(shadowMapShader);
    myRenderer.Render(scene);
    
    return 0;
}

