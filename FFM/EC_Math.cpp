#include "EC_Math.h"
#include "FFM/FE_Math.h"

#include <gmp.h>

#include <tuple>

namespace FFM
{

  //#define GETCTX(x) std::get<2>((x290:297:

  
  // EC_context::~EC_context () {} 


  typedef std::tuple<FE_t, FE_t> ElemTuple; 
  
  inline FE_t& x(ElemTuple& t) { return std::get<0> (t); }
  inline FE_t& y(ElemTuple& t) { return std::get<1> (t); } 
  inline const FE_t& x(const ElemTuple& t) { return std::get<0> (t); }
  inline const FE_t& y(const ElemTuple& t) { return std::get<1> (t); }

  template<typename Ty>
  inline bool is_INF (Ty x) { return x < 0 ? true : false; }
  
  inline ElemTuple& set_INF(ElemTuple& P)
  {
    x(P) = x(P) < 0 ? x(P) : -x(P);
    y(P) = y(P) < 0 ? y(P) : -y(P);
    return P;
  }

    
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
    // bool Mul_scalar (const std::string& O, const std::string& s,  const std::string& P); 
    bool Mul_scalar_ui (const std::string& O, size_t s, const std::string& P); 
    // bool SetCoeffs (const char* a, const char* b, size_t base);
    // bool SetCoeffs_ui (size_t, size_t);


    // initialize point with elements
    bool DefPoint (const std::string& sym, const char* a, const char* , size_t) ;
    bool CopyPoint (const std::string& sym, const std::string& P); 
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
    




  };

  
      // EC_con_impl (FEConPtr F, const char*, const char*, size_t); 
    //vir290:297:al ~EC_con_impl (); 
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
  bool EC_con_impl::Add (const std::string& out, const std::string& lhs, const std::string& rhs)
  {
    //printf ("%s[out:%s, lhs:%s, rhs:%s] \n", __FUNCTION__, out.c_str(), lhs.c_str(), rhs.c_str());  
    ScopeDeleter dr (F);

    FE_t s = dr(F->New());
    FE_t s_n = dr(F->New());
    FE_t s_d = dr(F->New());
    FE_t ss  = dr(F->New());
    FE_t t   = dr(F->New());
    FE_t u   = dr(F->New());
    FE_t v   = dr(F->New());
    
    
    FE_t xo  = F->New();
    FE_t yo  = F->New();
    
    if (!pt_exists(lhs) )
      {
	POUT ("lhs DNE");
	return false;
      }
    
    if ( !pt_exists(rhs) )
     {
       POUT ("rhs DNE");
       return false;
     }
    
    
    // Case 0.0: self is the point at infinity, return other
    if (is_INF(x(point(lhs))))
      {
	POUT("[case0.0: lhs.x INF]");
	set_INF(point(out));
	return false;
      }
    //      
   
   // Case 0.1: other is the point at infinity, return self
   //  if other.x is None:
   //      return 
    if (is_INF(x(point(rhs))))
     {
       POUT("[case0.1: rhs.x INF]");
       set_INF(point(out));
       return false;
     }
   
    if (is_INF (y(point(lhs))))
      {
	POUT ("y.lhs is INF");
	return false;
      }


    if (is_INF (y(point(rhs))))
      {
	POUT ("y.rhs is INF");
	return false;
      }
     
    const ElemTuple& R = point(rhs); 
    const ElemTuple& L = point(lhs);
    // O != 0
    char tbuf[32]; 
	
    // comparisons 
    int cmpx = F->Cmp (x(point(lhs)), x(point(rhs))); 
    int cmpy = F->Cmp (y(point(lhs)), y(point(rhs)));

    // Case 1: self.x == other.x, self.y != other.y
    // Result is point at infinity
    // //
    if (cmpx == 0 && cmpy != 0)
      {
	// POUT("[case1: x1==x2 && y1!=y2");
	POUT("[case1: INF]"); 
	
	set_INF (point(out));

	return false;
      }
    //      
    //Case 2: self.x != other.x
    // Formula (x3,y3)==(x1,y1)+(x2,y2)
    else if (cmpx != 0)
      {
	POUT("[case2: x1!=x2]"); 
        // POUT("  x1 != x2");
	
	// if self.x != other.x:
	//   s = (other.y - self.y) / (other.x - self.x)
	//   x = s**2 - self.x - other.x
	//   y = s * (self.x - x) - self.y
	//   return self.__class__(x, y, self.a, self290:297:)
		//   y = s * (self.x - x) - self.y
	//   return self.__class__(x, y, self.a, self290:297:)
	//   return self.__class__(x, y, self.a, self290:297:)
	
	
	F->Sub (s_n, y(R), y(L)); 
	F->Sub (s_d, x(R), x(L));
	F->Div (s, s_n, s_d); 
	// s=(y2-y1)/(x2-x1)


	F->Pow_ui(ss, s, 2);
	F->Sub (u, ss, x(L));
	F->Sub (xo, u, x(R));
	// x3=s**2-x1-x2
        
		
	F->Sub (u, x(L), xo);
	F->Mul (v, s, u);
	F->Sub (yo, v, y(L));
	
	// y3=s*(x1-x3)-y1


	// xtra check
        if (IsPointOnCurve(xo, yo))
	  {
	    if (pt_exists(out))
	      {
		ElemTuple& O = point(out);
		
		F->Set (x(O), xo);
		F->Set (y(O), yo);
		dr (xo);
		dr (yo);
		
	      }
	    else
	      {
		ElemTuple& O = point(out);
		x(O) = xo;
		y(O) = yo; 
		
	      }
	    
	    return true; 
	  }
	else
	  {
	    printf ("POINT NOT ON CURVE|ln:%i\n", __LINE__); 
	  }
	POUT("case4 breakout");
     }
    else if (cmpx == 0 && cmpy != 0)
      {
        // Case 4: if we are tangent to the vertical lin:
	//POUT("[case4:INF]"); 
	set_INF (point(out));
	return false;
	
      }
    else if (cmpx == 0 && cmpy == 0) 
      {
	POUT("[case3:lhs==rhs]"); 
	// Case 3: self == other
        // Formula (x3,y3)=(x1,y1)+(x1,y1)
        // s=(3*x1**2+a)/(2*y1)
        // x3=s**2-2*x1
        // y3=s*(x1-x3)-y1
        // if self =290:297:other:
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

        F->Sub(xo, ss, u);
        // x = s**2 - 2 * self.x

        F->Sub(v, x(L), xo);
	F->Mul(t, s, v);
	F->Sub(yo, t, y(L)); 
        // y3=s*(x1-x3)-y1

	if (IsPointOnCurve (xo, yo))
	  {
	  if (pt_exists(out))
	    {
	      ElemTuple& M = point(out);
	      F->Set (x(M), xo);
	      F->Set (y(M), yo); 
	      dr (xo);
	      dr (yo); 
	    }
	  else
	    {

	      ElemTuple& nupoint = point(out);
	      x(nupoint) = xo;
	      y(nupoint) = yo;   
	    }
	  return true; 
	}
	else
	  {
	    printf ("POINT NOT ON CURVE|ln:%i\n", __LINE__);

	  }
      }


    
    // F->Del(x(M));
    // F->Del(y(M));
 
    POUT("return end");  
    return false; 
  }

 //
  //
  // bool EC_con_impl:: Mul_scalar (const std::string& O, const std::string& s,  const std::string& P)
  // {

  //   if (!el_exists(s) || !pt_exists(P) || pt_exists(O))
  //     return false;

  //   FE_t one = F->New_ui (1);

    
  //   CopyPoint(O, P);

  //   while(F->Cmp (elem(s), 0))
  //   {
  //     // s & 0x1
  //     if (F->LogiBit (elem(s), 0))
  // 	{
  // 	  // if s & 0x1 is true, 
  // 	  Add (O, O, P); 
	  
  // 	}
  //     Add (O, O, P); 

  //     // s == s >> 1
  //     F->LogiShiftR (elem(s), 1); 
  //   }
    

  //   return true; 
  // }

  //
  // 
  bool EC_con_impl:: Mul_scalar_ui (const std::string& O, size_t s_, const std::string& P)
  {

    printf ("%s[o:%s, s:%zu, P:%s]\n", __FUNCTION__, O.c_str(), s_, P.c_str());

    if ( !pt_exists(P))
      {
	POUT ("P=DNE");
	return false;
      }
    
    // std::array<char,128> xbuf;
    // std::array<char, 128> ybuf;
  
    //    int mcnt = 0; 
    size_t scount = s_ - 1;
    CopyPoint (O, P);
    //mcnt++;	
    //F->Format (xbuf, "%Zu", x(point(O)));
    //F->Format (ybuf, "%Zu", y(point(O)));
    //printf ("|%i) %s(x:%s, y:%s)\n", mcnt , O.c_str(), xbuf, ybuf); 
    bool add_success = true;
    while (scount && add_success)  
      {
	// if (scount & 0x1)
	// {

	//   POUT ("inwhile_if"); 
	//   Add(O, O, P);
	//   numadds++; 
	// }
	

	add_success = Add (O, O, P); 
	if (add_success == false)
	  {
	    printf ("add FAILED\n");
	  }

	scount--; 


	//mcnt++;	
	//F->Format (xbuf, "%Zu", x(point(O)));
	//F->Format (ybuf, "%Zu", y(point(O)));
	//printf ("|%i) %s(x:%s, y:%s)\n", mcnt,  O.c_str(), xbuf, ybuf); 
      }
    
    
    
    return false; 
  }


  
  //
  //
  bool EC_con_impl::IsPointOnCurve (FE_t ptx, FE_t pty)
  {

    char xbuf[256];
    char ybuf[256];


    // negative element names are define to represent infinity
    if (ptx < 0 || pty < 0)
      {
	// is this correct?
	return false; 
      }


    //F->Format (xbuf, "%Zu", ptx);
    //F->Format (ybuf, "%Zu", pty);
    //printf ("%s[x:%s, y:%s):", __FUNCTION__, xbuf, ybuf); 
 
    ScopeDeleter dr (F); 
    
    FE_t lhs = dr(F->New ());
    FE_t rhs = dr(F->New ());
    FE_t t0  = dr(F->New ());  
    FE_t t1  = dr(F->New ());
    FE_t t2  = dr(F->New ()); 



    // y^2 = x^3 + ax + b
    
    F->Pow_ui (t0, ptx, 3); 
    F->Mul (t1, a(coeffs), ptx);
    F->Add (t2, t0, t1); 
    F->Add (rhs, t2, b(coeffs));
    // rhs
    
    F->Mul (lhs, pty, pty);
    // lhs

    
    //F->Format (xbuf, "%Zu", rhs);
    //F->Format (ybuf, "%Zu", lhs);
    //printf ("[%s=%s]", ybuf, xbuf); 
    // lhs > rhs :
    // +
    // lhs = rhs : 0
    // lhs < rhs : - 
    int res = F->Cmp (lhs, rhs);
    //printf("%s\n", res == 0? "yes" : "no");


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


  bool EC_con_impl::CopyPoint(const std::string &sym, const std::string &P)
  {
    if (!pt_exists(P))
      return false;

    ElemTuple& newpoint = point(sym);   
    x(newpoint) = F->New ();  
    y(newpoint) = F->New ();
    if (is_INF(x(point(P))) || is_INF(y(point(P))))
      {
	x(newpoint) = -x(newpoint);
	y(newpoint) = -x(newpoint); 
      }
    else
      {
	F->Set (x(newpoint), x(point(P)));
	F->Set (y(newpoint), y(point(P)));
      }
    
   
    return true;
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

    

    if (is_INF (x(point(P))))
      {
	printf ("[%s is INF]\n", P.c_str());
	return; 
      }
    if (is_INF (y(point(P))))
      {
	printf ("[%s is INF]\n", P.c_str());
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
