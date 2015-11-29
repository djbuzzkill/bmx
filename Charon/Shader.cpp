#include "Shader.h"
#include "Charon.h"
#include <boost/assert.hpp>
#include <stdio.h>
#include <string.h>


using namespace Rn; 

AttributeMap& Attribute_locations(
   AttributeMap&  attrib,
   AttributeDef*  defs,
   size_t         count,
   GLuint         progID)
{
   attrib.clear ();

   for (int i = 0; i < count; i++)
   {
      attrib[defs[i].var] = glGetAttribLocation (progID, defs[i].var.c_str());
   }

   return attrib; 
}

UniformMap& Uniform_locations(
   Rn::UniformMap&    uniforms,
   UniformDef*    defs,
   size_t         count,
   GLuint         progID)
{
   uniforms.clear();


   for (int i = 0; i < count; i++)
   {  
      uniforms[defs[i].var] = glGetUniformLocation(progID, defs[i].var.c_str());
   }
   return uniforms;
}


//
void Update_uniforms(
   const UniformMap&       unifloc,
   const UniformValueMap&  values,
   const UniformDef*       unifdef,
   int                     unifcount)
{
   for (int i = 0; i < unifcount; i++) 
   {
      switch (unifdef[i].type)
      {
      case UT_VEC2F: 
         glUniform2fv (unifloc.at(unifdef[i].var), unifdef[i].count, (GLfloat*)values.at(unifdef[i].var).p);
         break;
      case UT_VEC3F:
         glUniform3fv (unifloc.at(unifdef[i].var), unifdef[i].count, (GLfloat*)values.at(unifdef[i].var).p);
         break; 
      case UT_VEC4F: 
         glUniform4fv (unifloc.at(unifdef[i].var), unifdef[i].count, (GLfloat*)values.at(unifdef[i].var).p);
         break; 
     case UT_MAT3F:
        glUniformMatrix3fv (unifloc.at(unifdef[i].var), unifdef[i].count, GL_FALSE, (GLfloat*)values.at(unifdef[i].var).p);
         break;
      case UT_MAT4F:
         glUniformMatrix4fv (unifloc.at(unifdef[i].var), unifdef[i].count, GL_FALSE, (GLfloat*)values.at(unifdef[i].var).p);
         break;
      case UT_SAMPLER: 
         glUniform1i (unifloc.at(unifdef[i].var), values.at(unifdef[i].var).i);
         break;
      }      
      Validate_GL_call();
   }
}

void Update_uniform (
   const Rn::UniformMap&       unifloc,
   const Rn::UniformValueMap&  values,
   const UniformDef&           unifdef)
{
   Update_uniforms (unifloc, values, &unifdef, 1); 
}


GLuint Create_shader(const GLchar* shaderSource, GLenum shaderType)
{
   BOOST_ASSERT(shaderSource);

   printf("\n Create_shader ()\n\n%s", shaderSource);


   bool valid_res = false;

   const int valid_shader_tpes[] = {
      GL_VERTEX_SHADER,
      GL_FRAGMENT_SHADER,
      GL_TESS_CONTROL_SHADER,
      GL_TESS_EVALUATION_SHADER,
      GL_GEOMETRY_SHADER
   };

   for (int i = 0; i < 5; i++)
      valid_res |= shaderType == valid_shader_tpes[i];

   BOOST_ASSERT(valid_res);

   GLuint shaderID = glCreateShader(shaderType);

   const GLchar* source[] = {
      shaderSource,
      0
   };

   GLint len_source[] = {
      strlen(shaderSource) - 1,
      0
   };

   glShaderSource(shaderID, 1, source, len_source);

   glCompileShader(shaderID);

   int compileStatus;
   glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
   if (compileStatus != GL_TRUE)
   {
      GLchar err_buf[1024];
      GLsizei infolen;
      glGetShaderInfoLog(shaderID, 1024, &infolen, err_buf);

      printf("\n%s\n", err_buf);
      //Debug ("Shader Error:\n %s", output);
      //BreakAssert (0, "shader compile fail");
      glDeleteShader(shaderID);
      BOOST_ASSERT(0);
      return 0;
   }

   return shaderID;

}

GLuint Build_shader_program(const GLuint* shaders)
{
   GLuint progID = glCreateProgram();


   int count_shaders = 0;
   while (shaders[count_shaders])
   {
      Validate_GL_call();
      glAttachShader(progID, shaders[count_shaders]);
      count_shaders++;
   }

   // 
   // 
   glLinkProgram(progID);
   Validate_GL_call();

   GLint    linkRes;
   GLsizei  bufflen;
   char     outputbuffer[1024];
   glGetProgramiv(progID, GL_LINK_STATUS, &linkRes);
   if (linkRes == GL_FALSE)
   {
      glGetProgramInfoLog(progID, 1024, &bufflen, outputbuffer);

      printf("\nBuild_shader_program() : Shader Linking Failed: %s\n", outputbuffer);
      //Debug ( "\nGL Info Log :  \n\n %s\n", outputbuffer);
      //Assert (0, "ShaderProg::Link () - Failed");
      glDeleteProgram(progID);
      return 0;
   }

   return progID;
}
