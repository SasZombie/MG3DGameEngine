///@brief
///Read me [1]
#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wshadow"

#include <GL/glew.h>

#pragma GCC diagnostic pop

//In my opinion these custom define types that just redifine a type
//Are very annoying
GLuint loadBMP(const char * imagepath);
