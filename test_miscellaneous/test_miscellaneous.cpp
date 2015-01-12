// test_miscellaneous.cpp : Defines the entry point for the console application.



#include <Dx/Dx.h>
#include "IL/IL.h"
#include <tuple>
#include <initializer_list>

struct error_msg
{
   int num; 
   char* msg; 
}


const static kError_msg[] = {
   { IL_INVALID_ENUM         , "IL_INVALID_ENUM"         } ,         
   { IL_OUT_OF_MEMORY        , "IL_OUT_OF_MEMORY"        } ,        
   { IL_FORMAT_NOT_SUPPORTED , "IL_FORMAT_NOT_SUPPORTED" } , 
   { IL_INTERNAL_ERROR       , "IL_INTERNAL_ERROR"       } ,       
   { IL_INVALID_VALUE        , "IL_INVALID_VALUE"        } ,        
   { IL_ILLEGAL_OPERATION    , "IL_ILLEGAL_OPERATION"    } ,    
   { IL_ILLEGAL_FILE_VALUE   , "IL_ILLEGAL_FILE_VALUE"   } ,   
   { IL_INVALID_FILE_HEADER  , "IL_INVALID_FILE_HEADER"  } ,  
   { IL_INVALID_PARAM        , "IL_INVALID_PARAM"        } ,        
   { IL_COULD_NOT_OPEN_FILE  , "IL_COULD_NOT_OPEN_FILE"  } ,  
   { IL_INVALID_EXTENSION    , "IL_INVALID_EXTENSION"    } ,    
   { IL_FILE_ALREADY_EXISTS  , "IL_FILE_ALREADY_EXISTS"  } ,   
   { IL_OUT_FORMAT_SAME      , "IL_OUT_FORMAT_SAME"      } ,      
   { IL_STACK_OVERFLOW       , "IL_STACK_OVERFLOW"       } ,       
   { IL_STACK_UNDERFLOW      , "IL_STACK_UNDERFLOW"      } ,      
   { IL_INVALID_CONVERSION   , "IL_INVALID_CONVERSION"   } ,   
   { IL_BAD_DIMENSIONS       , "IL_BAD_DIMENSIONS"       } ,       
   { IL_FILE_READ_ERROR      , "IL_FILE_READ_ERROR "     } ,     
   { IL_FILE_WRITE_ERROR     , "IL_FILE_WRITE_ERROR"     } ,     

   { IL_LIB_GIF_ERROR  , "IL_LIB_GIF_ERROR"}, 
   { IL_LIB_JPEG_ERROR , "IL_LIB_JPEG_ERROR"}, 
   { IL_LIB_PNG_ERROR  , "IL_LIB_PNG_ERROR"}, 
   { IL_LIB_TIFF_ERROR , "IL_LIB_TIFF_ERROR"}, 
   { IL_LIB_MNG_ERROR  , "IL_LIB_MNG_ERROR"}, 
   { IL_LIB_JP2_ERROR  , "IL_LIB_JP2_ERROR"}, 
   { IL_LIB_EXR_ERROR  , "IL_LIB_EXR_ERROR"}, 
   { IL_UNKNOWN_ERROR  , "IL_UNKNOWN_ERROR"}, 
}; 


typedef int (*FnType) (float f); 

int Fun (float f) 
{
   return (int) f; 
}

int Foo (float f) 
{
   return (int) (f + 69.0f); 
}


void deferred_fn (FnType f) 
{
   float pi = 3.14159f;
   
   printf ( "\nsome value: %i\n", f(pi)); 


}

const char* Get_IL_error (ILenum err)
   {
http://www.awaresystems.be/imaging/tiff.html

   FnType myfn = Fun ; 

   deferred_fn (Foo);
   deferred_fn (Fun); 

   for (int i = 0; i < El_count(kError_msg); i++)
      if (kError_msg[i].num == err)
         return kError_msg[i].msg; 

   return 0; 
   }

//
//
class test_miscellaneous : public sy::RT_window_listener
   {
public: 

   test_miscellaneous (sy::System_context* sys) 
   {
      int dosomeshit = 0; 
      const char* err_msg = IL_NO_ERROR; 
      
{
   struct tup { 
   int a; 
   std::string b; 
   double c; 
   }; 

   tup t = {1, "hello", Ma::Pi}; 
   t.a; 
   t.b;
   t.c; 

typedef std::tuple<int, std::string, double> MyTup; 
MyTup test (1, "hello", Ma::Pi); 
auto val1 = std::get<0>(test); 
auto val2 = std::get<1>(test); 
auto val3 = std::get<2>(test); 

}
      std::wstring fn_Mars_red   = L"C:/Quarantine/Mars/ESP_018065_1975_RED_ESP_019133_1975_RED-DRG.tif"; 
      std::wstring fn_Mars_hgt   = L"C:/Quarantine/Mars/ESP_018065_1975_RED_ESP_019133_1975_RED-DEM.tif"; 
      std::wstring fn_png        = L"C:/Quarantine/Textures/liquid_metal_texture_011.png"; 
      ilInit(); 

      std::wstring hgtm =  L"C:/Quarantine/Textures/hgt/mountains512.png";


      ILuint imgID = ilGenImage (); 
      ILuint hgtID = ilGenImage (); 
      ILuint pngID = ilGenImage (); 

      ilBindImage (pngID);          
      std::map<std::string, ILint> img_props; 

      if (ilLoad (IL_PNG, hgtm.c_str()))
      {

img_props["IL_IMAGE_WIDTH           "] = ilGetInteger (IL_IMAGE_WIDTH           ); // 0x0DE4
img_props["IL_IMAGE_HEIGHT          "] = ilGetInteger (IL_IMAGE_HEIGHT          ); // 0x0DE5
img_props["IL_IMAGE_DEPTH           "] = ilGetInteger (IL_IMAGE_DEPTH           ); // 0x0DE6
img_props["IL_IMAGE_SIZE_OF_DATA    "] = ilGetInteger (IL_IMAGE_SIZE_OF_DATA    ); // 0x0DE7
img_props["IL_IMAGE_BPP             "] = ilGetInteger (IL_IMAGE_BPP             ); // 0x0DE8
img_props["IL_IMAGE_BYTES_PER_PIXEL "] = ilGetInteger (IL_IMAGE_BYTES_PER_PIXEL ); // 0x0DE8
img_props["IL_IMAGE_BPP             "] = ilGetInteger (IL_IMAGE_BPP             ); // 0x0DE8
img_props["IL_IMAGE_BITS_PER_PIXEL  "] = ilGetInteger (IL_IMAGE_BITS_PER_PIXEL  ); // 0x0DE9
img_props["IL_IMAGE_FORMAT          "] = ilGetInteger (IL_IMAGE_FORMAT          ); // 0x0DEA
img_props["IL_IMAGE_TYPE            "] = ilGetInteger (IL_IMAGE_TYPE            ); // 0x0DEB
img_props["IL_PALETTE_TYPE          "] = ilGetInteger (IL_PALETTE_TYPE          ); // 0x0DEC
img_props["IL_PALETTE_SIZE          "] = ilGetInteger (IL_PALETTE_SIZE          ); // 0x0DED
img_props["IL_PALETTE_BPP           "] = ilGetInteger (IL_PALETTE_BPP           ); // 0x0DEE
img_props["IL_PALETTE_NUM_COLS      "] = ilGetInteger (IL_PALETTE_NUM_COLS      ); // 0x0DEF

       dosomeshit++;
      }
      else
      {
         err_msg = Get_IL_error (ilGetError ()); 
      }

//      ilBindImage (imgID); 
//      if (ilLoadImage  (hgtm[1].c_str()))
//      {
//         dosomeshit++;
//      }
//      else
//      {
//         err_msg = Get_IL_error (ilGetError ()); 
//      }
 
      ilBindImage (hgtID); 
      if (ilLoadImage (fn_Mars_hgt.c_str()))
      {
         dosomeshit++;
      }
      else
      {
         err_msg = Get_IL_error (ilGetError ()); 
      }


      ilDeleteImage (imgID); 
      ilDeleteImage (hgtID); 

      sys->SysReq (sy::System_context::Req_Exit); 
      }

		virtual void OnWindowResize		(int wd, int ht) {}
		virtual void OnWindowClose		() {}
		virtual void OnWindowActivate	(bool activate) {}
      
      virtual ~test_miscellaneous   () {}
		virtual int Initialize        (sy::System_context* s) { return 0; }
		virtual int Update            (sy::System_context* s) { return 0; }
		virtual int	Deinitialize      (sy::System_context* s) { return 0; }
}; 


sy::RT_window_listener* Create_test (sy::System_context* s)
{
   return new test_miscellaneous (s); 
}
