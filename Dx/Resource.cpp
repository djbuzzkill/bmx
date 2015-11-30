#include "Resource.h"
#include <memory>
#include "Utility.h"


using namespace Dx; 

size_t count_data_type (ResourceData type)
{
   switch (type)
   {
   case RD_UCHAR  : 
   case RD_CHAR   : 
   case RD_SHORT  : 
   case RD_USHORT : 
   case RD_FLOAT  : 
   case RD_DOUBLE : 
   case RD_INT    : 
   case RD_UINT   : 
      return 1; 
   case RD_VEC2UC :
   case RD_VEC2C  :
   case RD_VEC2S  :
   case RD_VEC2US :
   case RD_VEC2F  :
   case RD_VEC2D  :
   case RD_VEC2I  :
   case RD_VEC2UI :
      return 2; 
   case RD_VEC3UC : 
   case RD_VEC3C  : 
   case RD_VEC3S  : 
   case RD_VEC3US : 
   case RD_VEC3F  : 
   case RD_VEC3D  : 
   case RD_VEC3I  : 
   case RD_VEC3UI : 
      return 3; 
   case RD_VEC4UC : 
   case RD_VEC4C  : 
   case RD_VEC4S  : 
   case RD_VEC4US : 
   case RD_VEC4F  : 
   case RD_VEC4D  : 
   case RD_VEC4I  : 
   case RD_VEC4UI : 
      return 4; 
   }
   return 0; 
}




bool Dx::Read_resource(
   Resource_obj&        res,
   const Resource_def*  def,
   size_t               count,
   const std::string    fname)
{
   DX_ASSERT(def, "invalid definition supplied");
   DX_ASSERT(count, "invalid def count supplied");

   res.fieldMap.clear();
   if (size_t sizeOf_file = ut::SizeOf_file(fname))
   {
      std::shared_ptr<FILE> rs(fopen(fname.c_str(), "rb"), fclose);
      res.buffer.resize(sizeOf_file);
      fread(res.buffer.data(), sizeof(unsigned char), sizeOf_file, rs.get());

      res.label = reinterpret_cast<char*> (res.buffer.data());
      unsigned char* curpos = reinterpret_cast<unsigned char*> (res.buffer.data() + Dx::Resource_obj::MaxLabelLength);
      
      for (int i = 0; i < count; i++)
      {
         size_t sizeOf_type = count_data_type(def[i].type);

         res.fieldMap[def[i].label].count = *(unsigned int*) curpos;
         curpos += sizeof(unsigned int); 

         switch (def[i].type)
         {
            case RD_UCHAR : 
            case RD_VEC2UC: 
            case RD_VEC3UC: 
            case RD_VEC4UC: 
               res.fieldMap[def[i].label].mem = curpos;
               curpos += res.fieldMap[def[i].label].count * sizeOf_type * sizeof(unsigned char);
               break;
            case RD_CHAR : 
            case RD_VEC2C: 
            case RD_VEC3C: 
            case RD_VEC4C: 
               res.fieldMap[def[i].label].mem = curpos;
               curpos += res.fieldMap[def[i].label].count * sizeOf_type * sizeof(char);
               break;

            case RD_SHORT: 
            case RD_VEC2S: 
            case RD_VEC3S: 
            case RD_VEC4S: 
               res.fieldMap[def[i].label].mem = curpos;
               curpos += res.fieldMap[def[i].label].count * sizeOf_type * sizeof(short);
               break;

            case RD_USHORT: 
            case RD_VEC2US: 
            case RD_VEC3US: 
            case RD_VEC4US: 
               res.fieldMap[def[i].label].mem = curpos;
               curpos += res.fieldMap[def[i].label].count * sizeOf_type * sizeof(unsigned short);
               break;

            case RD_FLOAT: 
            case RD_VEC2F: 
            case RD_VEC3F: 
            case RD_VEC4F: 
               res.fieldMap[def[i].label].mem = curpos;
               curpos += res.fieldMap[def[i].label].count * sizeOf_type * sizeof(float);
               break;

            case RD_DOUBLE: 
            case RD_VEC2D : 
            case RD_VEC3D : 
            case RD_VEC4D : 
               res.fieldMap[def[i].label].mem = curpos;
               curpos += res.fieldMap[def[i].label].count * sizeOf_type * sizeof(double);
               break;

            case RD_INT  : 
            case RD_VEC2I: 
            case RD_VEC3I: 
            case RD_VEC4I: 
               res.fieldMap[def[i].label].mem = curpos;
               curpos += res.fieldMap[def[i].label].count * sizeOf_type * sizeof(int);
               break;

            case RD_UINT  : 
            case RD_VEC2UI: 
            case RD_VEC3UI: 
            case RD_VEC4UI: 
               res.fieldMap[def[i].label].mem = curpos;
               curpos += res.fieldMap[def[i].label].count * sizeOf_type * sizeof(unsigned int);
               break;

         }
      }
   }
   return false;




}
