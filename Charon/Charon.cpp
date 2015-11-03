#include "Charon.h"


//
//
size_t Max_elems_in_buff (size_t sizeOf_buf, size_t sizeOf_elem)
{
   DX_ASSERT (sizeOf_elem, "sizeOf_elem is 0"); 

   if (!sizeOf_elem)
      return ~0x0; 

   return (sizeOf_buf / sizeOf_elem); 
} 

void lolwut()
{
   ptru mem { malloc (1024) };

   free (mem.v); 
}

//
//
void xp::Update_view_transform (
	Ma::Vec3f&				view_Pos, 
	Ma::Vec3f&				view_Rot, 
	float					   move_Rate,
	const sy::Keyboard_state& kb,    
	const sy::Mouse_state&    ms)
{
   float dYdx = 1.0f; 
   float dXdy = 1.0f; 

   Ma::Vec3f dir_Fwd, dir_Right, dir_Up; 

   Ma::Set (dir_Up, 0.0f, 1.0f, 0.0f); 

   {
   
      Ma::X (view_Rot) += -(ms.yRel* dXdy);
      Ma::Clamp (Ma::X (view_Rot), -60.0f, 60.0f); 

      Ma::Y (view_Rot) += ms.xRel* -dYdx;


      while (Ma::Y (view_Rot) < 0.0)
   	   Ma::Y (view_Rot) += 360.0f; 
   
      while (Ma::Y (view_Rot) > 360.0)
   	   Ma::Y (view_Rot) -= 360.0f; 
   
      const float kDeg2Pi = (float) Ma::Pi / 180.0f; 
      const float fHalfPi = (float) Ma::HalfPi;
   
      Ma::Vec3f v_t;
      // (\ spherical.theta(0).phi(0)) => <1, 0, 0>
      Ma::Spherical (v_t, 0.0f, 0.0f);
      Ma::Spherical (dir_Fwd,    kDeg2Pi * -Ma::Y (view_Rot) - fHalfPi, 0.0f);
      Ma::Spherical (dir_Right,  kDeg2Pi * -Ma::Y (view_Rot), 0.0f); 
      Ma::X (dir_Right);
   }
   
   
   
   // view movement
   {
      Ma::Vec3f v;


      if (sy::Is_keydown (sy::SC_F, kb)) {
   	   X (v) = X (Ma::Scale (v, dir_Fwd, move_Rate));
         Ma::Incr (view_Pos, v);
   	   }
      else 
      if (sy::Is_keydown (sy::SC_V, kb)) {
   	   X (v) = X (Ma::Scale (v, dir_Fwd, -move_Rate));
         Ma::Incr (view_Pos, v);
   	   }
   
      //
      if (sy::Is_keydown (sy::SC_D, kb)) {
   	   Z (v) = 	Z (Ma::Scale (v, dir_Right, -move_Rate)); 
         Ma::Incr (view_Pos, v);
   	   }
      else 
      if (sy::Is_keydown (sy::SC_G, kb)) {
   	   Z (v) = 	Z (Ma::Scale (v, dir_Right, move_Rate)); 
         Ma::Incr (view_Pos, v);
   	   }

      if (sy::Is_keydown (sy::SC_A, kb)) {
   	   Y (v) = 	Y (Ma::Scale (v, dir_Up, move_Rate)); 
         Ma::Incr (view_Pos, v);
   	   }
      else
      if (sy::Is_keydown (sy::SC_Z, kb)) {
   	   Y (v) = 	Y (Ma::Scale (v, dir_Up, -move_Rate)); 
         Ma::Incr (view_Pos, v);
   	   }


   }   

   // view orientation
   //Ma::Set (view_Rot, 1.0f, 0.0f, 0.0f, 0.0f);
   //if (sg_X_mouse > 0 && sg_Y_mouse > 0 && sg_X_mouse < sg_Wd_window && sg_Y_mouse < sg_Ht_window)
   //	Ma::Mult (view_Rot, Ma::RotYAxis (q_Y, view_Y_rot), Ma::RotXAxis (q_X, view_X_rot)); 
   
}






namespace mars_terr
{
   using namespace glm;

   //
   const dvec3 kPatch_verts[] = {
      dvec3(0.0, 0.0, 0.0),
      dvec3(1000.0, 0.0, 0.0),
      dvec3(1000.0, 1000.0, 0.0),
      dvec3(0.0, 1000.0, 0.0),
   };

   const dvec2 kPatch_txcrd[] = {
      dvec2(0.0, 0.0),
      dvec2(1.0, 0.0),
      dvec2(1.0, 1.0),
      dvec2(0.0, 1.0),
   };


   const char*    kFiles[] = {
      MARS_DRIVE "Quarantine/Mars/ESP_018065_1975_RED_ESP_019133_1975_RED-DRG.tif",
      MARS_DRIVE "Quarantine/Mars/ESP_018065_1975_RED_ESP_019133_1975_RED-DEM.tif",
      MARS_DRIVE "Quarantine/Mars/ESP_018065_1975_RED_ESP_019133_1975_RED-IGM.dat"
   };
   const size_t   kNum_files = El_count(kFiles);

   const std::string kType[] = {
      "mars_col_",
      "mars_hgt_",
      "mars_igm_",
   };

   const FREE_IMAGE_FORMAT kFIF_fmt[] = {
      FIF_TIFF,
      FIF_TIFF,
      FIF_UNKNOWN,
   };


   const char     kBase_path[] = MARS_DRIVE "Quarantine/";
   const char     kTile_subp[] = "Mars/tiled/";
   const char     kShader_subp[] = "Hardcore/shader/";

   extern const char kShader_name[] = "mars_terr";

   extern const char* kShader_ext[] = {
      "_vp.glsl",
      "_tc.glsl",
      "_te.glsl",
      "_fp.glsl",
   };

   const GLuint kShader_types[] = {
      GL_VERTEX_SHADER,
      GL_TESS_CONTROL_SHADER,
      GL_TESS_EVALUATION_SHADER,
      GL_FRAGMENT_SHADER,
   };

   const int      kTexture_dim = 1024;
   const int      kNum_X_tiles = (6900 / kTexture_dim) + (6900 % kTexture_dim ? 1 : 0);
   const int      kNum_Y_tiles = (17177 / kTexture_dim) + (17177 % kTexture_dim ? 1 : 0);

   const unsigned kWd = 6900;
   const unsigned kHt = 17177;
}
