

#include "Terrain_renderer.h"


// #include <FileReader.h>

GLenum GL_TEXTURE_stage (int stage)
{
   BOOST_ASSERT (stage >= 0); 
   BOOST_ASSERT (stage < 16); 

   return GL_TEXTURE0 + stage; 
}

// Temporary: for now we will only do this many
const int k_MaxDetailLevelToBuild = 5;


off_t SizeOf_file (const std::string& fname)
{
	if (FILE* f = fopen (fname.c_str(), "rb"))
	{
		fseek (f, 0, SEEK_END);
		long len = ftell (f); 
		fclose (f); 
		return len ; 
	}
	return 0;  

}



std::string& From_file (std::string& out, const std::string& fname)
{
	size_t sizeOf_file = SizeOf_file (fname) ; 
	out.resize (sizeOf_file); 
	if (FILE* f = fopen (fname.c_str(), "rb"))
	{
		size_t read_Len = fread (&out[0], 1, sizeOf_file, f);
		fclose (f); 
		return out; 
	}
	 
	return out; 
}

void Validate_GL_call ()
{
   int wat = 0; 
   GLenum         err      = 0; 
   const GLubyte* error_s_ = 0; 
      
   err = glGetError ();
   if (err != GL_NO_ERROR )
   {
      error_s_  = glewGetErrorString (err);
      BOOST_ASSERT (0); 
   }


   wat++; 
}

GLuint Create_shader (const GLchar* shaderSource, GLenum shaderType)
{
   BOOST_ASSERT (shaderSource); 
 
   bool valid_res = false;

      const int valid_shader_tpes[] = {   
         GL_VERTEX_SHADER , 
         GL_FRAGMENT_SHADER,
         GL_TESS_CONTROL_SHADER,
         GL_TESS_EVALUATION_SHADER,
         GL_GEOMETRY_SHADER
         }; 

   for (int i = 0; i < 5; i++) 
      valid_res |= shaderType == valid_shader_tpes[i];

   BOOST_ASSERT (valid_res); 

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

      printf ("\n%s\n", err_buf); 
      //Debug ("Shader Error:\n %s", output);
		//BreakAssert (0, "shader compile fail");
      glDeleteShader(shaderID); 
      BOOST_ASSERT (0); 
		return 0;
	}

   return shaderID; 

}

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

      printf ("\n%s : Shader Linking Failed: %s\n", __FUNCTIONW__, outputbuffer); 
      //Debug ( "\nGL Info Log :  \n\n %s\n", outputbuffer);
		//Assert (0, "ShaderProg::Link () - Failed");
      glDeleteProgram (progID); 
		return 0;
	}

   return progID; 
}


//
// Generate_tri_grid_indices - starts bottom left 
template<typename IntTy>
   std::vector<IntTy>& Generate_tri_grid_indices (std::vector<IntTy>& tri_inds, int num_u_tiles, int num_v_tiles) 
{
   int num_tiles = num_u_tiles * num_v_tiles; 
   tri_inds.resize (num_tiles * 2 * 3); 

   int num_x_verts = num_u_tiles + 1; 

   // generate tri indies for grid                   
   for (int iv  = 0; iv  < num_v_tiles ; iv++)
      for (int iu = 0; iu < num_u_tiles; iu++)
   {
      int base_ind = 6 * (iv * num_u_tiles + iu) ; 
      tri_inds[base_ind+0] = (iv * num_x_verts) + iu;       
      tri_inds[base_ind+1] = (iv * num_x_verts) + iu + 1;       
      tri_inds[base_ind+2] = ((1+iv) * num_x_verts) + iu + 1;       
      tri_inds[base_ind+3] = ((1+iv) * num_x_verts) + iu + 1;       
      tri_inds[base_ind+4] = ((1+iv) * num_x_verts) + iu;       
      tri_inds[base_ind+5] = (iv * num_x_verts) + iu;       
   }

   return tri_inds; 
}

template<typename Ty>
   Ty* Generate_tri_grid_indices (Ty* tri_inds, int num_u_tiles, int num_v_tiles) 
{
   int num_tiles = num_u_tiles * num_v_tiles; 

   int num_x_verts = num_u_tiles + 1; 

   // generate tri indies for grid                   
   for (int iv  = 0; iv  < num_v_tiles ; iv++)
      for (int iu = 0; iu < num_u_tiles; iu++)
   {
      int base_ind = 6 * (iv * num_u_tiles + iu) ; 
      tri_inds[base_ind+0] = (iv * num_x_verts) + iu;       
      tri_inds[base_ind+1] = (iv * num_x_verts) + iu + 1;       
      tri_inds[base_ind+2] = ((1+iv) * num_x_verts) + iu + 1;       
      tri_inds[base_ind+3] = ((1+iv) * num_x_verts) + iu + 1;       
      tri_inds[base_ind+4] = ((1+iv) * num_x_verts) + iu;       
      tri_inds[base_ind+5] = (iv * num_x_verts) + iu;       
   }

   return tri_inds; 
}


unsigned short* Generate_tri_grid_indices_u16 (unsigned short* tri_inds, int num_u_tiles, int num_v_tiles)
   {
return Generate_tri_grid_indices (tri_inds, num_u_tiles, num_v_tiles) ; 
   }
    


glm::dvec3* Generate_grid_vert3 (glm::dvec3* verts, int num_x_verts, int num_y_verts, double x_size, double y_size)
{
   double x_step = x_size / (num_x_verts - 1); 
   double y_step = y_size / (num_y_verts - 1); 

   for (int iy = 0; iy < num_y_verts; iy++) 
   {
      for (int ix = 0; ix < num_x_verts; ix++)
      {
      verts[(iy * num_x_verts) + ix].x = x_step * ix; 
      verts[(iy * num_x_verts) + ix].y = y_step * iy; 
      }
   }

   return verts; 
}

//
//
glm::dvec2* Generate_grid_vert2 (
   glm::dvec2* verts, 
   int         num_x_verts, 
   int         num_y_verts, 
   
   double      x_size, 
   double      y_size, 
   
   bool        flipX, 
   bool        flipY)
{
   double x_step = x_size / (num_x_verts - 1); 
   double y_step = y_size / (num_y_verts - 1); 

   for (int iy = 0; iy < num_y_verts; iy++) 
   {
      
      int ycrd = flipY ? (num_y_verts - iy - 1) : iy; 
      
      for (int ix = 0; ix < num_x_verts; ix++)
      {
         int xcrd = flipX ? (num_x_verts - ix - 1) : ix; 
         verts[ycrd * num_x_verts + xcrd].x = x_step * ix; 
         verts[ycrd * num_x_verts + xcrd].y = y_step * iy; 
      }
   }

   return verts; 
}


const int k_Texture_budget = 128; 


//
//
struct GL_HWT_renderer : public Terrain_renderer
{
   GL_HWT_renderer ();
   virtual ~GL_HWT_renderer ();
   int            Initialize                 ();
   virtual int    Initialize_draw_context    (); 
   virtual int    Deinitialize_draw_context  (); 
   virtual void   Patch_scale (double xscale, double yscale, double zscale); 
   virtual void   Patch_offset (double xoffs, double yoffs, double zoffs)  ; 

   virtual void   Tesselation_factors (float inner, float outer); 
   virtual int    Draw                       (const View_params& params, std::vector<Renderable*> objs);
   //
   // 
   TextureIDMap   tex_ID_cache[BG_tile::kNumTextureLevels];
   GLuint         texture_ID_budget[2 * k_Texture_budget]; 
   std::string    vp_filename; 
   std::string    fp_filename; 

   std::string    vp_fname;
   std::string    te_fname;
   std::string    tc_fname;
   std::string    fp_fname;   

   ShaderTable    shader_Table; 
   UniformMap     uniformLoc_map; 
   AttributeMap   attribLoc_map; 

   std::array<glm::dvec3, 4> patch_verts;
   std::array<glm::dvec2, 4> patch_txcds;

   // NOTE: treat all uniforms as vectors of 4
   // NOTE: f64 uniforms are not available, so use f32
   glm::fvec4 patch_Scale;
   glm::fvec4 patch_Offset;

   float tessInner;
   float tessOuter;



}; 

void GL_HWT_renderer::Tesselation_factors (float inner, float outer)
{
   tessInner = inner; 
   tessOuter = outer; 
}

#define HWTESS_ROOT "C:/Quarantine/hardcore/hwtess_Mars/shader/"
GL_HWT_renderer::GL_HWT_renderer ()  
   : vp_fname(HWTESS_ROOT "bg_hwtess.vp")
   , te_fname(HWTESS_ROOT "bg_hwtess.te")
   , tc_fname(HWTESS_ROOT "bg_hwtess.tc")
   , fp_fname(HWTESS_ROOT "bg_hwtess.fp")

   , shader_Table    ()
   , uniformLoc_map  ()
   , attribLoc_map   ()
   , patch_Scale     (1000.0f, 1.0f, 1000.0f, 0.0f)
   , patch_Offset    (0.0f, 0.0f, 0.0f, 0.0f)
   , tessInner       (1.0f)
   , tessOuter       (1.0f)
{
   const glm::dvec3 verts[] = {
      glm::dvec3(0.0, 0.0, 0.0), 
      glm::dvec3(1.0, 0.0, 0.0), 
      glm::dvec3(1.0, 0.0, -1.0), 
      glm::dvec3(0.0, 0.0, -1.0), 
   };

   const glm::dvec2 txcds[] = {
      glm::dvec2 (0.0, 0.0), 
      glm::dvec2 (1.0, 0.0),
      glm::dvec2 (1.0, 1.0), 
      glm::dvec2 (0.0, 1.0)
     };

   for (int i = 0; i < 4; i++)
   {
      patch_verts[i] = verts[i]; 
      patch_txcds[i] = txcds[i]; 
   }
}
//
GL_HWT_renderer::~GL_HWT_renderer ()
{

} 

int GL_HWT_renderer::Initialize ()
{
   return  0; 
}

int GL_HWT_renderer::Initialize_draw_context ()
{

      GLboolean ver_avail_11 = GLEW_VERSION_1_1  ;
      GLboolean ver_avail_12 = GLEW_VERSION_1_2  ;
      GLboolean ver_avail_121 = GLEW_VERSION_1_2_1;
      GLboolean ver_avail_13 = GLEW_VERSION_1_3  ;
      GLboolean ver_avail_14 = GLEW_VERSION_1_4  ;
      GLboolean ver_avail_15 = GLEW_VERSION_1_5  ;
      GLboolean ver_avail_20 = GLEW_VERSION_2_0  ;
      GLboolean ver_avail_21 = GLEW_VERSION_2_1  ;
      GLboolean ver_avail_30 = GLEW_VERSION_3_0  ;
      GLboolean ver_avail_31 = GLEW_VERSION_3_1  ;
      GLboolean ver_avail_32 = GLEW_VERSION_3_2  ;
      GLboolean ver_avail_33 = GLEW_VERSION_3_3  ;
      GLboolean ver_avail_40 = GLEW_VERSION_4_0  ;
      GLboolean ver_avail_41 = GLEW_VERSION_4_1  ;
      GLboolean ver_avail_42 = GLEW_VERSION_4_2  ;
      GLboolean ver_avail_43 = GLEW_VERSION_4_3  ;
      GLboolean ver_avail_44 = GLEW_VERSION_4_4  ;

         // vp_fname
         // te_fname
         // tc_fname
         // fp_fname
      std::string vert_shader_source; 
      From_file (vert_shader_source, vp_fname); 
      shader_Table["BG_vs"] = Create_shader (vert_shader_source.c_str(), GL_VERTEX_SHADER);  

      std::string tess_contr_shader_source; 
      From_file (tess_contr_shader_source, tc_fname); 
      shader_Table["BG_tc"] = Create_shader (tess_contr_shader_source.c_str(), GL_TESS_CONTROL_SHADER); 
         
      std::string tess_eval_source; 
      From_file (tess_eval_source, te_fname); 
      shader_Table["BG_te"] = Create_shader (tess_eval_source.c_str(), GL_TESS_EVALUATION_SHADER); 

      std::string frag_shader_source; 
      From_file (frag_shader_source, fp_fname); 
      shader_Table["BG_fs"] = Create_shader (frag_shader_source.c_str(), GL_FRAGMENT_SHADER); 

      GLuint shaders[] = 
      {
         shader_Table["BG_vs"] , 
         shader_Table["BG_tc"] , 
         shader_Table["BG_te"] , 
         shader_Table["BG_fs"] , 
         0
      } ; 

      shader_Table["BG_prog"] = Build_shader_program (shaders); 
      
      if (shader_Table["BG_prog"]) 
      {
      glUseProgram (shader_Table["BG_prog"]); 
      Validate_GL_call (); 

      GLint validate_res = 0; 
      glValidateProgram (shader_Table["BG_prog"]); 
      glGetProgramiv    (shader_Table["BG_prog"], GL_VALIDATE_STATUS, &validate_res) ; 
      BOOST_ASSERT      (validate_res); 
    


      // find input locations
      attribLoc_map["patch_coord"]   = glGetAttribLocation (shader_Table["BG_prog"], "patch_coord");
      attribLoc_map["tex_coord"]     = glGetAttribLocation (shader_Table["BG_prog"], "tex_coord"  );

      // constants
      uniformLoc_map["colorMap"]    = glGetUniformLocation (shader_Table["BG_prog"], "colorMap"   );
      uniformLoc_map["heightMap"]   = glGetUniformLocation (shader_Table["BG_prog"], "heightMap"  );
      uniformLoc_map["patch_Scale"] = glGetUniformLocation (shader_Table["BG_prog"], "patch_Scale");
      //uniformLoc_map["normalMap"]   = glGetUniformLocation (shader_Table["BG_prog"], "normalMap");
      uniformLoc_map["tessInner"] = glGetUniformLocation (shader_Table["BG_prog"], "tessInner"   );
      uniformLoc_map["tessOuter"] = glGetUniformLocation (shader_Table["BG_prog"], "tessOuter"   );


      uniformLoc_map["mat_Model"]   = glGetUniformLocation (shader_Table["BG_prog"], "mat_Model");
      uniformLoc_map["mat_View"]    = glGetUniformLocation (shader_Table["BG_prog"], "mat_View" );
      uniformLoc_map["mat_Proj"]    = glGetUniformLocation (shader_Table["BG_prog"], "mat_Proj" ); 

      for (ShaderTable::const_iterator i =  shader_Table.begin(); i != shader_Table.end(); i++) 
         BOOST_ASSERT (i->second > 0); 

      for (UniformMap::const_iterator i =  uniformLoc_map.begin(); i != uniformLoc_map.end(); i++) 
         BOOST_ASSERT (i->second >= 0); 
   
      for (AttributeMap::const_iterator i =  attribLoc_map.begin(); i != attribLoc_map.end(); i++) 
         BOOST_ASSERT (i->second >= 0); 

      }
      glUseProgram (0); 

      return 0; 
}

int GL_HWT_renderer::Deinitialize_draw_context ()
{
   return  0; 
}

void GL_HWT_renderer::Patch_scale (double xscale, double yscale, double zscale)
{
   patch_Scale[0] = xscale;
   patch_Scale[1] = yscale;
   patch_Scale[2] = zscale;
}

void GL_HWT_renderer::Patch_offset (double xoffs, double yoffs, double zoffs)
{
   patch_Offset[0] = xoffs;
   patch_Offset[1] = yoffs;
   patch_Offset[2] = zoffs;
}



int GL_HWT_renderer::Draw (const View_params& params, std::vector<Renderable*> objs)
   {
      glMatrixMode (GL_MODELVIEW); 
      glPushMatrix (); 

      glUseProgram (shader_Table["BG_prog"]); 
      Validate_GL_call (); 
      
      glPatchParameteri(GL_PATCH_VERTICES, 4);


      // face cull
      static bool enable_culling = true; 
      (enable_culling ? glEnable : glDisable) (GL_CULL_FACE); 
      glFrontFace (GL_CCW); 

      // polygon mode   
      static int DBG_polygon_mode = 2; 
      GLenum polygon_Mode[] = { GL_POINT,  GL_LINE,  GL_FILL }; 
      glPolygonMode (GL_FRONT_AND_BACK, polygon_Mode[DBG_polygon_mode]); 
      Validate_GL_call (); 



      glEnable (GL_TEXTURE_2D); 
      Validate_GL_call (); 
      
      //
      // geometry streams  
      glEnableVertexAttribArray  (attribLoc_map["patch_coord"]); 
      glVertexAttribPointer      (attribLoc_map["patch_coord"], 3, GL_DOUBLE, GL_FALSE, 0, patch_verts.data()); 
      Validate_GL_call (); 
      glEnableVertexAttribArray  (attribLoc_map["tex_coord"]); 
      glVertexAttribPointer      (attribLoc_map["tex_coord"], 2, GL_DOUBLE, GL_FALSE, 0, patch_txcds.data()); 
      Validate_GL_call (); 

      //
      // patch scaling (glUniform4dv not available)
      glUniform4fv (uniformLoc_map["patch_Scale"], 1, glm::value_ptr (patch_Scale)); 
      Validate_GL_call (); 
       
      // tesselation factor
      glUniform1f (uniformLoc_map["tessInner"], tessInner); 
      glUniform1f (uniformLoc_map["tessOuter"], tessOuter); 

      //
      // view matrix 
      glm::dmat4 mat_View; 
      {
      const glm::dvec3  v_right  (1.0, 0.0, 0.0), 
                        v_up     (0.0, 1.0, 0.0), 
                        v_fwd    (0.0, 0.0, 1.0); 

      mat_View = glm::translate  (params.pos) * glm::eulerAngleYX(params.rot[1], params.rot[0]) ;  
      mat_View = glm::inverse    (mat_View); 

      float fMat_view[16]; // double precision (glUniformMatrix4dv) not available
      std::copy (glm::value_ptr (mat_View), glm::value_ptr (mat_View) + 16, fMat_view); 
      glUniformMatrix4fv (uniformLoc_map["mat_View"], 1, GL_FALSE, fMat_view); 

      Validate_GL_call (); 

      }

      //
      // projection 
      {

      glm::dmat4 mat_Proj = glm::perspective (
         params.FoV,       //kPerspective_FoV,  
         params.Asp_ratio, //camera_y_asp,
         params.dist_Near, //kNear_plane_dist,  
         params.dist_Far   //kFar_plane_dist  
         ); 


      float fMat_proj[16];// double precision (glUniformMatrix4dv) not available
      std::copy (glm::value_ptr (mat_Proj), glm::value_ptr (mat_Proj) + 16, fMat_proj); 
      glUniformMatrix4fv (uniformLoc_map["mat_Proj"], 1, GL_FALSE, fMat_proj); 
      Validate_GL_call (); 
      }

      //
      for (int i = 0; i < objs.size(); i++)
      {
         // this is draw loop 
         // set up grid first  // abstract data set into interface?   
         Validate_GL_call (); 

         const glm::dvec3& dPos = objs[i]->Get_pos (); 

         glm::fvec3 model_pos (dPos.x, dPos.y, dPos.z);  
         glm::fmat4 mat_Model = glm::translate (model_pos); 
         glUniformMatrix4fv (uniformLoc_map["mat_Model"] , 1, GL_FALSE, glm::value_ptr (mat_Model)); 
         Validate_GL_call (); 

//      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE); 
         objs[i]->Setup_RS (uniformLoc_map, attribLoc_map); 
         glDrawArrays (GL_PATCHES, 0, 4); 
         Validate_GL_call (); 
      }

      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); 

      glUseProgram (0); 

      glEnable (GL_DEPTH_TEST); 
      glDisableVertexAttribArray (attribLoc_map["patch_coord"]); 
      glDisableVertexAttribArray (attribLoc_map["tex_coord"]); 
      glMatrixMode (GL_MODELVIEW); 
      glPopMatrix(); 

      return 0; 
   }


//
//
// GL_BackgroundRenderer
struct GL_BackgroundRenderer : public Terrain_renderer
{
   static const int kVertex_X_dim = 33;
   static const int kVertex_Y_dim = 33;
   
   TextureIDMap               tex_ID_cache[BG_tile::kNumTextureLevels];
   GLuint                     texture_ID_budget[2 * k_Texture_budget]; 
   std::string                vp_filename; 
   std::string                fp_filename; 
   
   
   //std::vector<tx_ID_pair>    available_texture_IDs;

   //std::vector<GLuint>      height_IDs; 
   //std::vector<GLuint>      color_IDs ; 
   boost::shared_array<glm::dvec2>      grid_verts; // [BG_tile::kNumTextureLevels]; 
   boost::shared_array<glm::dvec2>      grid_txcrds;
   boost::shared_array<unsigned short>  grid_inds; 

   bool no_draw; 

   //HDC               hdc; 
   //HGLRC             glrc; 
   //GLEWContext*      glew_Context; 
   
   ShaderTable    shader_Table; 
   UniformMap     uniformLoc_map; 
   AttributeMap   attribLoc_map; 


   GLuint Create_shader          (const char* shaderSource, GLenum shaderType); 
   GLuint Build_shader_program   (const GLuint* shaders); 
   void Validate_GL_call         (); 


   GL_BackgroundRenderer ();
   virtual ~GL_BackgroundRenderer (); 

   int Initialize ();
   virtual int Initialize_draw_context (); 
   virtual int Deinitialize_draw_context ();
   virtual void   Patch_scale (double xscale, double yscale, double zscale); 
   virtual void   Patch_offset (double xoffs, double yoffs, double zoffs)  ; 
   virtual void   Tesselation_factors (float inner, float outer); 

   virtual int Draw (const View_params& params, std::vector<Renderable*> objs); 

}; 


   //
   //
   GL_BackgroundRenderer::GL_BackgroundRenderer () 
      : vp_filename("J:/Quarantine/Mars/bg_shader.vp")
      , fp_filename("J:/Quarantine/Mars/bg_shader.fp")
      , grid_verts () 
      , grid_txcrds()
      , grid_inds  () 
   {
      Initialize (); 
   }
   
   //
   //
   GL_BackgroundRenderer::~GL_BackgroundRenderer () 
   {
      glDeleteTextures (k_Texture_budget , texture_ID_budget); 
   }

   //
   //
   int GL_BackgroundRenderer::Initialize ()
   {
      int kVertex_X_dim = 33;
      int kVertex_Y_dim = 33;

      // alloc geometry
      int num_verts  = kVertex_X_dim * kVertex_Y_dim;
      int num_cols   = kVertex_X_dim - 1; 
      int num_rows   = kVertex_Y_dim - 1; 

      //rows * cols * 3inds/tri * 2tris/tile; 
      int num_inds = num_rows * num_cols * 2 * 3; 

      grid_verts.reset (new glm::dvec2[num_verts]); 
      Generate_grid_vert2 (grid_verts.get(), kVertex_X_dim , kVertex_Y_dim , 1.0, 1.0, false, true); 

      grid_txcrds.reset (new glm::dvec2[num_verts]); 
      Generate_grid_vert2 (grid_txcrds.get(), kVertex_X_dim , kVertex_Y_dim , 1.0, 1.0, false, false);

      grid_inds.reset   (new unsigned short[num_inds]  ); 
      Generate_tri_grid_indices (grid_inds.get(), num_cols, num_rows);
      
      return 0; 
   }

 

   int GL_BackgroundRenderer::Initialize_draw_context ()
   {

      GLboolean ver_avail_11 = GLEW_VERSION_1_1  ;
      GLboolean ver_avail_12 = GLEW_VERSION_1_2  ;
      GLboolean ver_avail_121 = GLEW_VERSION_1_2_1;
      GLboolean ver_avail_13 = GLEW_VERSION_1_3  ;
      GLboolean ver_avail_14 = GLEW_VERSION_1_4  ;
      GLboolean ver_avail_15 = GLEW_VERSION_1_5  ;
      GLboolean ver_avail_20 = GLEW_VERSION_2_0  ;
      GLboolean ver_avail_21 = GLEW_VERSION_2_1  ;
      GLboolean ver_avail_30 = GLEW_VERSION_3_0  ;
      GLboolean ver_avail_31 = GLEW_VERSION_3_1  ;
      GLboolean ver_avail_32 = GLEW_VERSION_3_2  ;
      GLboolean ver_avail_33 = GLEW_VERSION_3_3  ;
      GLboolean ver_avail_40 = GLEW_VERSION_4_0  ;
      GLboolean ver_avail_41 = GLEW_VERSION_4_1  ;
      GLboolean ver_avail_42 = GLEW_VERSION_4_2  ;
      GLboolean ver_avail_43 = GLEW_VERSION_4_3  ;
      GLboolean ver_avail_44 = GLEW_VERSION_4_4  ;


      //static char vert_shader_source[] = {
      //   "  uniform mat4      mat_Model   ;                                               \n"
      //   "  uniform mat4      mat_View    ;                                               \n"
      //   "  uniform mat4      mat_Proj    ;                                               \n"
      //   "  uniform float     height_Scale;                                               \n"
      //   "  uniform sampler2D heightMap;                                                  \n"
      //   "                                                                                \n"
      //   "  in vec2      coord_EN;                                                        \n"
      //   "  in vec2      texCoord;                                                        \n"
      //   "                                                                                \n"
      //   "  varying vec2     frag_txcd;                                                   \n"
      //   "                                                                                \n"
      //   "  void main ()                                                                  \n"
      //   "  {                                                                             \n"
      //   "     float height_Coord   = height_Scale * texture2D (heightMap, texCoord).x;   \n"
      //   "     vec4  posi           = vec4 (coord_EN.x, coord_EN.y, height_Coord, 1.0);   \n"
      //   "     posi = mat_Model * posi;                                                   \n"
      //   "     posi = mat_View * posi;                                                    \n"
      //   "                                                                                \n"
      //   "     frag_txcd   = texCoord;                                                    \n"
      //   "     gl_Position = mat_Proj * posi;                                             \n"
      //   "  }                                                                             \n"
      //   "                                                                                \n"
      //   }; 

      // static GLchar frag_shader_source[] = {
      //    "                                                     \n"
      //    "  uniform sampler2D colorMap;                        \n"
      //    "                                                     \n"
      //    "  in vec2           frag_txcd;                       \n"
      //    "                                                     \n"
      //    "  // out vec4          frag_color                    \n"
      //    "  void main ()                                       \n"
      //    "  {                                                  \n"
      //    "     gl_FragColor = texture2D (colorMap, frag_txcd); \n"
      //    "  }                                                  \n"
      //    "                                                     \n"
      //    }; 

      
      //"C:/usr/bg_shader.vp"
      //"C:/usr/bg_shader.fp"
      std::string vert_shader_source; 
      From_file (vert_shader_source, vp_filename ); 
      shader_Table["BG_vs"] = Create_shader (vert_shader_source.c_str(), GL_VERTEX_SHADER);  

      std::string frag_shader_source; 
      From_file (frag_shader_source, fp_filename ); 
      shader_Table["BG_fs"] = Create_shader (frag_shader_source.c_str(), GL_FRAGMENT_SHADER); 

      GLuint shaders[] = 
      {
         shader_Table["BG_vs"] , 
         shader_Table["BG_fs"] , 
         0
      } ; 

      shader_Table["BG_prog"] = Build_shader_program (shaders); 

      // find input locations
      attribLoc_map["coord_EN"] = glGetAttribLocation (shader_Table["BG_prog"], "coord_EN");
      attribLoc_map["texCoord"] = glGetAttribLocation (shader_Table["BG_prog"], "texCoord");

      // contants
      uniformLoc_map["colorMap"]    = glGetUniformLocation (shader_Table["BG_prog"], "colorMap");
      uniformLoc_map["heightMap"]   = glGetUniformLocation (shader_Table["BG_prog"], "heightMap");
      uniformLoc_map["height_Scale"]= glGetUniformLocation (shader_Table["BG_prog"], "height_Scale");
      
      uniformLoc_map["mat_Model"]   = glGetUniformLocation (shader_Table["BG_prog"], "mat_Model");
      uniformLoc_map["mat_View"]    = glGetUniformLocation (shader_Table["BG_prog"], "mat_View");
      uniformLoc_map["mat_Proj"]    = glGetUniformLocation (shader_Table["BG_prog"], "mat_Proj");

      glUseProgram (0); 

      return 0; 
   } 

   //
   void   GL_BackgroundRenderer::Tesselation_factors (float inner, float outer)
   {
   }

   //
   //
   int GL_BackgroundRenderer::Deinitialize_draw_context ()
   {
      BOOST_ASSERT (0); 
      return -1;
   } 

   void GL_BackgroundRenderer::Patch_scale (double X, double Y, double Z)
   {
      for (int i = 0; i < kVertex_X_dim * kVertex_X_dim ; i++) 
      {
         glm::dvec2& v = grid_verts.get()[i]; 

         v.x *= X; 
         v.y *= Z; 
      }
   } 

   //
   void GL_BackgroundRenderer::Patch_offset(double X, double Y, double Z)
   {
      for (int i = 0; i < kVertex_X_dim * kVertex_X_dim ; i++) 
      {
         glm::dvec2& v = grid_verts.get()[i]; 

         v.x += X; 
         v.y += Z; 
      }
   }

   //
   int GL_BackgroundRenderer::Draw (const View_params& params, std::vector<Renderable*> objs)
   {

      glMatrixMode (GL_MODELVIEW); 
      glPushMatrix (); 

      glUseProgram (shader_Table["BG_prog"]); 
      Validate_GL_call (); 

static int DBG_polygon_mode = 2; 
GLenum polygon_Mode[] = { GL_POINT,  GL_LINE,  GL_FILL }; 

glEnable (GL_CULL_FACE); 
glFrontFace (GL_CCW); 
glPolygonMode (GL_FRONT_AND_BACK, polygon_Mode[DBG_polygon_mode]); 

      Validate_GL_call (); 

      glEnable (GL_TEXTURE_2D); 
      Validate_GL_call (); 
      
      glEnableVertexAttribArray  (attribLoc_map["coord_EN"]); 
      glVertexAttribPointer      (attribLoc_map["coord_EN"], 2, GL_DOUBLE, GL_FALSE, 0, grid_verts.get()); 
      Validate_GL_call (); 
      
      glEnableVertexAttribArray  (attribLoc_map["texCoord"]); 
      glVertexAttribPointer      (attribLoc_map["texCoord"], 2, GL_DOUBLE, GL_FALSE, 0, grid_txcrds.get()); 
      Validate_GL_call (); 
      
      //
      // view matrix 
      glm::dmat4 mat_View; 
      {
      const glm::dvec3  v_right  (1.0, 0.0, 0.0), 
                        v_up     (0.0, 1.0, 0.0), 
                        v_fwd    (0.0, 0.0, 1.0); 

      mat_View = glm::translate(params.pos) * glm::eulerAngleYX(params.rot[1], params.rot[0]) ;  
      mat_View = glm::inverse (mat_View); 

      float fMat_view[16]; // double precision (glUniformMatrix4dv) not available
      std::copy (glm::value_ptr (mat_View), glm::value_ptr (mat_View) + 16, fMat_view); 
      glUniformMatrix4fv (uniformLoc_map["mat_View"], 1, GL_FALSE, fMat_view); 
      Validate_GL_call (); 
      }

      //
      // projection 
      {
      glm::dmat4 mat_Proj = glm::perspective (
         params.FoV,       //kPerspective_FoV,  
         params.Asp_ratio, //camera_y_asp,
         params.dist_Near, //kNear_plane_dist,  
         params.dist_Far   //kFar_plane_dist  
         ); 

      float fMat_proj[16];// double precision (glUniformMatrix4dv) not available
      std::copy (glm::value_ptr (mat_Proj), glm::value_ptr (mat_Proj) + 16, fMat_proj); 
      glUniformMatrix4fv (uniformLoc_map["mat_Proj"], 1, GL_FALSE, fMat_proj); 
      Validate_GL_call (); 
      }


      //
      for (int i = 0; i < objs.size(); i++)
      {
         // this is draw loop 
         // set up grid first  // abstract data set into interface?   
         Validate_GL_call (); 

         const glm::dvec3& dPos = objs[i]->Get_pos (); 
         
         glm::fvec3 model_pos (dPos.x, dPos.y, dPos.z);  

         glm::fmat4 mat_Model = glm::translate (model_pos); 
         glUniformMatrix4fv (uniformLoc_map["mat_Model"] , 1, GL_FALSE, glm::value_ptr (mat_Model)); 
         Validate_GL_call (); 
      

//      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE); 


         objs[i]->Setup_RS (uniformLoc_map, attribLoc_map); 

         const int kNum_rows = GL_BackgroundRenderer::kVertex_Y_dim - 1; 
         const int kNum_cols = GL_BackgroundRenderer::kVertex_X_dim - 1; 
         int ind_count = 2 * 3 * kNum_rows * kNum_cols; 
         glDrawElements (GL_TRIANGLES, ind_count, GL_UNSIGNED_SHORT, grid_inds.get()); 
         Validate_GL_call (); 
      }

      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); 

      glUseProgram (0); 

      glEnable (GL_DEPTH_TEST); 
      glDisableVertexAttribArray (attribLoc_map["coord_EN"]); 
      glDisableVertexAttribArray (attribLoc_map["texCoord"]); 
      glMatrixMode (GL_MODELVIEW); 
      glPopMatrix(); 

      return 0; 
   }


//
//
void GL_BackgroundRenderer::Validate_GL_call ()
{
   ::Validate_GL_call (); 
}


//
GLuint GL_BackgroundRenderer::Create_shader (const GLchar* shaderSource, GLenum shaderType)
{
   BOOST_ASSERT (shaderSource); 
   BOOST_ASSERT (shaderType == GL_VERTEX_SHADER  || shaderType == GL_FRAGMENT_SHADER); 

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
      BOOST_ASSERT (0); 
		return 0;
	}

   return shaderID; 

}

//
GLuint GL_BackgroundRenderer::Build_shader_program (const GLuint* shaders)
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


//
//
Terrain_renderer* Terrain_renderer::Create(std::string& rtype)
{ 
   if (rtype == "hw")
      return new GL_HWT_renderer; 
   else 
   if (rtype == "std")
      return new GL_BackgroundRenderer; 
   else
      return new GL_BackgroundRenderer; 

   BOOST_ASSERT(0); 
   return nullptr; 
}
