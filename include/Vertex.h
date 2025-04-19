#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "libs.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoord;
    
    Vertex(glm::vec3 &position, glm::vec3 &normal, glm::vec2 &texcoord)
    : position(position), normal(normal), texcoord(texcoord) { };
    Vertex(const float* data);
};
#endif
    
