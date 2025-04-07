#include <iostream>

#include "libs.h"
#include "ErrorHandler.h"

#include "Primitives.h"

void Primitive::Set(const float* data, const int& data_count)
{
    for (size_t i = 0; i < data_count; i += 8)
    {
        Vertex vertex(data + i);
        this->vertices.push_back(vertex);
    }
}

void Primitive::Set(const float* data, const int& data_count, const GLint* indices, const int& indices_count)
{
    for (size_t i = 0; i < data_count; i += 8)
    {
        Vertex vertex(data + i);
        this->vertices.push_back(vertex);
    }
    for (size_t i = 0; i < indices_count; i ++)
    {
        this->indices.push_back(indices[i]);
    }
}
