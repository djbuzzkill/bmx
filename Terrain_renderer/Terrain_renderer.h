

#ifndef BG_RENDERER_H
#define BG_RENDERER_H

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <array>


#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <boost/assert.hpp>
#include <boost/shared_array.hpp>

#include <Dx/Render.h>
//
//

struct View_params
{
   glm::fvec3 pos;
   glm::fvec3 rot; 

   float FoV; 
   float Asp_ratio;
   float dist_Near;
   float dist_Far; 
}; 

void     Validate_GL_call (); 
GLenum   GL_TEXTURE_stage (int stage); 

namespace BG_tile
{
   typedef glm::ivec2 Key; 


   enum DataType 
   {
      INVALID_DATA_TYPE = -1, 
      DT_int8  , 
      DT_uint8 ,  

      DT_int16, 
      DT_uint16, 

      DT_int32, 
      DT_uint32, 

      DT_real32, 
      DT_real64, 

   }; 



   enum { kHeightMap_dim = 256 };                                
   enum { kTextureWidth = 1024 }; 
   enum { kTextureHeight= 1024 }; 

   enum { kVertexDim_x = 128 }; 
   enum { kVertexDim_y = 128 }; 

   enum { kNumTextureLevels   = 12 }; 
   enum { kTextureOriginOffs  = 4 };    // 

   inline int Level_index (int lvl) 
   {

// do not support sub meter detail yet
//BOOST_ASSERT (lvl >= 0); 
   if (lvl < 0)
   return 0; 



      BOOST_ASSERT ( (lvl + kTextureOriginOffs) > 0); 
      BOOST_ASSERT ( (lvl + kTextureOriginOffs) < kNumTextureLevels); 
      // level  1: 2 meters
      // level  0: meter  <= Struc::kTextureOriginOffs
      // level -1: half meter 
      // level -2: quarter
      // level -3: ....
      return lvl + kTextureOriginOffs; 
   }


   struct less_than // used for std::map
   { 
      bool operator() (const Key& lhs, const Key& rhs) 
         { 
         if (lhs[0] < rhs[0]) 
            return true; 
         else if (lhs[0] == rhs[0]) 
            return (lhs[1] < rhs[1]) ? true : false; 
         else return false; 
         }
   };    



   

   struct GR_Struc // rename to be GeoRepl specific 
   {
      // Define texture level 0 to be 1 meter per texel 
      // kNumTextureLevels detail levels 
      struct Level_struct
      {
         DataType type; 
         int      num_bands; 
         void*    color; 
         void*    height; 
      };

      GR_Struc () 
      { 
         for (int i = 0; i < kNumTextureLevels; i++) 
            {
         tile_level[i].type      = INVALID_DATA_TYPE; 
         tile_level[i].num_bands = 0;                       
         tile_level[i].color     = 0;                       
         tile_level[i].height    = 0;                       
            } 
      }


      // tex_id[19] most detailed, tex_id[0] lowest;
      Level_struct tile_level[kNumTextureLevels]; 

   }; 

   typedef std::map<Key, GR_Struc, less_than> GR_Map; 


}







struct Terrain_params 
{
   struct Tile
   {
      glm::dvec2  origin;
      void*       col;
      void*       hgt;

      typedef std::map<BG_tile::Key, Tile, BG_tile::less_than> Map; 
   }; 
   
   int      num_X_tiles;
   int      num_Y_tiles; 

   double   easting_Tile_step;
   double   northing_Tile_step; 

   double   height_range ; 
   double   height_offset; 

   Tile::Map tileMap;
}; 


bool Visible_tiles ( 
   std::vector<Terrain_params::Tile*>  out, 
   const View_params&                  view_Params, 
   Terrain_params&                     tileMap
   );


#if defined (GLEW_MX)

struct GLEW_MT_context 
{
   std::shared_ptr<GLEWContext> glew_context;  

   //
   GLEWContext* glewGetContext ()
   {
   
      if (!glew_context)
         glew_context.reset (new GLEWContext); 
   
      return glew_context.get(); 
   }

   // 
   GLuint Build_shader_program (const GLuint* vertShaders, const GLuint* fragShaders)
   {
      GLuint progID = glCreateProgram (); 

      int count_vs = 0; 
      while (vertShaders[count_vs]) 
      {
         glAttachShader (progID, vertShaders[count_vs]); 
         count_vs++;  
      }   

      int count_fs = 0; 
      while (fragShaders[count_fs])
      {
         glAttachShader (progID, fragShaders[count_fs]); 
         count_fs++; 
      }

      glCompileShader (progID); 
      return progID; 
   }

   //
   GLuint Create_shader (const char* shaderSource, GLenum shaderType)
   {
      BOOST_ASSERT (shaderSource); 
      bool valid_shader = false; 
      const GLenum acceptable_symbols[] = {
         GL_VERTEX_SHADER,  
         GL_FRAGMENT_SHADER,
         GL_TESS_EVALUATION_SHADER, 
         GL_TESS_CONTROL_SHADER
         }; 

      for (int i = 0; i < 4; i++) 
         valid_shader |= (shaderType == acceptable_symbols[i]); 

      BOOST_ASSERT (valid_shader); 

      //GL_TESS_EVALUATION_SHADER                          0x8E87
      //GL_TESS_CONTROL_SHADER                             0x8E88

      GLuint shaderID = glCreateShader (shaderType); 

      GLint len_source[1] = { strlen (shaderSource) }; 
      
      glShaderSource (shaderID, 1, &shaderSource, len_source); 

      glCompileShader(shaderID); 

      return shaderID; 

   }
   //
   void Validate_GL_call ()
   {
      GLenum         err      = 0; 
      const GLubyte* error_s_ = 0; 
      
      err = glGetError ();
      if (err != GL_NO_ERROR )
      {
         error_s_  = glewGetErrorString (err);
         BOOST_ASSERT (0); 
      }
   }
}; 

#endif // GLEW_MX

glm::dvec3* Generate_grid_vert3 (
   glm::dvec3* verts, 
   int num_x_verts, 
   int num_y_verts, 
   double x_size, 
   double y_size
   );

glm::dvec2* Generate_grid_vert2 (
   glm::dvec2* verts, 
   int num_x_verts, 
   int num_y_verts, 
   double x_size, 
   double y_size, 
   bool flipX = false, 
   bool flipY = false
   );


unsigned short* Generate_tri_grid_indices_u16 (
   unsigned short* tri_inds, 
   int num_u_tiles, 
   int num_v_tiles
   ); 







struct Renderable; 
// BackgroundRenderer

struct Terrain_renderer 
{
   struct   tx_ID_pair { GLuint c, h; }; 
   typedef std::map<BG_tile::Key, tx_ID_pair, BG_tile::less_than> TextureIDMap;

   static Terrain_renderer* Create(std::string& rtype);

   virtual ~Terrain_renderer() {
      }
      

   virtual void   Tesselation_factors (float inner, float outer) = 0; 
   virtual int    Initialize_draw_context    () = 0; 
   virtual int    Deinitialize_draw_context  () = 0; 

   virtual void   Patch_scale (double xscale, double yscale, double zscale) = 0; 
   virtual void   Patch_offset (double xoffs, double yoffs, double zoffs)   = 0; 

//   virtual int Draw (
//      const int*        dim_viewport, 
//      const Planed*     frustum, 
//      const glm::dmat4& mat_Proj, 
//      const glm::dmat4& mat_View, 
//      const glm::dvec3& look_At, 
//      int               zone, 
//      double            zoom, 
//      double            scale_alt
//      ) = 0;

   virtual int Draw (const View_params& params, std::vector<Renderable*> objs) = 0;


protected: 
   Terrain_renderer () {
      }
}; 

struct Renderable 
   {
   virtual glm::fvec3&  Pos  () = 0; 
   virtual glm::fvec3&  Rot  () = 0;
   virtual glm::fvec3&  Scl  () = 0;

   virtual GLuint       Bin_ID() = 0; 
   virtual GLuint       ROp_ID() = 0; 

   virtual void         Setup_RS(const Rn::UniformMap& uniformMap, const Rn::AttributeMap& attribMap) = 0;
                                   
protected: 
   Renderable () { }
   }; 


#if 0

   // TileAccumulator 
   struct TileAccumulator 
   {

      //static TileAccumulator* Create (scc::CDem& dem);

      virtual ~TileAccumulator() {
         }


      // 2. accumulation section 

   #if defined (GLEW_MX)
      virtual int Initialize_accumulator_context   (HINSTANCE, BG_tile::Map* zones_N, BG_tile::Map* zones_S) = 0; 
      virtual int Deinitialize_accumulator_context () = 0; 
   #endif

      virtual int Accumulate_image (double& center_easting, double& center_northing, int& zone, const std::string& fname_geotiff) = 0; 

   protected: 
      TileAccumulator () {
         } 
   }; 

#endif



#endif // BG_RENDERER_H
