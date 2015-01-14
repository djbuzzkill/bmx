

#include "stdafx.h"
#include "test_GL4_experiment.h"


static void wat () 
{
   int i = 0; 
   i++; 
}


#define IL_FALSE			0
#define IL_TRUE				1

//  Matches OpenGL's right now.
//! Data formats \link Formats Formats\endlink

//
// exper_alpha 
class exper_alpha : public sy::RT_window_listener 
   {
public: 

   static const std::string kImagePath_height   ; //= L"C:/Quarantine/Textures/hgt/mountains512.png";
   static const std::string kImagePath_color    ; //= L"C:/Quarantine/Textures/hgt/mountains512.hgt.png";

   exper_alpha () {}
   virtual ~exper_alpha () {}

		// 
	virtual int		Initialize	   (sy::System_context*); 
	virtual int		Deinitialize   (sy::System_context*); 
	virtual int		Update	      (sy::System_context*); 

	virtual void OnWindowResize		(int wd, int ht); 
	virtual void OnWindowClose		(); 
	virtual void OnWindowActivate	(bool activate); 

private: 
   std::shared_ptr<sy::OpenGL_system>     glsys;
   std::shared_ptr<sy::Graphics_window>   windo; 

}; 

const std::string exper_alpha :: kImagePath_height = "C:/Quarantine/Textures/hgt/mountains512.png";
const std::string exper_alpha :: kImagePath_color  = "C:/Quarantine/Textures/hgt/mountains512.hgt.png";


#define DEVIL_LIB_IS_AVAILABLE 1

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
         return false; 
      
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

//
//// 
int exper_alpha::Initialize (sy::System_context* sc) 
{ 
   windo.reset (sc->Create_GraphicsWindow(this, "EXP0", 1024, 768, false));
   windo->Show (true); 
   glsys.reset(sy::Create_OpenGL_system());
   GLenum glew_res = ::glewInit (); 
   ////
   //
   // 



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
   std::map<std::string, GLuint> objIDs ; 
   objIDs["sh_vertex"]           = glCreateShader (GL_VERTEX_SHADER); 
   objIDs["sh_fragment"]         = glCreateShader (GL_FRAGMENT_SHADER);
   objIDs["sh_tess_control"]     = glCreateShader (GL_TESS_CONTROL_SHADER);
   objIDs["sh_tess_evalution"]   = glCreateShader (GL_TESS_EVALUATION_SHADER);
   glGenProgramPipelines (1, &objIDs["prog_pipeline"]);
   wat ();

   //
   GLint max_txr_size = 0; 
   glGetIntegerv (GL_MAX_TEXTURE_SIZE, &max_txr_size );  
   GLuint txrIDs[10] =  {0}; 
   glGenTextures (10, txrIDs); 


   return 0; 
   }

//
//
int exper_alpha::Deinitialize(sy::System_context* sc)
{ 
   return 0; 
}

//
//
int exper_alpha::Update (sy::System_context* sc) 
{
   return 0; 
}

//
//
void exper_alpha::OnWindowResize	(int wd, int ht) {} 

//
//
void exper_alpha::OnWindowClose	() {}

//
//
void exper_alpha::OnWindowActivate(bool activate) {}

//
// 
int _tmain (int argc, _TCHAR* argv[])
{
   std::shared_ptr<sy::System_context>       sc (sy::Create_context ());
   std::shared_ptr<sy::RT_window_listener>   run (new exper_alpha);
   sy::Run_realtime_task (sc.get(), run.get()); 
	return 0;
}

