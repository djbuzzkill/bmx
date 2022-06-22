#ifndef EC_MATH_H
#define EC_MATH_H



#include "Common.h"
#include "FE_Math.h"


namespace FFM
{


  typedef std::tuple<FE_t, FE_t> FE_Point; 
  typedef std::tuple<FE_t, FE_t> FE_Curve; 


// s * P
  FE_Point& FE_Mult (FE_Point& out, FE_t s, const FE_Point& P, FEConPtr); 
 



  bool is_point_on_curve (FE_Point& p, FE_Curve& eq, FEConPtr); 

}

#endif
