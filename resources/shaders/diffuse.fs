#version 330 core
in vec3 normal;
in vec2 texcoord;
in vec3 fragPos;
in vec4 lightSpacePos;

out vec4 result;

uniform vec3 viewPos;
uniform sampler2D shadowMap;

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
#define nrOfDirectionalLight 2
uniform DirectionalLight directional[nrOfDirectionalLight];

#define EPS 1e-3
#define PI 3.141592653589793
#define PI2 6.283185307179586

// Shadow
#define NUM_SAMPLES 40
#define NUM_BLOCKER_SAMPLES NUM_SAMPLES
#define NUM_PCF_SAMPLES NUM_SAMPLES
#define NUM_RINGS 5

#define FRUSTUM_SIZE 20.
#define SHADOW_MAP_SIZE 1024.
#define NONE_BIAS_C 0.2
#define PCF_BIAS_C 0.6

vec2 poissonDisk[NUM_SAMPLES];
void poissonDiskSamples(const in vec2 randomSeed);
float rand_1to1(float x);
float rand_2to1(vec2 uv);

float getShadowBias(float biasC, float filterRadius);
float sampleShadow(sampler2D shadowMap, vec3 coords, float biasC, float filterRadius);
float PCF(sampler2D shadowMap, vec3 coords, float biasC, float filterRadius);
// float PCSS(sampler2D shadowMap, vec3 coords, float biasC, float filterRadius);

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
    
    // Shadow
    vec3 shadowCoords = lightSpacePos.xyz / lightSpacePos.w;
    shadowCoords =  (shadowCoords + 1) / 2;
    float visibility = 1.;
    
    //visibility = sampleShadow(shadowMap, shadowCoords, NONE_BIAS_C, 0.);
    visibility = PCF(shadowMap, shadowCoords, PCF_BIAS_C, 0.005);
    
    result = vec4(color * visibility, 1.0f);
    
}

float rand_1to1(float x) {
  // -1 -1
  return fract(sin(x)*10000.0);
}

float rand_2to1(vec2 uv) {
  // 0 - 1
    const highp float a = 12.9898, b = 78.233, c = 43758.5453;
    highp float dt = dot( uv.xy, vec2( a,b ) ), sn = mod( dt, PI );
    return fract(sin(sn) * c);
}

void poissonDiskSamples(const in vec2 randomSeed)
{
  float ANGLE_STEP = PI2 * float( NUM_RINGS ) / float( NUM_SAMPLES );
  float INV_NUM_SAMPLES = 1.0 / float( NUM_SAMPLES );

  float angle = rand_2to1( randomSeed ) * PI2;
  float radius = INV_NUM_SAMPLES;
  float radiusStep = radius;

  for( int i = 0; i < NUM_SAMPLES; i ++ ) {
    poissonDisk[i] = vec2( cos( angle ), sin( angle ) ) * pow( radius, 0.75 );
    radius += radiusStep;
    angle += ANGLE_STEP;
  }
}

float getShadowBias(float biasC, float filterRadius)
{

  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(directional[0].direction);
  
  float A = (1.0 + ceil(filterRadius)) * (FRUSTUM_SIZE / SHADOW_MAP_SIZE / 2.0);
  float B = 1. - dot(lightDir, norm);

  return max(A, A * B) * biasC;
}

float sampleShadow(sampler2D shadowMap, vec3 coords, float biasC, float filterRadius)
{
    float bias = getShadowBias(biasC, filterRadius);
    float cur_depth = coords.z;
    float map_depth = vec3(texture(shadowMap, coords.xy)).r;
    if(cur_depth - bias > map_depth + EPS)
    {
        return 0.;
    }
    return 1.;
}

float PCF(sampler2D shadowMap, vec3 coords, float biasC, float filterRadius)
{
    poissonDiskSamples(coords.xy);
    
    float visibility = 0.;
    
    for (int i = 0; i < NUM_SAMPLES; i++)
    {
        vec2 offset = poissonDisk[i] * filterRadius;
        visibility += sampleShadow(shadowMap, coords + vec3(offset, 0.), biasC, filterRadius);
    }
    
    return visibility / float(NUM_SAMPLES);
}

float PCSS(sampler2D shadowMap, vec3 coords, float biasC, float filterRadius)
{
    // 1: Average Blocker Depth
    
    
    // 2:
    return 1.;
}

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 reflectDir = reflect(-light.direction, normal);
    
    vec3 albedo = vec3(texture(material.diffuseTex, texcoord)) * 0.6;
    
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
    // specular *= vec3(texture(material.specularTex, texcoord));
    specular = specular * material.specular;
    
    return ambient + diffuse + specular;
}
