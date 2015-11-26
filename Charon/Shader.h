#include "GL/glew.h"


#ifndef SHADER_INCLUDE
#define SHADER_INCLUDE


GLuint Create_shader       (const GLchar* shaderSource, GLenum shaderType);
GLuint Build_shader_program(const GLuint* shaders);


#endif 
