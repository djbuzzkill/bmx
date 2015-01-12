

#ifndef hello_Bounding
#define hello_Bounding


#include "VecMath.h"

namespace Bn
{
   template<typename ElTy>
   struct AABBox 
      {
   Ma::Vector<3, ElTy> min_ext, max_ext;
      };

   typedef AABBox<float>   AABBf; 
   typedef AABBox<double>  AABBd; 
   typedef AABBox<int>     AABBi; 
}

#endif
