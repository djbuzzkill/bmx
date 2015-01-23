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


#include <Dx/Utility.h>

#define MARS_DRIVE "J:/" 


namespace mars_terr
{
   extern const char                kBase_path[]; 
   extern const char                kTile_path[]; 
   extern const char                kShader_path[]; 
   extern const char                kShader_name[];
   extern const char*               kShader_ext[];
   extern const unsigned int        kShader_types[];
   extern const char*               kFiles[];
   extern const std::string         kType[];
   extern const FREE_IMAGE_FORMAT   kFIF_fmt[];
   extern const size_t              kNum_files; 

   //extern const char*               kBasePath[]; 
   //extern const char*               kTilePath; 

   extern const int                 kTexture_dim; 
   extern const int                 kNum_X_tiles;  
   extern const int                 kNum_Y_tiles;  
   extern const unsigned            kWd; 
   extern const unsigned            kHt; 
}

#endif