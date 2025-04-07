#ifndef _MODEL_H_
#define _MODEL_H_

#include <vector>

#include "libs.h"

#include "Mesh.h"
#include "Texture.h"

class Model
{
public:
    
    Model(std::string const &path);
    ~Model() = default;
    
    std::vector<Mesh> meshes;
    std::vector<Texture> textures;
    
private:
    
};

#endif
