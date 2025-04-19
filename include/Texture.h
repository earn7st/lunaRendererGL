#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "libs.h"

enum TEXTURE_TYPE{
    TEXTURE_TYPE_DIFFUSE = 0,
    TEXTURE_TYPE_SPECULAR = 1
};

class Texture
{
private:
    unsigned int ID;
    GLenum GL_type;
    TEXTURE_TYPE type;
    int textureUnit = 0;
    int width, height, BPP;
    std::string directory;
    
    
public:
    Texture() {};
    Texture(const std::string& path, GLenum GL_type, TEXTURE_TYPE type, int textureUnit);
    Texture(const std::string& path, GLenum GL_type, TEXTURE_TYPE type);
    ~Texture() = default;
    
    inline unsigned int GetID() const { return this->ID; }
    
    // Modifiers
    void SetTextureType(TEXTURE_TYPE type);
    
    void Bind() const;
    void Unbind() const;
};

#endif
