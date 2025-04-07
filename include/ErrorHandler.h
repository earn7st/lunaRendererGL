#ifndef _ERRORHANDLER_H_
#define _ERRORHANDLER_H_

#include "libs.h"

#define glCall(x) x;\
    glCheckError_(__FILE__, __LINE__)

GLenum glCheckError_(const char* file, int line);

#endif
