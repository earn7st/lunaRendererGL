#ifndef _SHADER_H_
#define _SHADER_H_

#include "libs.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
private:
    unsigned int ID;
    void checkCompileErrors(unsigned int shader, std::string type);
    
public:

    Shader(const std::string vertexPath, const std::string fragmentPath);
    ~Shader();
    
    void Use() const;
    
    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;
    void SetVec2(const std::string &name, float x, float y) const;
    void SetVec2(const std::string &name, const glm::vec2 &vec) const;
    void SetVec3(const std::string &name, float x, float y, float z) const;
    void SetVec3(const std::string &name, const glm::vec3 &vec) const;
    void SetVec4(const std::string &name, float x, float y, float z, float w) const;
    void SetVec4(const std::string &name, const glm::vec4 &vec) const;
    void SetMat3(const std::string &name, const glm::mat3 &mat) const;
    void SetMat4(const std::string &name, const glm::mat4 &mat) const;

};
#endif
