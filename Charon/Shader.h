#include "GL/glew.h"
#include "Dx/Render.h"

#ifndef SHADER_INCLUDE
#define SHADER_INCLUDE



enum UniformType
{
   _INVALID_UniformType_ = 0,

   UT_VEC2F,
   UT_VEC3F,
   UT_VEC4F,

   UT_MAT3F,
   UT_MAT4F,

   UT_SAMPLER
};

enum AttributeType
{
   _INVALID_AttributeType = 0,
   AT_FLOAT,
   AT_VEC2F,
   AT_VEC3F,

};

struct UniformDef
{
   std::string var;
   UniformType type;
   size_t      count;
   void*       ptr;
};

struct AttributeDef
{
   std::string    var;
   AttributeType  type;
   void*          ptr;
};


Rn::AttributeMap& Attribute_locations(
   Rn::AttributeMap&  attrib,
   AttributeDef*  defs,
   size_t         count,
   GLuint         progID
   );
Rn::UniformMap& Uniform_locations(
   Rn::UniformMap&    uniforms,
   UniformDef*    defs,
   size_t         count,
   GLuint         progID
   );


GLuint Create_shader       (const GLchar* shaderSource, GLenum shaderType);
GLuint Build_shader_program(const GLuint* shaders);


#endif 
