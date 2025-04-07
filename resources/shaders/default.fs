#version 330 core
in vec3 normal;
in vec2 texcoord;
in vec3 fragPos;

out vec4 result;

uniform vec3 viewPos;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    sampler2D diffuseTex;
    sampler2D specularTex;
    float shininess;
};
uniform Material material;

struct DirectionalLight
{
    vec3 direction;
    vec3 color;
};
#define nrOfDirectionalLight 4
uniform DirectionalLight directional[nrOfDirectionalLight];

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);

void main()
{
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 norm = normalize(normal);
    
    vec3 color = vec3(0.f);
    
    for (int i = 0; i < nrOfDirectionalLight; i++)
    {
        color += CalcDirectionalLight(directional[i], norm, viewDir);
    }
    
    result = vec4(color, 1.0f);
}

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 reflectDir = reflect(-light.direction, normal);
    vec3 albedo = vec3(texture(material.diffuseTex, texcoord));
    vec3 halfwayDir = normalize(viewDir + light.direction);
    
    vec3 ambient = light.color * albedo;
    ambient = ambient * material.ambient;
    
    float diff = max(dot(normal, light.direction), 0.0);
    vec3 diffuse = light.color * diff * albedo;
    diffuse = diffuse * material.diffuse;
    
    // Phong
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // Blinn-Phong
    float spec = pow(max(dot(halfwayDir, normal), 0.0), material.shininess);
    vec3 specular = light.color * spec;
    specular = specular * material.specular;
    
    return ambient + diffuse + specular;
}
