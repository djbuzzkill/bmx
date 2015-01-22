#include "process_mars_terrain_for_runtime.h"

#include <map>
#include <list>
#include <vector>
#include <string>

#include <boost/shared_array.hpp>

#include <FreeImage.h>

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
