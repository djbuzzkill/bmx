

#ifndef BG_RENDERER_H
#define BG_RENDERER_H

#include <map>
#include <string>
#include <vector>
#include <memory>

#include <boost/assert.hpp>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

//
//
template<typename Ty>
   struct Plane { Ty A, B, C, D; 
   }; 

//
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

   // enum { kEasting_meters = 1000 };
   // enum { kNorthing_meters = 1000};
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


   
   struct Struc
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

      Struc () 
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


   typedef std::map<Key, Struc, less_than> Map; 
}; 


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
      BOOST_ASSERT (shaderType == GL_VERTEX_SHADER  || shaderType == GL_FRAGMENT_SHADER); 

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

// BackgroundRenderer
struct BackgroundRenderer 
{
   static BackgroundRenderer* Create ();

   virtual ~BackgroundRenderer () {
      }
      

#if defined (GLEW_MX)
   virtual int Initialize_draw_context    (GLEWContext*, HDC, HGLRC, scc::CDem& dem, BG_tile::Map*, BG_tile::Map*) = 0; 
   virtual int Deinitialize_draw_context  () = 0; 

#endif


   virtual int Draw (
      const int*           dim_viewport, 
      const Plane<double>* frustum, 
      const glm::dmat4&    mat_Proj, 
      const glm::dmat4&    mat_View, 
      const glm::dvec3&    look_At, 
      int                  zone, 
      double               zoom, 
      double               scale_alt
      ) = 0;

protected: 
   BackgroundRenderer () {
      }
}; 

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
