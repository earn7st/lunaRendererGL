#include "libs.h"

#include "ErrorHandler.h"

GLenum glCheckError_(const char* file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM : error = "GL_INVALID_ENUM"; break;
            case GL_INVALID_VALUE : error = "GL_INVALID_VALUE"; break;
            case GL_INVALID_OPERATION : error = "GL_INVALID_OPERATION"; break;
            case GL_OUT_OF_MEMORY : error = "GL_OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION : error = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
 
    return errorCode;
}
