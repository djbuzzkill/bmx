#include "quantize_height_tiles_to_u16.h"
#include <map>
#include <list>
#include <vector>
#include <string>
#include <boost/shared_array.hpp>

void quantize_height_tiles_to_u16 ()
{
   // Step 1 - cut a hole in the box
   // compute statistics about the original source image
   // values below -2330.0 is a masked value
   
   const int height_index = 1; 

   const size_t npxls = mars_terr::kWd * mars_terr::kHt; 

   std::vector<float> v (npxls, 0.0f); 
   size_t ntiles = mars_terr::kNum_Y_tiles * mars_terr::kNum_X_tiles; 

   const size_t sizeOf_src = npxls * sizeof(float) ; 
   const size_t sizeOf_dst = npxls * sizeof(unsigned short) ; 
   int tile_count = 0;
   for (size_t ity = 0; ity < 2; ity++)
   {
      
      v.clear();
      const std::string kHeight_name = mars_terr::kFiles[height_index];  
       
 printf ("\nopen tif: %s", mars_terr::kFiles[ity]);
     FIBITMAP* img = FreeImage_Load (mars_terr::kFIF_fmt[ity], mars_terr::kFiles[ity]);

      Ut::ptru dat; 
      if (img) 
         dat.v = FreeImage_GetBits (img) ; 
      else 
         BOOST_ASSERT (0); 

printf ("\nfilter mask");
      const float mask_val = -2552.0f;;
      for (size_t i = 0; i < npxls ; i++)
      {
         if (dat.f[i] > mask_val )
            v.push_back (dat.f[i]);
      }

      FreeImage_Unload (img);

printf ("\ncompute stats");

      float min_val        = *std::min_element (v.begin(), v.end());
      float max_val        = *std::max_element (v.begin(), v.end());
      float val_range  = max_val - min_val;

printf ("\n   min_val  : %f", min_val    );
printf ("\n   max_val  : %f", max_val    );
printf ("\n   val_range: %f", val_range  );

printf ("\nbegin generating tiles");
      const size_t ntilepxls = mars_terr::kTexture_dim * mars_terr::kTexture_dim; 
      for (unsigned iy = 0; iy < mars_terr::kNum_Y_tiles; iy++) 
         for (unsigned ix = 0; ix < mars_terr::kNum_X_tiles; ix++) 
      { 

         std::ostringstream src_ss , dst_ss; 
         std::shared_ptr<float>            fbuf  (new float[ntilepxls]); 
         std::shared_ptr<unsigned short>   usbuf (new unsigned short[ntilepxls]); 

         // convert this
         src_ss << mars_terr::kBase_path << mars_terr::kTile_subp << mars_terr::kType[ity] << iy << "_" << ix << ".dat"; 
         std::shared_ptr<FILE> srcf (fopen (src_ss.str().c_str (), "rb"), fclose); 
         fread (fbuf.get(), sizeof(float), ntilepxls, srcf.get()); 
      
         // into this
         dst_ss << mars_terr::kBase_path << mars_terr::kTile_subp << mars_terr::kType[ity] << iy << "_" << ix << ".u16"; 
         std::shared_ptr<FILE> dstf (fopen (dst_ss.str().c_str (), "wb"), fclose); 

printf ("\n   tile (%i)", tile_count);
printf (" : %s --> %s", src_ss.str().c_str (), dst_ss.str().c_str ());


         // just remap the masked pixels to 0
         std::transform (
            fbuf.get(), 
            fbuf.get() + ntilepxls, 
            fbuf.get(), 
            [&] (float f) { return (f > mask_val  ? f : 0.0f); }
            ); 

         //
         float range_multiplier = 65535.0f / val_range; 
         std::transform (
            fbuf.get(), 
            fbuf.get() + ntilepxls, 
            usbuf.get(), 
            [&] (float f) { return unsigned short(range_multiplier * (f - min_val)); }
            ); 

         fwrite (usbuf.get(), sizeof (unsigned short), ntilepxls, dstf.get()); 
         tile_count++;

      }
printf ("\nfinish tiles ");

   }
printf ("\nwat");

}
