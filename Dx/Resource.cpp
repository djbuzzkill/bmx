#include "Resource.h"
#include <memory>
#include "Utility.h"


using namespace Dx; 

size_t data_type_comps (ResourceData type)
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
   const std::string    fname)
{
//   DX_ASSERT(def, "invalid definition supplied");
//   DX_ASSERT(count, "invalid def count supplied");
   char buf[Dx::Resource_obj::LabelLength + 1];
   std::fill(buf, buf + Dx::Resource_obj::LabelLength + 1, 0); 

   res.fieldMap.clear();
   if (size_t sizeOf_file = ut::SizeOf_file(fname))
   {
      std::shared_ptr<FILE> rs(fopen(fname.c_str(), "rb"), fclose);
      res.buffer.resize(sizeOf_file);
      fread(res.buffer.data(), sizeof(unsigned char), sizeOf_file, rs.get());

      char* curpos = reinterpret_cast<char*> (res.buffer.data());

      // obj name
      std::copy(curpos, curpos + Dx::Resource_obj::LabelLength, buf);
      res.label = buf;
      curpos += Dx::Resource_obj::LabelLength; 
      
      // num fields
      size_t count = *reinterpret_cast<unsigned int*> (curpos);
      curpos += sizeof(unsigned int);

      for (int i = 0; i < count; i++)
      {
         std::copy(curpos, curpos + Dx::Resource_obj::LabelLength, buf);
         const std::string name = buf; 
         curpos += Dx::Resource_obj::LabelLength;

         res.fieldMap[name].type = *(Dx::ResourceData*)curpos;
         curpos += sizeof(unsigned int);

         res.fieldMap[name].count = *(unsigned int*)curpos;
         curpos += sizeof(unsigned int); 

         size_t numcompoents = data_type_comps(res.fieldMap[name].type);

         switch (res.fieldMap[name].type)
         {
            case RD_UCHAR : 
            case RD_VEC2UC: 
            case RD_VEC3UC: 
            case RD_VEC4UC: 
               res.fieldMap[name].mem = curpos;
               curpos += res.fieldMap[name].count * numcompoents * sizeof(unsigned char);
               break;
            case RD_CHAR : 
            case RD_VEC2C: 
            case RD_VEC3C: 
            case RD_VEC4C: 
               res.fieldMap[name].mem = curpos;
               curpos += res.fieldMap[name].count * numcompoents * sizeof(char);
               break;

            case RD_SHORT: 
            case RD_VEC2S: 
            case RD_VEC3S: 
            case RD_VEC4S: 
               res.fieldMap[name].mem = curpos;
               curpos += res.fieldMap[name].count * numcompoents * sizeof(short);
               break;

            case RD_USHORT: 
            case RD_VEC2US: 
            case RD_VEC3US: 
            case RD_VEC4US: 
               res.fieldMap[name].mem = curpos;
               curpos += res.fieldMap[name].count * numcompoents * sizeof(unsigned short);
               break;

            case RD_FLOAT: 
            case RD_VEC2F: 
            case RD_VEC3F: 
            case RD_VEC4F: 
               res.fieldMap[name].mem = curpos;
               curpos += res.fieldMap[name].count * numcompoents * sizeof(float);
               break;

            case RD_DOUBLE: 
            case RD_VEC2D : 
            case RD_VEC3D : 
            case RD_VEC4D : 
               res.fieldMap[name].mem = curpos;
               curpos += res.fieldMap[name].count * numcompoents * sizeof(double);
               break;

            case RD_INT  : 
            case RD_VEC2I: 
            case RD_VEC3I: 
            case RD_VEC4I: 
               res.fieldMap[name].mem = curpos;
               curpos += res.fieldMap[name].count * numcompoents * sizeof(int);
               break;

            case RD_UINT  : 
            case RD_VEC2UI: 
            case RD_VEC3UI: 
            case RD_VEC4UI: 
               res.fieldMap[name].mem = curpos;
               curpos += res.fieldMap[name].count * numcompoents * sizeof(unsigned int);
               break;

         }
      }
   }
   return false;




}
