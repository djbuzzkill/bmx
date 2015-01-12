#ifndef OPENGL_SYSTEM_H
#define OPENGL_SYSTEM_H

#include<GL/glew.h>
#include<Dx/System.h>

namespace sy
{ 
   //enum PipelineStage 
   //{
   //   _INVALID_PIPELINE_STAGE_ = -1, 
   //   PS_Vertex, 
   //   PS_TesselationControl, 
   //   PS_TesselationEvalution, 
   //   PS_Geometry, 
   //   PS_Fragment
   //}; 
   //
   //class ShaderPipelineDef 
   //{
   //
   //};


   class OpenGL_system : public cx::Destructor
      { 
   public: 
      virtual GLuint   Create_shader        (const GLchar* shaderSource, GLenum shaderType) = 0;
      virtual GLuint   Build_shader_program (const GLuint* shaders) = 0;
      virtual void     Validate_GL_call     () = 0;

      virtual void   other_opengl_shit () = 0;

      };

   OpenGL_system* Create_OpenGL_system ();  


   void Load_uniform_matrix   (GLuint uniformID, float* ); 
   void Load_uniform_vector   (GLuint uniformID, float* ); 
   void Load_uniform_texture  (GLuint uniformID, unsigned int); 
   void Load_uniform_sampler  (GLuint uniformID, unsigned int); 



}

#endif
