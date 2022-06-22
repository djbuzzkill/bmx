#include "EC_Math.h"

namespace FM
{

FE_Point& FE_Mult (FE_Point& out, FE_t s, const FE_Point& P, FEContextPtr ctx)
{

  ctx->Mul (std::get<0> (out), s, std::get<0>(out));
  ctx->Mul (std::get<1> (out), s, std::get<1>(out));
  
  return out; 
  
}


bool is_point_on_curve(FE_Point &p, FE_Curve &eq, FEContextPtr fc)
{
  (void) p;
  (void) eq;
  (void) fc;
  
  return false;
}

}
