#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 normal;
out vec2 texcoord;
out vec3 fragPos;
out vec4 lightSpacePos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform mat4 lightVP;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    fragPos = mat3(model) * aPos;
    normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    texcoord = aTexCoord;
    lightSpacePos = lightVP * model * vec4(aPos, 1.0);
}
