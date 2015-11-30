#ifndef DX_RESOURCE_INCLUDE
#define DX_RESOURCE_INCLUDE

#include <stdio.h>
#include <string>
#include <vector>
#include <map>


namespace Dx
{

   // Resource Data Type
   enum ResourceData
   {
      _INVALID_RESOURCE_DATA = 0, 
      RD_UCHAR,
      RD_VEC2UC,
      RD_VEC3UC,
      RD_VEC4UC,

      RD_CHAR,
      RD_VEC2C,
      RD_VEC3C,
      RD_VEC4C,

      RD_SHORT,
      RD_VEC2S,
      RD_VEC3S,
      RD_VEC4S,

      RD_USHORT,
      RD_VEC2US,
      RD_VEC3US,
      RD_VEC4US,

      RD_FLOAT,
      RD_VEC2F,
      RD_VEC3F,
      RD_VEC4F,

      RD_DOUBLE,
      RD_VEC2D,
      RD_VEC3D,
      RD_VEC4D,

      RD_INT,
      RD_VEC2I, 
      RD_VEC3I, 
      RD_VEC4I, 

      RD_UINT,
      RD_VEC2UI,
      RD_VEC3UI,
      RD_VEC4UI,

   };

   //
   struct Resource_def
   {
      std::string    label;
      ResourceData   type;
   };
   
   //
   struct Field_struc
   {
      ResourceData   type;
      size_t         count;
      void*          mem;

      typedef std::map<std::string, Field_struc> Map;
   };

   // Resource_obj - simple struct to 
   // store untype pointers of fields

   struct Resource_obj 
   {
      enum { LabelLength = 32 };
      std::string                label;
      std::vector<unsigned char> buffer;
      Field_struc::Map           fieldMap;
   };
   
   //
   bool Read_resource (
      Resource_obj& res    , 
      const std::string fname
      );

   };

#endif
