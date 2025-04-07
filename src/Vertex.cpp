#include "libs.h"
#include "ErrorHandler.h"

#include "Vertex.h"

Vertex::Vertex(const float* data)
{
    this->position = glm::vec3(data[0], data[1], data[2]);
    this->normal = glm::vec3(data[3], data[4], data[5]);
    this->texcoord = glm::vec2(data[6], data[7]);
}
