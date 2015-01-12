#ifndef introducing_Shapes
#define introducing_Shapes

#include "VecMath.h"

namespace Sh
{
   template<typename ElTy>
   struct Wedge { 

      Ma::Vector<3,ElTy> point; 
      Ma::Vector<3,ElTy> normal;
      Ma::Vector<3,ElTy> radius;
      Ma::Vector<2,ElTy> startstop_angls;

   }; 

   typedef Wedge<float> Wedgef; 
   typedef Wedge<double> Wedged; 
}


#endif
