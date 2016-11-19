#include "Render.h"


#include "GL/glew.h"

namespace Rn 
{
//   const int va_gl_type[] = {
//	   GL_FLOAT,  
//      GL_FLOAT, 
//      GL_FLOAT, 
//	   GL_BYTE, 
//      GL_BYTE, 
//	   GL_FLOAT,  
//      GL_FLOAT, 
//	   GL_BYTE,  
//      GL_FLOAT,
//	   GL_INT
//	   };

// bool CreateIndexBuffer (IndexBuffer::Params& params, IndexBuffer** ib) {
// 	*ib = 0; 
// 	*ib = new IndexBuffer (params.indexTyp, params.usage, params.numElems, params.source); 
// 	return (*ib) ? true : false; 
// 	}
// 
// 
// void SetIndexBuffer (IndexBuffer& ib) {
// 	//BreakAssert (ib, "SetIndexBuffer() : invalid pointer\n");
// 	//
// 	//sgCurrentIndexBuffer = ib;
// 	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ib.ID);
// 	}
// 
// 
// 
// 
// Texture2D::Texture2D (const Texture2D::Params& params)
// : fmt (params.format), wd (params.width), ht (params.height), ID (0)
// {
// 	glGenTextures (1, &ID);
// 
// 	glBindTexture (GL_TEXTURE_2D, ID);
// 
// 	glTexImage2D (
// 		GL_TEXTURE_2D, 
// 		0, 
// 		gl_intrnal_format[fmt], 
// 		wd, ht, 0, 
// 		gl_texture_format[fmt], 
// 		gl_data_type[fmt],
// 		params.source 
// 		);
// 
// 	BreakOnGLError_();
// }
// 
// //
// Texture2D::Texture2D (Texture::Format format, unsigned width, unsigned height, const void* source) 
// 	: fmt (format), wd (width), ht (height), ID (0)
// {
// 	glGenTextures (1, &ID);
// 
// 	glBindTexture (GL_TEXTURE_2D, ID);
// 
// 	glTexImage2D (
// 		GL_TEXTURE_2D, 
// 		0, 
// 		gl_intrnal_format[format], 
// 		width, height, 0, 
// 		gl_texture_format[format], 
// 		gl_data_type[format],
// 		source 
// 	);
// 
// 	BreakOnGLError_();
// } 
// 
// //
// void BindIndexBuffer (IndexBuffer* ib) {
// 	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ib ? ib->ID : 0);
// 	}
// 
// //
// void BindVertexBuffer (VertexBuffer* vb) {
// 	glBindBuffer (GL_ARRAY_BUFFER, vb->ID);
// }
// 
// 
// 
// 
// 
// void SetTextureStage2D (unsigned stage, Texture2D* txr) {
// 	//Assert (sgCurrentShaderProgram, "");
// 	//Assert(stage < MAX_SAMPLER_UNITS, "sampler stage out of range");
// 	//Assert(GL_TRUE == glIsTexture (txr->ID), "not a texture?");
// 
// 	glActiveTexture (GL_TEXTURE0 + stage);
// 	glEnable (GL_TEXTURE_2D);
// 	glBindTexture (GL_TEXTURE_2D, txr->ID); 
// 
// 	BreakOnGLError_();
// 
// 	}
//

   //
   void Validate_GL_call ()
   {
      int wat = 0; 
      GLenum         err      = 0; 
      const GLubyte* error_s_ = 0; 
      
      err = glGetError ();
      if (err != GL_NO_ERROR )
      {
         error_s_  = glewGetErrorString (err);
//         DX_ASSERT (0, "validation failed"); 
      }

      wat++; 
   }

   //
   GLuint Create_shader (const GLchar* shaderSource, GLenum shaderType)
   {
//      DX_ASSERT (shaderSource, "invalid shader source"); 
//      DX_ASSERT (shaderType == GL_VERTEX_SHADER  || shaderType == GL_FRAGMENT_SHADER, "must be a VS or FS (for now)"); 

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
         glDeleteShader(shaderID); 
//         DX_ASSERT (0, "compile failed"); 
		   return 0;
	   }

      return shaderID; 

   }

   //
   GLuint Build_shader_program (const GLuint* shaders)
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
		   //Assert (0, "ShaderProg::Link () - Failed");
         glDeleteProgram (progID); 
		   return 0;
	   }

      return progID; 
   }



}


