#ifndef EC_MATH_H
#define EC_MATH_H



#include "Common.h"
#include "FE_Math.h"


namespace FFM
{
 
  //  
  struct EC_Point : public std::tuple<FE_t, FE_t, FEConPtr>  {

    const FE_t& x() const { return std::get<0> (*this) ;}
    const FE_t& y() const { return std::get<1> (*this); }

    FE_t& x() { return std::get<0> (*this); }
    FE_t& y() { return std::get<1> (*this); }

  };

  
  struct EC_Coeffs : public std::tuple<FE_t, FE_t, FEConPtr> {
    
    const FE_t& a() const { return std::get<0> (*this) ;}
    const FE_t& b() const { return std::get<1> (*this); }

    FE_t& a() { return std::get<0> (*this) ;}
    FE_t& b() { return std::get<1> (*this); }

  }; 

    
  // s * P
  EC_Point& EC_Mul (EC_Point& out, FE_t s, const EC_Point& P); 
  //  
  EC_Point& EC_Add (EC_Point& out, const EC_Point& lhs, const EC_Point& rhs);
  EC_Point& EC_Sub (EC_Point& out, const EC_Point& lhs, const EC_Point& rhs);
  // initialize point with elements
  EC_Point  EC_MakePoint (FEConPtr);
  EC_Coeffs EC_MakeCoeffs(FEConPtr); 
  // return to source
  void Free (EC_Point& x);
  void Free (EC_Coeffs& x); 

  void EC_Print (const char* lbl, const EC_Point& x);
  void EC_Print (const char* lbl, const EC_Coeffs& x); 
  
  EC_Point& EC_Set (EC_Point& out, const char* strv_x, const char* strv_y, size_t base ); 
  EC_Coeffs& EC_Set (EC_Coeffs& out, const char* strv_a, const char* strv_b, size_t base ); 

  bool is_point_on_curve (EC_Point& p, EC_Coeffs& eq); 

}

#endif
