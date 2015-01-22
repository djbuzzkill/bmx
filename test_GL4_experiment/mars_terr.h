#ifndef mars_terr_h
#define mars_terr_h

#include <map>
#include <list>
#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include <numeric>
#include <algorithm>

#include <FreeImage.h>

#define MARS_DRIVE "C:/" 


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


namespace mars_terr
{
   extern const char* kFiles[] ;
   //= {
   //   MARS_DRIVE "Quarantine/Mars/ESP_018065_1975_RED_ESP_019133_1975_RED-DRG.tif", 
   //   MARS_DRIVE "Quarantine/Mars/ESP_018065_1975_RED_ESP_019133_1975_RED-DEM.tif",  
   //   MARS_DRIVE "Quarantine/Mars/ESP_018065_1975_RED_ESP_019133_1975_RED-IGM.dat", 
   //   };

   extern const std::string kType[] ;
   //= {
   //   "mars_col_", 
   //   "mars_hgt_", 
   //   "mars_igm_", 
   //   }; 

   extern const FREE_IMAGE_FORMAT kFIF_fmt[] ;
   //= { 
   //   FIF_TIFF, 
   //   FIF_TIFF, 
   //   FIF_UNKNOWN, 
   //   }; 

   extern const size_t   kNum_files    ; ///  = El_count (kFiles); 
   extern const char*    kTilePath     ; ///  = MARS_DRIVE "Quarantine/Mars/tiled/"; 
   extern const int      kTexture_dim  ; ///  = 1024; 
   extern const int      kNum_X_tiles  ; ///  = (6900  / kTexture_dim) + (6900  % kTexture_dim? 1 : 0); 
   extern const int      kNum_Y_tiles  ; ///  = (17177 / kTexture_dim) + (17177 % kTexture_dim? 1 : 0); 
                                       ; /// 
   extern const unsigned kWd           ; ///  = 6900 ;
   extern const unsigned kHt           ; ///  = 17177;
}

#endif