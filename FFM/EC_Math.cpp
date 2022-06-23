#include "EC_Math.h"


#include <gmp.h>


namespace FFM
{

#define GETCTX(x) std::get<2>((x))

  
  void EC_Print (const char* s, const EC_Point& x)
  {
    if (GETCTX(x))
      {
	FEConPtr fc = GETCTX(x);
	char xs_[256];
	char ys_[256];
	fc->Format (xs_, "%Zx", x.x());
	fc->Format (ys_, "%Zx", x.y());
	
	printf ( "%s[x:0x%s, y:0x%s]\n", s, xs_, ys_ ); 
      }	
      
  }



  void EC_Print (const char* s, const EC_Coeffs& x)
  {
     if (GETCTX(x))
      {
	FEConPtr fc = GETCTX(x);
	char xs_[256];
	char ys_[256];
	fc->Format (xs_, "%Zx", x.a());
	fc->Format (ys_, "%Zx", x.b());
	
	printf ( "%s[a:0x%s, b:0x%s]\n", s, xs_, ys_ ); 

		    
      }
    
  }


  
EC_Point& EC_Mul (EC_Point& out, FE_t s, const EC_Point& P  )
{
  enum {x = 0, y}; 
  
  if ( GETCTX(out) == GETCTX(P) )
    {
      FEConPtr fc = GETCTX(out);

      if ( !fc->IsValid (s) )
	return out; 

      fc->Mul (out.x(), s, P.x());
      fc->Mul (out.y(), s, P.y());
    }
  return out; 
  
}

  EC_Point EC_MakePoint (FEConPtr fc)
  {
    EC_Point out; 
    out.x() = fc->New(); 
    out.y() = fc->New ();
    std::get<2>(out) = fc; 


    if (!fc->IsValid (out.x()))
	printf ("x is bad\n"); 
    if (!fc->IsValid (out.y()))
	printf ("y is bad\n"); 
    
		 

    return out; 
  }

  
  EC_Coeffs EC_MakeCoeffs (FEConPtr fc)
  {
    EC_Coeffs out; 
    out.a() = fc->New(); 
    out.b() = fc->New();
    GETCTX(out) = fc; 
    return out; 
 }

  void EC_Free (EC_Point& x)
  {
    
    if ( !GETCTX(x) )
      return ; 
    
    GETCTX(x)->Del (std::get<0>(x));
    GETCTX(x)->Del (std::get<0>(x)); 
    GETCTX(x).reset (); 

  }
  
  EC_Point& EC_Set (EC_Point& out, const char* strv_x, const char* strv_y, size_t base)
  {

    
    if (GETCTX(out))
      {

	FEConPtr fc = GETCTX(out); 
	fc->Set (out.x(), strv_x, base);
	fc->Set (out.y(), strv_y, base);

      }
    return out; 
  }

  
  EC_Coeffs& EC_Set (EC_Coeffs& out, const char* strv_a, const char* strv_b, size_t base)
  {
    if (FEConPtr fc = std::get<2> (out))
      {
	enum { a = 0, b}; 
	// crash
	fc->Set (out.a(), strv_a, base); 
	fc->Set (out.b(), strv_b, base); 
      }

    
    return out; 

  }


  
  bool is_point_on_curve(EC_Point &p, EC_Coeffs& eq)
  {

    if (std::get<2>(p) != std::get<2>(eq))
      return false;
  
    FEConPtr fc = std::get<2> (p);
  
    return false;
  }


EC_Point& EC_Add (EC_Point& out, const EC_Point& lhs, const EC_Point& rhs)
  {
    enum { x = 0, y }; 

    if (GETCTX(out) == GETCTX(lhs) && GETCTX(out) == GETCTX(rhs))
      {
	FEConPtr fc = std::get<2> (out);

	fc->Add (out.x(), lhs.x(), rhs.x());

	fc->Add (out.y(), lhs.y(), rhs.y());
 
      }
    
   return out; 

  }
  
  EC_Point& EC_Sub (EC_Point& out, const EC_Point& lhs, const EC_Point& rhs)
  {
    enum {x = 0, y}; 

    
    if (GETCTX(out) == GETCTX(lhs) && GETCTX(out) == GETCTX(rhs))
      {
	FEConPtr fc = std::get<2> (out);
	fc->Sub (out.x(), lhs.x(), rhs.x());
	fc->Sub (out.x(), lhs.y(), rhs.y());
      }

    return out; 
  }

}
