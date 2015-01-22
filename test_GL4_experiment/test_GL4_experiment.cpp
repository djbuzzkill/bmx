

#include "stdafx.h"
#include "test_GL4_experiment.h"

#include <boost/shared_array.hpp>
#include <Dx/VecMath.h>

#define MARS_DRIVE "C:/" 

// convert original large textures to 1024 tiles
void process_mars_terrain_for_runtime ();

// convert f32 tiles to u16; 
void quantize_height_tiles_to_u16 ();

namespace mars_terr
{
   const char* kFiles[] = {
      MARS_DRIVE "Quarantine/Mars/ESP_018065_1975_RED_ESP_019133_1975_RED-DRG.tif", 
      MARS_DRIVE "Quarantine/Mars/ESP_018065_1975_RED_ESP_019133_1975_RED-DEM.tif",  
      MARS_DRIVE "Quarantine/Mars/ESP_018065_1975_RED_ESP_019133_1975_RED-IGM.dat", 
      };

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

   const size_t   kNum_files     = El_count (kFiles); 
   const char*    kTilePath      = MARS_DRIVE "Quarantine/Mars/tiled/"; 
   const int      kTexture_dim   = 1024; 
   const int      kNum_X_tiles   = (6900  / kTexture_dim) + (6900  % kTexture_dim? 1 : 0); 
   const int      kNum_Y_tiles   = (17177 / kTexture_dim) + (17177 % kTexture_dim? 1 : 0); 

   const unsigned kWd            = 6900 ;
   const unsigned kHt            = 17177;
}


//
////
template<
   size_t   Depth, 
   size_t   Row, 
   size_t   Col, 
   typename Ty, 
   typename MatTy = Ma::Matrix<Row, Col, Ty> > 

struct Cubetrix {
   
   typedef Ty ElemType; 
 
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


//
////
struct Spatial_sector 
{

   
}; 

typedef Cubetrix<3, 4, 5, Spatial_sector*> Sim_space;



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

static void wat () 
{
   int i = 0; 
   i++; 
}


union ptru 
{
   void*             v; 
   char*             c;
   unsigned char*    uc;
   short*            s;
   unsigned short*   us;
   int*              i;
   unsigned*         ui;
   float*            f;
   double*           d;
}; 


const size_t num_X_tiles = 7 ;
const size_t num_Y_tiles = 17;
//
// exper_alpha 
class exper_alpha : public sy::RT_window_listener 
   {
public: 

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

   std::map<std::string, GLuint>          objIDs; 
   View_struc                             view; 
   Ma::Vec2ui                             viewport_pos; 
   Ma::Vec2ui                             viewport_dim; 

   std::shared_ptr<sy::OpenGL_system>     glsys;
   std::shared_ptr<sy::Graphics_window>   windo; 

   typedef Ma::Matrix<num_Y_tiles, num_X_tiles, GLuint> TextureTable; 
   std::shared_ptr<TextureTable> colTbl ;
   std::shared_ptr<TextureTable> hgtTbl ;

   Sim_space sim; 
   }; 

const std::string exper_alpha :: kImagePath_height = "C:/Quarantine/Textures/hgt/mountains512.png";
const std::string exper_alpha :: kImagePath_color  = "C:/Quarantine/Textures/hgt/mountains512.hgt.png";

//
//// 
#define GENERATE_MARS_TILES 1

void process_mars_terrain_for_runtime ()
{
   printf ("\nbegin processing"); 
   //
   ("Saturn arch NITF framing camera", "get Cincotta IDL working on aces data"); 
   //
   std::map<FREE_IMAGE_TYPE, int> sizeOf_FI_component; 
   std::map<FREE_IMAGE_TYPE, int> numberOf_FI_components; 
   {
      sizeOf_FI_component[FIT_UNKNOWN]       = -1; 
      sizeOf_FI_component[FIT_BITMAP]        = -1; 
      sizeOf_FI_component[FIT_UINT16]        = 2;
      sizeOf_FI_component[FIT_INT16]         = 2; 
      sizeOf_FI_component[FIT_UINT32]        = 4; 
      sizeOf_FI_component[FIT_INT32]         = 4; 
      sizeOf_FI_component[FIT_FLOAT]         = 4; 
      sizeOf_FI_component[FIT_DOUBLE]        = 8; 
      sizeOf_FI_component[FIT_COMPLEX]       = 8; 
      sizeOf_FI_component[FIT_RGB16]         = 2; 
      sizeOf_FI_component[FIT_RGBA16]        = 2; 
      sizeOf_FI_component[FIT_RGBF]          = 4; 
      sizeOf_FI_component[FIT_RGBAF]         = 4; 
   
      numberOf_FI_components[FIT_UNKNOWN]    = -1;
      numberOf_FI_components[FIT_BITMAP]     = -1;
      numberOf_FI_components[FIT_UINT16]     = 1;  
      numberOf_FI_components[FIT_INT16]      = 1; 
      numberOf_FI_components[FIT_UINT32]     = 1; 
      numberOf_FI_components[FIT_INT32]      = 1; 
      numberOf_FI_components[FIT_FLOAT]      = 1; 
      numberOf_FI_components[FIT_DOUBLE]     = 1; 
      numberOf_FI_components[FIT_COMPLEX]    = 2; 
      numberOf_FI_components[FIT_RGB16]      = 3; 
      numberOf_FI_components[FIT_RGBA16]     = 4; 
      numberOf_FI_components[FIT_RGBF]       = 3; 
      numberOf_FI_components[FIT_RGBAF]      = 4; 
   } 

   std::vector<unsigned char> linebuff; 

   for (unsigned itx = 0; itx < 2; itx++) 
      for (unsigned iy = 0; iy < mars_terr::kNum_Y_tiles; iy++) 
         for (unsigned ix = 0; ix < mars_terr::kNum_X_tiles; ix++) 
   {
      const std::string cur_file    = mars_terr::kFiles[itx]; 

 
 
      // in pixels 
      unsigned x_tile_start   = ix * mars_terr::kTexture_dim; 
      unsigned y_tile_start   = iy * mars_terr::kTexture_dim * mars_terr::kWd; 
      // 
      // upper left (in pixels) of the current tile
      unsigned tile_start     = x_tile_start + y_tile_start; 


      // create out file
      std::ostringstream oss; 
      oss << mars_terr::kTilePath << mars_terr::kType[itx] << iy << "_" << ix << ".dat"; 
      std::shared_ptr<FILE> outf (fopen (oss.str().c_str (), "wb"), fclose); 

      
      unsigned x_count = (ix * mars_terr::kTexture_dim + mars_terr::kTexture_dim) < mars_terr::kWd ? mars_terr::kTexture_dim : mars_terr::kWd % mars_terr::kTexture_dim; 
      unsigned y_count = (iy * mars_terr::kTexture_dim + mars_terr::kTexture_dim) < mars_terr::kHt ? mars_terr::kTexture_dim : mars_terr::kHt % mars_terr::kTexture_dim; 

      if (itx < 2)
      {
         size_t      sizeOf_pixel = mars_terr::kWd * 4;
         FIBITMAP*   img          = FreeImage_Load (mars_terr::kFIF_fmt[itx], mars_terr::kFiles[itx]);  
         ptru        src          = { FreeImage_GetBits (img) }; 

         if (linebuff.size () < (sizeOf_pixel * mars_terr::kTexture_dim))
            linebuff.resize (sizeOf_pixel * mars_terr::kTexture_dim);
         ptru ptr = { linebuff.data() }; 

         //case 0: // color txr
         //case 1: // height txr
         for (unsigned iln = 0; iln < mars_terr::kTexture_dim; iln++)
         {
            std::fill (ptr.f, ptr.f + mars_terr::kTexture_dim, -2553.0f);  

            if (iln < y_count)
            {
               unsigned pixel_begin = iln * mars_terr::kWd + tile_start; 
               unsigned pixel_end   = pixel_begin + x_count; 
               std::copy (src.f + pixel_begin, src.f + pixel_end, ptr.f);
            }

            fwrite (ptr.v, sizeof(float), mars_terr::kTexture_dim, outf.get()); 
         }

         FreeImage_Unload (img); 
      }
      else
      {
 
         size_t sizeOf_pixel = 16; // 2 * sizeof(double)
         boost::shared_array<double>   img (new double[sizeOf_pixel * mars_terr::kWd * mars_terr::kHt]); 
         ptru                          src = { img.get() }; 
         std::shared_ptr<FILE>         infile (fopen (mars_terr::kFiles[itx], "rb"), fclose); 
         fread (src.v, 2 * sizeof(double), mars_terr::kWd * mars_terr::kHt, infile.get()); 

         if (linebuff.size () < (sizeOf_pixel * mars_terr::kTexture_dim))
            linebuff.resize (sizeOf_pixel * mars_terr::kTexture_dim);
         ptru ptr = { linebuff.data() }; 

         
         for (unsigned iln = 0; iln < mars_terr::kTexture_dim; iln++)
         {
            std::fill (ptr.d, ptr.d + 2 * mars_terr::kTexture_dim, -1.0);  

            if (iln < y_count)
            {
               unsigned pixel_begin = iln * mars_terr::kWd + tile_start; 
               unsigned pixel_end   = pixel_begin + x_count; 
               std::copy (src.f + pixel_begin, src.f + pixel_end, ptr.f);
            }

            fwrite (ptr.v, 2 * sizeof(double), mars_terr::kTexture_dim, outf.get()); 
         }
      }
   }
 

   printf ("\ndone processing"); 
   BOOST_ASSERT (0); 

}


//
////
#define RUN_QUANTIZE_HEIGHT_TILES_TO_U16 1
//
void quantize_height_tiles_to_u16 ()
{
   // Step 1 - cut a hole in the box
   // compute statistics about the original source image
   // values below -2330.0 is a masked value
   // find mean, min, max 
   //FreeImage_Initialise ();
   
   const int height_index = 1; 

   const size_t npxls = mars_terr::kWd * mars_terr::kHt; 

   // read something into v
   //BOOST_ASSERT (0); 


   std::vector<float> v (npxls, 0.0f); 


   //float mean = avg; 
   //std::transform (
   //   v.begin(), 
   //   v.end(), 
   //   diff.begin(),
   //   std::bind2nd(std::minus<float>(), mean)
   //   );
   //
   //sqr_sum = std::inner_product (diff.begin(), diff.end(), diff.begin(), 0.0);
   //std_dev = std::sqrt(sqr_sum / v.size());

   //   
   size_t ntiles = mars_terr::kNum_Y_tiles * mars_terr::kNum_X_tiles; 

   const size_t sizeOf_src = npxls * sizeof(float) ; 
   const size_t sizeOf_dst = npxls * sizeof(unsigned short) ; 

   for (size_t ity = 0; ity < 2; ity++)
   {
      
      v.clear();
      const std::string kHeight_name = mars_terr::kFiles[height_index];  

      ptru dat; 
       
      //FIBITMAP* img = FreeImage_Load (mars_terr::kFIF_fmt[ity], "C:/Quarantine/Mars/ESP_018065_1975_RED_ESP_019133_1975_RED-DRG.tif");
      FIBITMAP* img = FreeImage_Load (mars_terr::kFIF_fmt[ity], mars_terr::kFiles[ity]);


printf ("\nopen tif");

      if (img) 
         dat.v = FreeImage_GetBits (img) ; 
      else 
         BOOST_ASSERT (0); 
      

printf ("\nfilter mask");
      const float mask_val = -2553.0f;
      for (size_t i = 0; i < npxls ; i++)
      {
         if (dat.f[i] > mask_val )
            v.push_back (dat.f[i]);
      }

      FreeImage_Unload (img);


printf ("\ncompute stats");

      float min_val     = *std::min_element (v.begin(), v.end());
      float max_val     = *std::max_element (v.begin(), v.end());; 
      float inv_val_range   = 1.0f / (max_val - min_val);

      // we dont do it on the original file
      //std::vector<float> normalized_heights (v.size()); 
      for (size_t i = 0; i < v.size(); i++)
         v[i] =  (v[i] - min_val) * inv_val_range;

      float avg = 0.0f; 
      for (std::vector<float>::iterator it = v.begin(); it != v.end(); it++)
         avg += *it;        

      avg /= float (v.size());
      
printf ("\nbegin generating");
      const size_t ntilepxls = mars_terr::kTexture_dim * mars_terr::kTexture_dim; 
      for (unsigned iy = 0; iy < mars_terr::kNum_Y_tiles; iy++) 
         for (unsigned ix = 0; ix < mars_terr::kNum_X_tiles; ix++) 
      { 
         std::ostringstream src_ss , dst_ss; 
         std::shared_ptr<float>            fbuf  (new float[ntilepxls]); 
         std::shared_ptr<unsigned short>   usbuf (new unsigned short[ntilepxls]); 

         // convert this
         src_ss << mars_terr::kTilePath << mars_terr::kType[ity] << iy << "_" << ix << ".dat"; 
         std::shared_ptr<FILE> srcf (fopen (src_ss.str().c_str (), "rb"), fclose); 
         fread (fbuf.get(), sizeof(float), ntilepxls, srcf.get()); 
      
         // into this
         dst_ss << mars_terr::kTilePath << mars_terr::kType[ity] << iy << "_" << ix << ".u16"; 
         std::shared_ptr<FILE> dstf (fopen (dst_ss.str().c_str (), "wb"), fclose); 

         float inv_mean = 1.0f / avg; 
         std::transform (
            fbuf.get(), 
            fbuf.get() + ntilepxls, 
            usbuf.get(), 
            [&] (float f) { return unsigned short(inv_mean * f); }
            ); 

         fwrite (usbuf.get(), sizeof (unsigned short), ntilepxls, dstf.get()); 
      }
printf ("\nfinish tile ");

   }
printf ("\nwat");

}


//
//// 
exper_alpha::exper_alpha ()  
   : colTbl (new TextureTable)
   , hgtTbl (new TextureTable)
{

   #if GENERATE_MARS_TILES 
   process_mars_terrain_for_runtime  (); 
   assert (0); 
   #endif

   #if RUN_QUANTIZE_HEIGHT_TILES_TO_U16 
   printf ("\nbefore quantize_height_tiles_to_u16 ()");
   quantize_height_tiles_to_u16 (); 
   printf ("\nafter quantize_height_tiles_to_u16 ()");
   assert (0); 
   #endif



}

//
//


//
//// 
int exper_alpha::Initialize (sy::System_context* sc) 
{ 
      void; 

   windo.reset (sc->Create_GraphicsWindow(this, "EXP0", sy::Graphics_window::kDef_windowed_width, sy::Graphics_window::kDef_windowed_height, false));
   windo->Show (true); 
   glsys.reset (sy::Create_OpenGL_system ());
   GLenum glew_res = ::glewInit (); 

   ////
   //
   // 

   typedef Ma::Matrix<mars_terr::kNum_Y_tiles, mars_terr::kNum_X_tiles, GLuint> TextureTable; 

   TextureTable& colRef = *(colTbl.get()); 
   TextureTable& hgtRef = *(hgtTbl.get()); 
   
   size_t num_tiles = mars_terr::kNum_X_tiles * mars_terr::kNum_Y_tiles;

   Spatial_sector** sector = sim.ptr(); 

   const size_t npxls = mars_terr::kWd * mars_terr::kHt; 


   glGenTextures (num_tiles, colRef.ptr()); 
   glGenTextures (num_tiles, hgtRef.ptr()); 

   for (size_t iy = 0; iy < mars_terr::kNum_Y_tiles; iy++)
      for (size_t ix = 0; ix < mars_terr::kNum_X_tiles; ix++)
         for (size_t itx = 0; itx < 2; itx++)
   {
   
   
      std::stringstream oss; 
      oss << mars_terr::kTilePath << mars_terr::kType[itx] << iy << "_" << ix << ".dat"; 
      std::shared_ptr<FILE> intx (fopen (oss.str().c_str (), "rb"), fclose); 
      std::vector<float> fbuf (mars_terr::kWd * mars_terr::kHt); 

//      glTextureImage2DEXT (colRef[iy][ix], GL_TEXTURE_2D, 0, GL_R32F, kTextureDim,kTextureDim, 0, GL_R32F, fbuf.data()); 

      switch (itx) 
      {
      case 0: 

         fread (fbuf.data(), sizeof(float), npxls , intx.get());
         {

         }

      break; 

      case 1:

         fread (fbuf.data(), sizeof(float), npxls , intx.get());

      break;

      default:       
      break; 
      }      


   }
   // GLuint txrIDs[n_y_tiles][n_x_tiles];

   //glGenTextures (wd, txrIDs



   //oss << kTilePath << tile_type[itx] << iy << "_" << ix << ".dat"; 
   //std::shared_ptr<FILE> outf (fopen (oss.str().c_str (), "wb"), fclose); 


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
   // 
   objIDs["shader_vert"]      = glCreateShader (GL_VERTEX_SHADER); 
   objIDs["shader_frag"]      = glCreateShader (GL_FRAGMENT_SHADER);
   objIDs["shader_tessContr"] = glCreateShader (GL_TESS_CONTROL_SHADER);
   objIDs["shader_tessEval"]  = glCreateShader (GL_TESS_EVALUATION_SHADER);



   // glsys->Build_shader_program  
   // glsys->Create_shader

   wat ();

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





