#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "libs.h"
#include "ErrorHandler.h"

#include "Texture.h"

Texture::Texture(const std::string& path, GLenum GL_type, TEXTURE_TYPE type, int textureUnit)
: directory(path), ID(0), GL_type(GL_type), type(type), textureUnit(textureUnit), width(0), height(0), BPP(0)
{
    glCall(glGenTextures(1, &ID));
    glCall(glBindTexture(GL_type, ID));
    
    glCall(glTexParameterf(GL_type, GL_TEXTURE_WRAP_S, GL_REPEAT));
    glCall(glTexParameterf(GL_type, GL_TEXTURE_WRAP_T, GL_REPEAT));
    glCall(glTexParameterf(GL_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    glCall(glTexParameterf(GL_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    
    
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &BPP, 3);
    
    if(data)
    {
        glCall(glTexImage2D(GL_type, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
        glCall(glGenerateMipmap(GL_type));
    }
    stbi_image_free(data);
    
    glCall(glBindTexture(GL_type, 0));
    
}

Texture::Texture(const std::string& path, GLenum GL_type, TEXTURE_TYPE type)
: directory(path), ID(0), GL_type(GL_type), type(type), width(0), height(0), BPP(0)
{
    glCall(glGenTextures(1, &ID));
    glCall(glBindTexture(GL_type, ID));
    
    glCall(glTexParameterf(GL_type, GL_TEXTURE_WRAP_S, GL_REPEAT));
    glCall(glTexParameterf(GL_type, GL_TEXTURE_WRAP_T, GL_REPEAT));
    glCall(glTexParameterf(GL_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    glCall(glTexParameterf(GL_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    
    
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &BPP, 3);
    
    if(data)
    {
        glCall(glTexImage2D(GL_type, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
        glCall(glGenerateMipmap(GL_type));
    }
    stbi_image_free(data);
    
    glCall(glBindTexture(GL_type, 0));
}

// Modifiers
void Texture::SetTextureType(TEXTURE_TYPE type) { this->type = type; }

// Bind
void Texture::Bind() const
{
    if (this->textureUnit == -1)
    {
        std::cout << "Texture::Bind : No Valid textureUnit for Texture" << this->directory << std::endl;
        return ;
    }
    glCall(glActiveTexture(GL_TEXTURE0 + this->textureUnit));
    glCall(glBindTexture(this->GL_type, ID));
}

void Texture::Unbind() const
{
    glCall(glActiveTexture(0));
    glCall(glBindTexture(this->GL_type, 0));
}

