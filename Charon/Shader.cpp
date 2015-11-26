#include "Shader.h"
#include "Charon.h"
#include <boost/assert.hpp>
#include <stdio.h>
#include <string.h>

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
