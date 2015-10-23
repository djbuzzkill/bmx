

#include "stdafx.h"
#include "test_GL4_experiment.h"
#include "quantize_height_tiles_to_u16.h"
#include "process_mars_terrain_for_runtime.h"

#include <boost/shared_array.hpp>
#include <Dx/VecMath.h>


#define GENERATE_MARS_TILES 0
//// 
//

#define RUN_QUANTIZE_HEIGHT_TILES_TO_U16 0
//// 
//

namespace mars_terr
{
	using namespace glm; 

	//
	const dvec3 kPatch_verts[] = {
		dvec3 (0.0,      0.0,     0.0), 
		dvec3 (1000.0,   0.0,     0.0), 
		dvec3 (1000.0,   1000.0,  0.0), 
		dvec3 (0.0,      1000.0,  0.0), 
		}; 

   const dvec2 kPatch_txcrd[] = {
      dvec2 (0.0, 0.0), 
      dvec2 (1.0, 0.0), 
      dvec2 (1.0, 1.0), 
      dvec2 (0.0, 1.0), 
      }; 


   const char*    kFiles[] = {
      MARS_DRIVE "Quarantine/Mars/ESP_018065_1975_RED_ESP_019133_1975_RED-DRG.tif", 
      MARS_DRIVE "Quarantine/Mars/ESP_018065_1975_RED_ESP_019133_1975_RED-DEM.tif",  
      MARS_DRIVE "Quarantine/Mars/ESP_018065_1975_RED_ESP_019133_1975_RED-IGM.dat"
      };
   const size_t   kNum_files = El_count (kFiles); 

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


   const char     kBase_path[]   = MARS_DRIVE "Quarantine/"; 
   const char     kTile_subp[]   = "Mars/tiled/"; 
   const char     kShader_subp[] = "Hardcore/shader/"; 

   extern const char kShader_name[] = "mars_terr"; 

   extern const char* kShader_ext[] = {
      "_vp.glsl", 
      "_tc.glsl", 
      "_te.glsl", 
      "_fp.glsl", 
      }; 

   const GLuint kShader_types[] = {
      GL_VERTEX_SHADER           ,  
      GL_TESS_CONTROL_SHADER     , 
      GL_TESS_EVALUATION_SHADER  , 
      GL_FRAGMENT_SHADER         , 
      }; 

   const int      kTexture_dim   = 1024; 
   const int      kNum_X_tiles   = (6900  / kTexture_dim) + (6900  % kTexture_dim? 1 : 0); 
   const int      kNum_Y_tiles   = (17177 / kTexture_dim) + (17177 % kTexture_dim? 1 : 0); 

   const unsigned kWd            = 6900 ;
   const unsigned kHt            = 17177;
}

//// 
//



// long beach mini FAX
// 562 216 6642

//
////
template<
   size_t   Depth, 
   size_t   Row, 
   size_t   Col, 
   typename Ty, 
   typename MatTy = Ma::Matrix<Row, Col, Ty> > 

struct Cubetrix {
   
   typedef Ty ValueType; 
 
   enum { NDepth  = Depth } ;
   enum { NHeight = MatTy::NRows };  
   enum { NWidth  = MatTy::NCols };

   Cubetrix () : m () { }
      // 
      // subscript 
            MatTy& operator [] (size_t rindex) { return m[rindex]; }
      const MatTy& operator [] (size_t rindex ) const { return m[rindex]; }
		// :::::::::::::::::::::::
   Ty* ptr () { 
			return reinterpret_cast<Ty*> (this); 
			}

	const Ty* ptr () const { 
		return reinterpret_cast<const Ty*> (this); 
	   }

   Ma::Matrix<Row, Col, Ty> m;
};



template<
   typename Ty,
   size_t NDepth, 
   size_t NHeight, 
   size_t NWidth>
struct array3D : std::array<std::array<std::array<Ty, NWidth>, NWidth>, NDepth> {

   typedef Ty value_type;

   //typedef Ty 
   enum { NDepth = NDepth }; 
   enum { NHeight= NHeight}; 
   enum { NWidth = NWidth }; 
   };

//
////
struct Spatial_sector 
{

   
}; 


typedef array3D<Spatial_sector*, 3, 4, 5> Sim_space;

//
//
struct View_struc 
{
   Ma::Vec3d pos; 
   Ma::Vec3d rot; 
   double dist_near; 
   double dist_far; 
   double FoV_rad; 
   double aspect; 
}; 


const size_t num_X_tiles = 7 ;
const size_t num_Y_tiles = 17;

//
// exper_alpha 
class exper_alpha : public sy::RT_window_listener 
   {
public: 

   static void wat () 
   {
      int i = 0; 
      i++; 
   }

   static void die () 
   {
      fprintf(stderr,  "\n....This is the end."); 
      0[(int*)0] = 0;
   }

   static const std::string kImagePath_height   ; //= L"C:/Quarantine/Textures/hgt/mountains512.png";
   static const std::string kImagePath_color    ; //= L"C:/Quarantine/Textures/hgt/mountains512.hgt.png";

   exper_alpha (); 


   virtual ~exper_alpha () {}

		// 
	virtual int		Initialize	      (sy::System_context*); 
	virtual int		Deinitialize      (sy::System_context*); 
	virtual int		Update	         (sy::System_context*); 

	virtual void   OnWindowResize    (int wd, int ht); 
	virtual void   OnWindowClose		(); 
	virtual void   OnWindowActivate	(bool activate); 

private: 

   void update_input    (sy::System_context*); 
   void update_render   (sy::System_context*); 

   typedef std::map<std::string, GLuint> ObjectMap;         
   ObjectMap                              objIDs; 
   View_struc                             view; 
   Ma::Vec2ui                             viewport_pos; 
   Ma::Vec2ui                             viewport_dim; 

   std::shared_ptr<sy::OpenGL_system>     gl;
   std::shared_ptr<sy::Graphics_window>   windo; 

   typedef Ma::Matrix<num_Y_tiles, num_X_tiles, GLuint> TextureTable; 
   std::shared_ptr<TextureTable> colTbl ;
   std::shared_ptr<TextureTable> hgtTbl ;

   typedef std::map<std::string, GLint>   AttributeMap; 
   typedef std::map<std::string, GLuint>  UniformMap; 

   AttributeMap   terr_attrib_map; 
   UniformMap     terr_uniform_map; 

   Sim_space sim; 
   }; 

const std::string exper_alpha :: kImagePath_height = "C:/Quarantine/Textures/hgt/mountains512.png";
const std::string exper_alpha :: kImagePath_color  = "C:/Quarantine/Textures/hgt/mountains512.hgt.png";


//
//// 
exper_alpha::exper_alpha ()  
   : colTbl (new TextureTable)
   , hgtTbl (new TextureTable)
{

   wat (); 

#if GENERATE_MARS_TILES 
   process_mars_terrain_for_runtime  (); 
   die();
   #endif


#if RUN_QUANTIZE_HEIGHT_TILES_TO_U16 
   printf ("\nbefore quantize_height_tiles_to_u16 ()");
   quantize_height_tiles_to_u16 (); 
   printf ("\nafter quantize_height_tiles_to_u16 ()");
   die ();
   #endif

}

//
//

//
//// 
int exper_alpha::Initialize (sy::System_context* sc) 
{ 
   using namespace glm; 

   char some_abcs[] = "ABCDEFG"; 

      void; 

   windo.reset (sc->Create_GraphicsWindow(this, "EXP0", sy::Graphics_window::kDef_windowed_width, sy::Graphics_window::kDef_windowed_height, false));
   windo->Show (true); 
   gl.reset (sy::Create_OpenGL_system ());
   GLenum glew_res = ::glewInit (); 

   ::glewGetExtension ("GL_ARB_separate_shader_objects"); 
   ////
   //
   // 

   typedef Ma::Matrix<mars_terr::kNum_Y_tiles, mars_terr::kNum_X_tiles, GLuint> TextureTable; 

   TextureTable& colRef = *(colTbl.get()); 
   TextureTable& hgtRef = *(hgtTbl.get()); 
   
   size_t num_tiles = mars_terr::kNum_X_tiles * mars_terr::kNum_Y_tiles;

   //Spatial_sector** sector = sim.ptr(); 

   const size_t npxls = mars_terr::kWd * mars_terr::kHt; 

   glGenTextures (num_tiles, colRef.ptr()); 
   glGenTextures (num_tiles, hgtRef.ptr()); 

   for (size_t iy = 0; iy < mars_terr::kNum_Y_tiles; iy++)
      for (size_t ix = 0; ix < mars_terr::kNum_X_tiles; ix++)
         for (size_t itx = 0; itx < 2; itx++)
   {
      std::stringstream oss; 
      oss << mars_terr::kBase_path << mars_terr::kTile_subp << mars_terr::kType[itx] << iy << "_" << ix << ".dat"; 
      std::string             fname = oss.str();

      size_t sizeOf_file = ut::SizeOf_file (fname);

      if (sizeOf_file)
      {
         std::stringstream fnss; 
         fnss << mars_terr::kType[itx] << iy << "_" << ix << ".dat";  
         std::string curfname = fnss .str();
         printf ("\n%s is present.", curfname.c_str());  
      }
      else
      {
         std::stringstream fnss; 
         fnss << mars_terr::kType[itx] << iy << "_" << ix << ".dat";  
         std::string curfname = fnss .str();
         printf ("\nError: %s has a problem", curfname.c_str());  
         BOOST_ASSERT (ut::SizeOf_file (fname)); 
      }

      //std::shared_ptr<FILE>   indat (fopen (fname.c_str(), "rb"), fclose);
      //std::vector<float>      fbuf (mars_terr::kWd * mars_terr::kHt); 
   }

   //
   // setup viewport fields 
   Ma::Set(view.pos, 0.0, 0.0, 0.0); 
   Ma::Set(view.rot, 0.0, 0.0, 0.0); 
   view.FoV_rad      = Ma::Pi / 3.0; 
   view.aspect       = double(sy::Graphics_window::kDef_windowed_width) / double(sy::Graphics_window::kDef_windowed_height); 
   view.dist_near    = 10.0; 
   view.dist_far     = 5000.0; 

   Ma::Set (viewport_pos, unsigned (0), unsigned (0)); 
   Ma::Set (viewport_dim, sy::Graphics_window::kDef_windowed_width, sy::Graphics_window::kDef_windowed_height); 
   //
   //  OpenGL 
   std::map<std::string, int> avail_feat; 
   std::map<std::string, int> avail_vers;
   {
      avail_feat  ["GL_ARB_tessellation_shader"]  =      glewIsExtensionSupported ("GL_ARB_tessellation_shader") ;
      avail_feat  ["GL_tessellation_shader"    ]  =      glewIsExtensionSupported ("GL_tessellation_shader")     ;
      avail_feat  ["GL_SGIX_clipmap"           ]  =      glewIsExtensionSupported ("GL_SGIX_clipmap")              ;
      avail_feat  ["not a feature"             ]  =      glewIsExtensionSupported ("not a feature")              ;

      avail_vers  ["GLEW_VERSION_3_0"]            =      GLEW_VERSION_3_0; 
      avail_vers  ["GLEW_VERSION_3_1"]            =      GLEW_VERSION_3_1; 
      avail_vers  ["GLEW_VERSION_3_2"]            =      GLEW_VERSION_3_2; 
      avail_vers  ["GLEW_VERSION_4_0"]            =      GLEW_VERSION_4_0;
      avail_vers  ["GLEW_VERSION_4_1"]            =      GLEW_VERSION_4_1;
      avail_vers  ["GLEW_VERSION_4_2"]            =      GLEW_VERSION_4_2;
      avail_vers  ["GLEW_VERSION_4_3"]            =      GLEW_VERSION_4_3;
      avail_vers  ["GLEW_VERSION_4_4"]            =      GLEW_VERSION_4_4;
   };

   // 
   // create shaders
   //objIDs["shader_vert"]      = glCreateShader (GL_VERTEX_SHADER); 
   //objIDs["shader_frag"]      = glCreateShader (GL_FRAGMENT_SHADER);
   //objIDs["shader_tessContr"] = glCreateShader (GL_TESS_CONTROL_SHADER);
   //objIDs["shader_tessEval"]  = glCreateShader (GL_TESS_EVALUATION_SHADER);
   // load and build program
   printf ("\n\n"); 


   // setup shaders 
   for (int ish = 0; ish < 4; ish++) 
   {
      std::ostringstream oss;    
      oss   << SHADER_DRIVE
            << mars_terr::kShader_subp
            << mars_terr::kShader_name 
            << mars_terr::kShader_ext[ish];  

      int         sizeOf_file = ut::SizeOf_file(oss.str ()); 
      std::string shader_name = mars_terr::kShader_subp;
      shader_name += mars_terr::kShader_name; 
      shader_name += mars_terr::kShader_ext[ish]; 

printf ("\nshader: %s == %i bytes", shader_name.c_str(), sizeOf_file); 

      std::vector<GLchar> src_txt; 
      ut::Load_text_file (src_txt, oss.str ()); 

      objIDs[shader_name] = gl->Create_shader (
         src_txt.data(), 
         mars_terr::kShader_types[ish]
         ); 

      // gl->Create_shader

      wat ();

   }


   //GLuint mars_prog = glCreateProgram (); 
   //
   //glUseProgram (mars_prog); 
   GLuint mars_shaders[5] = {0}; 
   int ish = 0; 
   for (ObjectMap::const_iterator it = objIDs.begin(); it != objIDs.end(); it++) 
      mars_shaders[ish++] = it->second; 
   
   
   
   // 
      
   objIDs["mars_terr_prog"] = gl->Build_shader_program (mars_shaders); 

   //glUseProgram (mars_prog); 

   GLchar attrib_position[] = "attrib_position"; 
   GLchar attrib_texcoord[] = "attrib_texcoord"; 
   //
   terr_attrib_map[attrib_position] = glGetAttribLocation (objIDs["mars_terr_prog"], attrib_position), 
   terr_attrib_map[attrib_texcoord] = glGetAttribLocation (objIDs["mars_terr_prog"], attrib_texcoord), 
   // contants
   terr_uniform_map["colorMap"]     = glGetUniformLocation (objIDs["mars_terr_prog"], "colorMap"     );       
   terr_uniform_map["heightMap"]    = glGetUniformLocation (objIDs["mars_terr_prog"], "heightMap"    );       
   terr_uniform_map["heightScale"]  = glGetUniformLocation (objIDs["mars_terr_prog"], "heightScale"  );       
   terr_uniform_map["mat_Model"]    = glGetUniformLocation (objIDs["mars_terr_prog"], "mat_Model"    );      
   terr_uniform_map["mat_View"]     = glGetUniformLocation (objIDs["mars_terr_prog"], "mat_View"     );      
   terr_uniform_map["mat_Proj"]     = glGetUniformLocation (objIDs["mars_terr_prog"], "mat_Proj"     );      


   size_t sizeOf_dmat4 = sizeof(dmat4); 
   size_t sizeOf_dvec3 = sizeof(dvec3); 
   

   wat ();
   // !!!! Remember what to do with the sampler location !!!! 
   //
   //    -> glUniform1i (uniformLoc_map["heightMap"], texture_stage);
   //

   //
   // query attrib. and uniforms
   // setup geometry

   return 0; 
   }

//
//
int exper_alpha::Deinitialize(sy::System_context* sc)
{ 
   return 0; 
}



void exper_alpha::update_input (sy::System_context* sc)
{
   sy::Mouse_state ms; 
   sy::Keyboard_state kb;

   sc->Poll_input (ms, kb); 

}


void exper_alpha::update_render (sy::System_context* sc)
{
   using namespace glm; 
   // Eric C. ..
   // Typically, cracked screens and water damage are considered out of warranty. 
   // Sometimes the phone agents have options for you so you would need to call 
   // 1-855-836-3987 and talk to one of them. They simply have more tools than 
   // I do!
 

   GLbitfield clear_flags =  
         GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT; 

   gl->Clear      (clear_flags); 
   gl->UseProgram (objIDs["mars_terr_prog"]); 

   // gl->Uniform4d
   // HR Workday 844 727 3375
   // 1. Support GS financial goals, sales orders and gross margin.
   // 2. Support IRTP financial goals, sales orders and gross margin.
   // 3. Support inter-company activities by meeting with people outside IRTP, provide input to proposal/IRAD, L & L.
   // 4. Make four signature document presentations with positive project manager feedback, provide document or presentation to customer. 
   // 5. Produce two significant work products within schedule and budget.


   glEnableVertexAttribArray (0); 
   glEnableVertexAttribArray (1); 
   glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, value_ptr(*mars_terr::kPatch_verts)); 
   glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, value_ptr(*mars_terr::kPatch_txcrd)); 


   glDrawArrays(GL_PATCHES, 0, 4); 

   windo->Swap_buffers (); 
}
//
//
int exper_alpha::Update (sy::System_context* sc) 
{
   // process input
   update_input (sc); 

   // update states

   // render 
   update_render (sc); 

   return 0; 
}

//
//
void exper_alpha::OnWindowResize	(int wd, int ht) 
{
   Ma::Set (viewport_dim, unsigned (wd), unsigned (ht) ); 
}
 

//
//
void exper_alpha::OnWindowClose () 
{
}

//
//
void exper_alpha::OnWindowActivate(bool activate) 
{
}


//
// 
int _tmain (int argc, _TCHAR* argv[])
{
   std::shared_ptr<sy::System_context>       sc (sy::Create_context ());
   std::shared_ptr<sy::RT_window_listener>   run (new exper_alpha);
   sy::Run_realtime_task (sc.get(), run.get()); 
	return 0;
}

//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//

#if DEVIL_LIB_IS_AVAILABLE // Devil/ResIL not available in 64 binary

//
//// 
class DevIL
   {
public: 
   DevIL () { ilInit (); }

   ~DevIL () { ilShutDown(); }

   std::vector<unsigned char>& get_data (std::vector<unsigned char>& out, const std::string& fname) const
   {
      out.clear (); 
      ILuint imgID = ilGenImage (); 

      ilBindImage (imgID); 
      if (IL_FALSE == ilLoadImage (fname.c_str ()))
         return out; 
      

      GLuint sizeOf_data = ilGetInteger (IL_IMAGE_SIZE_OF_DATA  );   

      out.resize (sizeOf_data, 0); 
      
      ILubyte* data = ilGetData (); 
      std::copy(data , data + sizeOf_data, &out[0]); 
      ilDeleteImage (imgID); 

      return out; 

   } 

   bool image_properties (std::map<std::string, int>& iprops, const std::string& fname) const
   {
      iprops.clear();
      
      ILuint imgID = ilGenImage (); 

      ilBindImage (imgID); 
      if (IL_FALSE == ilLoadImage (fname.c_str ()))
      {
        ILenum erro_no = ilGetError ();  
        return false; 
      }
      
      iprops["IL_IMAGE_BITS_PER_PIXEL"]   = ilGetInteger (IL_IMAGE_BITS_PER_PIXEL);   
      iprops["IL_IMAGE_WIDTH"         ]   = ilGetInteger (IL_IMAGE_WIDTH         );   
      iprops["IL_IMAGE_HEIGHT"        ]   = ilGetInteger (IL_IMAGE_HEIGHT        );   
      iprops["IL_IMAGE_BPP"           ]   = ilGetInteger (IL_IMAGE_BPP           );   
      iprops["IL_IMAGE_DEPTH"         ]   = ilGetInteger (IL_IMAGE_DEPTH         );   
      iprops["IL_IMAGE_CHANNELS"      ]   = ilGetInteger (IL_IMAGE_CHANNELS      );   
      iprops["IL_IMAGE_FORMAT"        ]   = ilGetInteger (IL_IMAGE_FORMAT        );   
      iprops["IL_IMAGE_TYPE"          ]   = ilGetInteger (IL_IMAGE_TYPE          );   
      iprops["IL_IMAGE_FORMAT"        ]   = ilGetInteger (IL_IMAGE_FORMAT        ); 
      iprops["IL_IMAGE_TYPE"          ]   = ilGetInteger (IL_IMAGE_TYPE          );   
      iprops["IL_IMAGE_SIZE_OF_DATA"  ]   = ilGetInteger (IL_IMAGE_SIZE_OF_DATA  );   

      ilDeleteImage (imgID); 
      return true; 
   }

   std::string enum_2_string (int i) const
   {
      static std::map<int, std::string> IL_define_map; 

      if ( !IL_define_map.size() )
      {
         IL_define_map[IL_COLOUR_INDEX]      = "IL_COLOUR_INDEX"    ;
         IL_define_map[IL_COLOR_INDEX]       = "IL_COLOR_INDEX"     ;
         IL_define_map[IL_ALPHA]             = "IL_ALPHA"           ;
         IL_define_map[IL_RGB]               = "IL_RGB"             ;
         IL_define_map[IL_RGBA]              = "IL_RGBA"            ;
         IL_define_map[IL_BGR]               = "IL_BGR"             ;
         IL_define_map[IL_BGRA]              = "IL_BGRA"            ;
         IL_define_map[IL_LUMINANCE]         = "IL_LUMINANCE"       ;
         IL_define_map[IL_LUMINANCE_ALPHA]   = "IL_LUMINANCE_ALPHA" ;
         IL_define_map[IL_BYTE]              = "IL_BYTE"            ;
         IL_define_map[IL_UNSIGNED_BYTE]     = "IL_UNSIGNED_BYTE"   ;
         IL_define_map[IL_SHORT]             = "IL_SHORT"           ;
         IL_define_map[IL_UNSIGNED_SHORT]    = "IL_UNSIGNED_SHORT"  ;
         IL_define_map[IL_INT]               = "IL_INT"             ;
         IL_define_map[IL_UNSIGNED_INT]      = "IL_UNSIGNED_INT"    ;
         IL_define_map[IL_FLOAT]             = "IL_FLOAT"           ;
         IL_define_map[IL_DOUBLE]            = "IL_DOUBLE"          ;
         IL_define_map[IL_HALF]              = "IL_HALF"            ;
      }

      return IL_define_map.count (i) ? IL_define_map[i] : std::string("");  
   }
   
protected:
   }; 
#endif





