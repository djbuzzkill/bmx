#include "OpenGL_system.h"

#include <stdlib.h>
#include <string.h>

#include "stdio.h"
#include "Dx/Utility.h"
#include <thread>

namespace sy
{
   //
   class OpenGL_system_impl : public OpenGL_system 
      { 
   public: 
      //
   OpenGL_system_impl (); 
   virtual ~OpenGL_system_impl (); 

   // ez fns
   virtual GLuint Create_shader        (const GLchar* shaderSource, GLenum shaderType);
   virtual GLuint Build_shader_program (const GLuint* shaders);
   virtual void   Validate_GL_call     ();

   // pure
   virtual void   UseProgram           (GLuint progID);
   virtual void   Clear                (GLbitfield clear_flags); 
   virtual void   UniformMatrix4fv     (GLint loc, GLsizei count, GLboolean transpose, const GLfloat* dat); 
   virtual void   Uniform1i            (GLint location, GLint i); 
   virtual void   Uniform4fv           (GLint location, GLsizei count, const GLfloat* value); 
      };




   OpenGL_system_impl::OpenGL_system_impl ()
   {
   }

   OpenGL_system_impl::~OpenGL_system_impl ()
   {
   }

   void OpenGL_system_impl::Validate_GL_call ()
   {
   int wat = 0; 
   GLenum         err      = 0; 
   const GLubyte* error_s_ = 0; 
      
   err = glGetError ();
   if (err != GL_NO_ERROR )
   {
       
      error_s_  = glewGetErrorString (err);
      // BOOST_ASSERT (0); 
   }
   wat++; 
   }

   void OpenGL_system_impl::UniformMatrix4fv (GLint loc, GLsizei count, GLboolean transpose, const GLfloat* dat)
   {
      glUniformMatrix4fv (loc, count, transpose, dat); 
   } 

   void OpenGL_system_impl::Uniform4fv(GLint location, GLsizei count, const GLfloat* value)
   {  
      glUniform4fv (location, count, value); 
   } 
   //
   void OpenGL_system_impl::Uniform1i (GLint location, GLint v0)
   {



      glUniform1i (location, v0); 
   } 

   //
   void OpenGL_system_impl::UseProgram (GLuint progID)
   {
      glUseProgram(progID); 
   }

   //
   void OpenGL_system_impl::Clear (GLbitfield clear_flags)
   {
      glClear (clear_flags); 
   }

   //
   GLuint OpenGL_system_impl::Create_shader (const GLchar* shaderSource, GLenum shaderType)
   {
      const GLint valid_shader_types[] = { 
         GL_VERTEX_SHADER  , 
         GL_FRAGMENT_SHADER,
         GL_TESS_CONTROL_SHADER, 
         GL_TESS_EVALUATION_SHADER
         }; 

      if (std::count (valid_shader_types, valid_shader_types + 4, shaderType)) 
      {

         GLuint shaderID = glCreateShader (shaderType); 

         const GLchar* source[] = {
            shaderSource, 
            0   
            }; 

         GLint len_source[] = { 
            strlen (shaderSource) - 1, 
            0
            }; 
      
         glShaderSource (shaderID, 1, source, len_source); 

         glCompileShader(shaderID); 

         int compileStatus;
         glGetShaderiv (shaderID, GL_COMPILE_STATUS, &compileStatus);
         if (compileStatus != GL_TRUE) 
         {
	         GLchar err_buf[1024];
	         GLsizei infolen;
	         glGetShaderInfoLog (shaderID, 1024, &infolen, err_buf);
	         //Debug ("Shader Error:\n %s", output);
	         //BreakAssert (0, "shader compile fail");

            DX_ASSERT (0, "\ncompile failed: %s", err_buf); 

            glDeleteShader(shaderID); 
         //      BOOST_ASSERT (0); 
	         return 0;
         }

         return shaderID; 
      }

      return 0; 
   }



   GLuint OpenGL_system_impl::Build_shader_program (const GLuint* shaders)
   {
	   GLuint progID = glCreateProgram (); 

	   int count_shaders = 0; 
	   while (shaders[count_shaders]) 
	   {
		  glAttachShader (progID, shaders[count_shaders]); 
		  count_shaders++;
	   }   

	   // 
	   // 
	   glLinkProgram (progID);

	   GLint    linkRes;
	   GLsizei  bufflen;
	   char     outputbuffer[1024];
	   glGetProgramiv (progID, GL_LINK_STATUS, &linkRes);
	   if (linkRes == GL_FALSE)
	   {
		   glGetProgramInfoLog (progID, 1024, &bufflen, outputbuffer);
		   //Debug ( "\nGL Info Log :  \n\n %s\n", outputbuffer);
      
		  printf ("\n%s", outputbuffer); 
		   //Assert (0, "ShaderProg::Link () - Failed");
		  glDeleteProgram (progID); 
		   return 0;
	   }

	   return progID; 
   }

   //
   //
   OpenGL_system* Create_OpenGL_system ()
   {    
      return new OpenGL_system_impl ;
   }


   void Load_uniform_matrix (GLuint uniformID, float* mat) 
   {

   }
   
   void Load_uniform_vector (GLuint uniformID, float* mat)
   {
   } 
   
   void Load_uniform_texture (GLuint uniformID, unsigned int)
   {
   }   

   void Load_uniform_sampler (GLuint uniformID, unsigned int)
   {
   } 

}

