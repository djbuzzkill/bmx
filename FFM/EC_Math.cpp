#include "EC_Math.h"
#include "FFM/FE_Math.h"


#include <gmp.h>

#include <tuple>

namespace FFM
{

  //#define GETCTX(x) std::get<2>((x))

  
  // EC_context::~EC_context () {} 


  typedef std::tuple<FE_t, FE_t> ElemTuple; 
  
  inline FE_t& x(ElemTuple& t) { return std::get<0> (t); }
  inline FE_t& y(ElemTuple& t) { return std::get<1> (t); }
  
  inline const FE_t& x(const ElemTuple& t) { return std::get<0> (t); }
  inline const FE_t& y(const ElemTuple& t) { return std::get<1> (t); }



  
  inline FE_t& a(ElemTuple& t) { return std::get<0> (t); }
  inline FE_t& b(ElemTuple& t) { return std::get<1> (t); }

  inline const FE_t& a(const ElemTuple& t) { return std::get<0> (t); }
  inline const FE_t& b(const ElemTuple& t) { return std::get<1> (t); }


  class EC_con_impl : public EC_context //  , public std::enable_shared_from_this<EC_con_impl> 
  {
  public:
    
    EC_con_impl (FEConPtr fc, const char* astr, const char* bstr, size_t base);
    ~EC_con_impl ();  
   // s * P , 
    bool Add (const std::string& out, const std::string& lhs, const std::string& rhs);
    bool Mul_scalar (const std::string& O, const std::string& s,  const std::string& P); 
    bool Mul_scalar_ui (const std::string& O, size_t s, const std::string& P);
 
    // bool SetCoeffs (const char* a, const char* b, size_t base);
    // bool SetCoeffs_ui (size_t, size_t);


    // initialize point with elements
    bool DefPoint (const std::string& sym, const char* a, const char* , size_t) ;
    bool DefPoint_ui (const std::string& sym, size_t, size_t);  

    bool DefElem (const std::string& sym, const char*, size_t ) ;
    bool DefElem_ui (const std::string& sym, size_t);

    // return to source
    bool UndefPoint (const std::string&) ;
    bool UndefElem  (const std::string&) ;
    
    void PrintPoint (const std::string& , const std::string&, EC_Format f = EC_Format::DEC) ;
    void PrintCoeffs (const std::string&, EC_Format f= EC_Format::DEC); 
    void PrintElem (const std::string& , const std::string&, EC_Format f= EC_Format::DEC); 
    
    
    //  bool EC_IsAtInfinity (const EC_Point& P);
    bool IsPointOnCurve (FE_t, FE_t);

    // -------------------------------------------------------------------
    //
    inline bool pt_exists (const std::string& s) { return pointmap.count(s) ? true : false; }
    //inline bool co_exists (const std::string& s) { return curvemap.count(s) ? true : false; }
    inline bool el_exists (const std::string& s) { return elmap.count(s) ? true : false; }

    ElemTuple& point (const std::string& name) { return pointmap[name]; }
    const ElemTuple& cpoint (const std::string& name) { return pointmap.at(name); }


    FE_t& elem (const std::string& name) { return elmap[name]; }
    const FE_t& celem  (const std::string& name) { return elmap.at(name); }
 
    
    typedef std::map<std::string, ElemTuple> PointMap;
    typedef std::map<std::string, ElemTuple> CurveMap;
    typedef std::map<std::string, FE_t> ElementMap; 
    
    FEConPtr F;  
    PointMap pointmap;

    ElementMap elmap;
    ElemTuple coeffs;
    

    enum { 


      kTempStackSize = 32  } ; 


  };

  
      // EC_con_impl (FEConPtr F, const char*, const char*, size_t); 
    //virtual ~EC_con_impl (); 
  EC_con_impl::EC_con_impl (FEConPtr fc, const char* astr, const char* bstr, size_t base)
      : F(fc)
      , pointmap()
      , coeffs ()
	//, curvemap()dd
      , elmap()
    {
      a(coeffs) = F->New(astr, base);
      b(coeffs) = F->New(bstr, base);
      
    }
    
  //
  EC_con_impl:: ~EC_con_impl ()
    {
      
     
      for (auto& p : pointmap)
	{
	  F->Del (x(p.second));
	  F->Del (y(p.second));
	}

      for (auto e : elmap)
	{
	  F->Del(e.second); 
	}
      
	F->Del (a(coeffs)); 
      F->Del (b(coeffs));

    }
 


  //
  //
  bool EC_con_impl::Add (const std::string& out, const std::string& lhs, const std::string& rhs)
  {
    printf("%s\n", __FUNCTION__); 
	 
    ScopeDeleter dr (F);

    FE_t s = dr(F->New());
    FE_t s_n = dr(F->New());
    FE_t s_d = dr(F->New());
    FE_t ss = dr(F->New());
    FE_t t = dr(F->New());
    FE_t u = dr(F->New());
    FE_t v = dr(F->New());

   ElemTuple M;
   x(M) = F->New();
   y(M) = F->New(); 

   if (!pt_exists(lhs) || !pt_exists(rhs) || pt_exists(out))
     return false;

   
    // Case 0.0: self is the point at infinity, return other
    //  if self.x is None:
    //      return other

    // Case 0.1: other is the point at infinity, return self
    //  if other.x is None:
    //      return self

    const ElemTuple& R = point(rhs); 
    const ElemTuple& L = point(lhs);
    // O != 0

    // comparisons 
    int cmpx = F->Cmp (x(point(lhs)), x(point(rhs))); 
    int cmpy = F->Cmp (y(point(rhs)), y(point(rhs)));


    int cmp_y1_0 =  F->Cmp_ui (y(L), 0);
    
    // 
   printf("line(%i)\n", __LINE__);
    // Case 1: self.x == other.x, self.y != other.y
    // Result is point at infinity
    // //
    if (cmpx == 0 && cmpy != 0)
      {
	POUT("(cmpx == 0 && cmpy != 0)"); 
      }
    

    
    //      
    //Case 2: self.x ≠ other.x
    // Formula (x3,y3)==(x1,y1)+(x2,y2)
    else if (cmpx != 0)
      {
	POUT("cmpx != 0");
	
	// if self.x != other.x:
	//   s = (other.y - self.y) / (other.x - self.x)
	//   x = s**2 - self.x - other.x
	//   y = s * (self.x - x) - self.y
	//   return self.__class__(x, y, self.a, self.b)
	
	
	F->Sub (s_n, y(R), y(L)); 
	F->Sub (s_d, x(R), x(L));
	F->Div (s, s_n, s_d); 
	// s=(y2-y1)/(x2-x1)

	F->Pow_ui(ss, s, 2);
	F->Sub (u, ss, x(L));
	F->Sub (x(M), u, x(R));
	// x3=s**2-x1-x2


	F->Sub (u, x(L), x(M));
	F->Mul (v, s, u);
	F->Sub (y(M), v, y(L)); 
	// y3=s*(x1-x3)-y1


	// xtra check
        assert (IsPointOnCurve(x(M), y(M)));
	point(out) = M; 

	// O = (x3,y3) .. 
	return true; 
     }
   

    else if (cmpx == 0 && cmp_y1_0 == 0)
      {
	POUT("cmpx == 0 && cmp_y1_0 == 0..point is at infinity");

        // Case 4: if we are tangent to the vertical line,

	// just fall thru

      }


    
    else 
      {
	POUT("lhs == rhs"); 
	// Case 3: self == other
        // Formula (x3,y3)=(x1,y1)+(x1,y1)
        // s=(3*x1**2+a)/(2*y1)
        // x3=s**2-2*x1
        // y3=s*(x1-x3)-y1
        // if self == other:
        //    s = (3 * self.x**2 + self.a) / (2 * self.y)
        //    x = s**2 - 2 * self.x
        //    y = s * (self.x - x) - self.y
        //    return self.__class__(x, y, self.a, self.b)
	F->Set_ui(v, 3);
	
	F->Pow_ui(u, x(L), 2);
        F->Mul(t, u, v);
	F->Add(s_n, t, a(coeffs));

	F->Set_ui(v, 2);
	F->Mul (s_d, v, y(L));  
	
	F->Div (s, s_n, s_d);
	// s = (3 * self.x**2 + self.a) / (2 * self.y)

	F->Pow_ui(ss, s, 2);
	F->Add(u, x(L),  x(L)); 
	F->Sub(x(M), ss, u);
        // x = s**2 - 2 * self.x


        F->Sub(v, x(L), x(M));
	F->Mul(t, s, v);
	F->Sub(y(M), t, y(L)); 


	assert (IsPointOnCurve (x(M), y(M)));
	point(out) = M;
	return true; 

      }


    
    F->Del(x(M));
    F->Del(y(M));
 

    return false; 
  }

 //
  //
  bool EC_con_impl:: Mul_scalar (const std::string& O, const std::string& s,  const std::string& P)
  {
    if (!el_exists(s) || !pt_exists(P) || !pt_exists(O))
      return false;


    return true; 
    
    
  }

  //
  // 
  bool EC_con_impl:: Mul_scalar_ui (const std::string& O, size_t s, const std::string& P)
  {
    
    return false; 
  }


  
  //
  //
  bool EC_con_impl::IsPointOnCurve (FE_t ptx, FE_t pty)
  {

    ScopeDeleter dr (F); 
    
    FE_t lhs = dr(F->New ());
    FE_t rhs = dr(F->New ());
    FE_t t0  = dr(F->New ());  
    FE_t t1  = dr(F->New ());
    // y^2 = x^3 + ax + b
    
    F->Pow_ui (t0, ptx, 3); 
    F->Mul (t1, a(coeffs), ptx);
    F->Add (rhs, t0, t1); 
    F->Add (rhs, rhs, b(coeffs));
    
    F->Mul (lhs, pty, pty);

    // lhs > rhs :
    // +
    // lhs = rhs : 0
    // lhs < rhs : - 
    int res = F->Cmp (lhs, rhs);

   return res == 0;
  }

  ECConRef Create_EC_context (FEConPtr F, const char* astr, const char* bstr, size_t base) {

    ECConRef ret (std::make_shared<EC_con_impl> (F, astr, bstr, base));

   return ret; 
  }



  bool EC_con_impl::DefPoint (const std::string& sym, const char* strx, const char* stry, size_t base )
  {


    if (pt_exists(sym))
      {
	// must undef first
	return false; 
      }

    
    FE_t ptx = F->New(strx, base);
    FE_t pty = F->New(stry, base);
    

    if (IsPointOnCurve(ptx, pty))
      {    
	ElemTuple& e = point(sym);
	x(e) = ptx;
	y(e) = pty;
	return true; 	
      }
    else {
      F->Del(ptx);
      F->Del(pty); 

    }
    return false; 

  }
 
  bool EC_con_impl::DefPoint_ui (const std::string& sym, size_t px, size_t py)
  {

    if (pt_exists(sym))
      {
	// already defined, must undef
	return false;

      }
      
    FE_t ptx = F->New_ui (px);
    FE_t pty = F->New_ui (py);
    
    if (IsPointOnCurve(ptx, pty))
      {
	ElemTuple& e = point(sym);
	x(e) = ptx;
	y(e) = pty;

	
	return true;
      }
    else
      {
	F->Del (ptx);
	F->Del(pty); 

      }

    
    return false;
  }
 
  bool EC_con_impl::DefElem (const std::string& sym, const char* strv, size_t base)
  {
    if (!pt_exists(sym))
      {
	elem(sym) = F->New(strv, base);  	
      }
    
    else {

      // already defined
      F->Set (elem(sym), strv, base);  

    }
    return true; 
  }
  
  // return to source
  bool EC_con_impl::DefElem_ui (const std::string& sym, size_t v)
  {
    if (!el_exists(sym))
      {
	elem(sym) = F->New(); 

      }


    F->Set_ui(elem(sym), v);
    
    return true; 
  }



  bool EC_con_impl::UndefPoint (const std::string& sym)
  {
    if (pt_exists (sym)) 
      {
	PointMap::iterator it = pointmap.find (sym);
	if (it != pointmap.end ())
	  {
	    F->Del (x(it->second));
	    F->Del (y(it->second));
	    pointmap . erase (it); 
	    return true; 
	  }
      }
    
    return false; 
  }
  
    
  // bool EC_con_impl::UndefCoeffs (const std::string& sym) {


  //   if (co_exists (sym))
  //     {
  // 	CurveMap::iterator it = curvemap.find (sym); 
  // 	if (it != curvemap.end ())
  // 	  {
  // 	    F->Del (a(it->second));
  // 	    F->Del (b(it->second));
  // 	    curvemap.erase (it);
  // 	    return true; 

  // 	  }

  //     }
  //   return false; 
  // }

  bool EC_con_impl::UndefElem (const std::string& sym)
  {
    if (el_exists(sym))
      {
	ElementMap::iterator it = elmap.find (sym);
	  if (it != elmap.end ())
	    {
	      F->Del (it->second);
	      elmap.erase (it);
	      
	      return true; 

	    }
      }
    return false;
      
  }
  
  
  void EC_con_impl::PrintPoint (const std::string& lbl, const std::string& P, EC_Format fmt)
  {
    char xs_[256];
    char ys_[256];


    if (!pt_exists(P))
      {
	printf ("%s: \"%s\" is not defined\n", lbl.c_str(), P.c_str()); 
	return;
      }

      
      switch (fmt)
      {
      case EC_Format::HEX:  
	F->Format (xs_, "%Zx", x(point(P)));
	F->Format (ys_, "%Zx", x(point(P)));
	printf ( "%s[x:0x%s, y:0x%s]\n", lbl.c_str(), xs_, ys_ ); 
	break;
	
      case EC_Format::DEC: 
	
      default: 
	F->Format (xs_, "%Zd", x(point(P)));
	F->Format (ys_, "%Zd", y(point(P))); 
	printf ( "%s[x:%s, y:%s]\n", lbl.c_str(), xs_, ys_ ); 
	break;
      }	
  	
      
  } 
  
  void EC_con_impl::PrintElem (const std::string& lbl , const std::string& e, EC_Format fmt )
  {
    char xs[256];

    switch (fmt)
      {
      case EC_Format::HEX:
	F->Format (xs, "%Zx", elem(e));
	printf ( "%s[0x%s]\n", lbl.c_str(), xs); 
	break;
	
      case EC_Format::DEC:
      default: 
	F->Format (xs, "%Zd", elem(e));
	printf ( "%s[%s]\n", lbl.c_str(), xs); 
	break;
      }	
 
  }
  


void EC_con_impl::PrintCoeffs  (const  std::string& lbl , EC_Format fmt)
  {

    char xs_[256];
    char ys_[256];

    switch (fmt)
      {
      case EC_Format::HEX:
	F->Format (xs_, "%Zx", a(coeffs));
	F->Format (ys_, "%Zx", b(coeffs));
	printf ( "%s[a:0x%s, b:0x%s]\n", lbl.c_str(), xs_, ys_ ); 
	break;
	
      case EC_Format::DEC:
      default: 
	F->Format (xs_, "%Zd", a(coeffs));
	F->Format (ys_, "%Zd", b(coeffs));
	printf ( "%s[a:%s, b:%s]\n", lbl.c_str(), xs_, ys_ ); 
	break;
      }	
  } 

  
  // bool EC_con_impl::SetCoeffs ( const char* strv_a, const char* strv_b, size_t base )
  // {
    
  //   ElemTuple& tup = curve (out);
    
  //   F->Set (a(coeffs), strv_a, base);
  //   F->Set (b(coeffs), strv_b, base);
  //   return true; 


  // } 


  // bool EC_con_impl::SetElem (const std::string& el, const char* str_v, size_t base)
  // {
  //   if (!el_exists(el))
  //     return false;

  //   F->Set (elem(el), str_v, base);
  //   return true; 
    
  // }
 

  // bool EC_con_impl::SetPoint_ui (const std::string& sym, size_t x_, size_t y_)
  // {
  //   if (!pt_exists (sym))
  //     return false; 
    
  //   const ElemTuple& out = point(sym); 
  //   F->Set_ui (x(out), x_);
  //   F->Set_ui (y(out), y_);
  //   return true; 

  // }
   
  // bool EC_con_impl::SetCoeffs_ui (const std::string& out, size_t a_, size_t b_ )
  // {
  //   if (!co_exists(out))
  //     return false;

    
  //   const ElemTuple& tup = curve (out);
    
  //   F->Set_ui (a(tup), a_);
  //   F->Set_ui (b(tup), b_);
  //   return true; 


  // } 

  // bool EC_con_impl::SetElem_ui (const std::string& el, size_t v)
  // {
  //   if (!el_exists(el))
  //     return false;

    
  //   F->Set_ui (elem(el), v);
  //   return true; 

    
  // }
  

 
}
