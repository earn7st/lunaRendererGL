#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "libs.h"
class Texture
{
private:
    unsigned int ID;
    GLenum type;
    int textureUnit;
    int width, height, BPP;
    
    
public:
    Texture(const std::string& path, GLenum type, int textureUnit);
    ~Texture();
    
    inline unsigned int GetID() const { return this->ID; }
    
    void Bind() const;
    void Unbind() const;
};

#endif
