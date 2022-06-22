#ifndef EC_MATH_H
#define EC_MATH_H


#include "Common.h"
#include "FE_Math.h"



typedef std::tuple<FE_t, FE_t> FE_curve; 
bool is_point_on_curve (FE_Point &p, FE_curve &eq, FEContextPtr); 



#endif
