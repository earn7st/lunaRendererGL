#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "libs.h"
#include "ErrorHandler.h"

#include "Texture.h"

Texture::Texture(const std::string& path, GLenum type, int textureUnit)
: ID(0), type(type), textureUnit(textureUnit), width(0), height(0), BPP(0)
{
    glCall(glGenTextures(1, &ID));
    glCall(glBindTexture(type, ID));
    
    glCall(glTexParameterf(type, GL_TEXTURE_WRAP_S, GL_REPEAT));
    glCall(glTexParameterf(type, GL_TEXTURE_WRAP_T, GL_REPEAT));
    glCall(glTexParameterf(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    glCall(glTexParameterf(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    
    
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &BPP, 3);
    
    if(data)
    {
        glCall(glTexImage2D(type, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
        glCall(glGenerateMipmap(type));
        
        
    }
    stbi_image_free(data);
    
    glCall(glBindTexture(type, 0));
    
}

Texture::~Texture()
{
    glCall(glDeleteTextures(1, &ID));
}

void Texture::Bind() const
{
    glCall(glActiveTexture(GL_TEXTURE0 + this->textureUnit));
    glCall(glBindTexture(this->type, ID));
}

void Texture::Unbind() const
{
    glCall(glActiveTexture(0));
    glCall(glBindTexture(this->type, 0));
}

