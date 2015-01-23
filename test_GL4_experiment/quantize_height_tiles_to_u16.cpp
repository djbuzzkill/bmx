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
   int tile_count = 0;
   for (size_t ity = 0; ity < 2; ity++)
   {
      
      v.clear();
      const std::string kHeight_name = mars_terr::kFiles[height_index];  

      Ut::ptru dat; 
       
      //FIBITMAP* img = FreeImage_Load (mars_terr::kFIF_fmt[ity], "C:/Quarantine/Mars/ESP_018065_1975_RED_ESP_019133_1975_RED-DRG.tif");
      FIBITMAP* img = FreeImage_Load (mars_terr::kFIF_fmt[ity], mars_terr::kFiles[ity]);


printf ("\nopen tif");

      if (img) 
         dat.v = FreeImage_GetBits (img) ; 
      else 
         BOOST_ASSERT (0); 
      

printf ("\nfilter mask");
      const float mask_val = -2552.0f;
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
printf ("\nbegin generating tiles");
      const size_t ntilepxls = mars_terr::kTexture_dim * mars_terr::kTexture_dim; 
      for (unsigned iy = 0; iy < mars_terr::kNum_Y_tiles; iy++) 
         for (unsigned ix = 0; ix < mars_terr::kNum_X_tiles; ix++) 
      { 
printf ("\n   tile (%i)", tile_count);

         

         std::ostringstream src_ss , dst_ss; 
         std::shared_ptr<float>            fbuf  (new float[ntilepxls]); 
         std::shared_ptr<unsigned short>   usbuf (new unsigned short[ntilepxls]); 

         // convert this

         
         src_ss << mars_terr::kBase_path << mars_terr::kTile_path << mars_terr::kType[ity] << iy << "_" << ix << ".dat"; 
         std::shared_ptr<FILE> srcf (fopen (src_ss.str().c_str (), "rb"), fclose); 
         fread (fbuf.get(), sizeof(float), ntilepxls, srcf.get()); 
      
         // into this
         dst_ss << mars_terr::kBase_path << mars_terr::kTile_path << mars_terr::kType[ity] << iy << "_" << ix << ".u16"; 
         std::shared_ptr<FILE> dstf (fopen (dst_ss.str().c_str (), "wb"), fclose); 

         // just remap the masked pixels to 0
         std::transform (
            fbuf.get(), 
            fbuf.get() + ntilepxls, 
            fbuf.get(), 
            [&] (float f) { return (f > mask_val  ? f : 0.0f); }
            ); 

         float inv_mean = 1.0f / avg; 
         std::transform (
            fbuf.get(), 
            fbuf.get() + ntilepxls, 
            usbuf.get(), 
            [&] (float f) { return unsigned short(inv_mean * f); }
            ); 

         fwrite (usbuf.get(), sizeof (unsigned short), ntilepxls, dstf.get()); 
         tile_count++;

      }
printf ("\nfinish tiles ");

   }
printf ("\nwat");

}
