#include "ec_math.h"
#include "ffm/fe_math.h"

#include <gmp.h>

#include <tuple>

namespace ffm {
  //
  // 
  class EC_con_imp2 : public EC_context {
    // 
  public:
    
    EC_con_imp2 (FEConRef fc, el::map& em, pt::map& pm, const char* astr, const char* bstr, const char*, size_t base);
    ~EC_con_imp2 ();  
    // s * P , 
    bool AddPoint (const std::string& out, const std::string& lhs, const std::string& rhs);
    
     
    bool Mul_scalar_ui (const std::string& O, size_t s, const std::string& P); 
    bool Mul_scalar (const std::string& O, const std::string& s, const std::string& P);   

    bool Mul_scalar (const std::string& O, FE_t s, const std::string& P); 

    bool MakeElem (const std::string& sym, const std::string& sym2);  
    bool MakeElem (const std::string& sym, const char*, size_t ) ;
    bool MakeElem_ui (const std::string& sym, size_t);
    bool SetElem (const std::string& dst, const std::string& src);

    bool MakePoint (const std::string& sym, FE_t, FE_t); 
    bool MakePoint (const std::string& sym, const char* xstr, const char* ystr, size_t base);
    bool MakePoint (const std::string& sym, const std::string& xel, const std::string& yel);
    bool MakePoint_ui (const std::string& sym, size_t xval, size_t yval); 
    bool SetPoint (const std::string& sym, const std::string& xel, const std::string& yel);
    bool CopyPoint (const std::string& lhs, const std::string& rhs);


    // return to source
    bool DelPoint (const std::string&) ;
    bool DelElem  (const std::string&) ;

    void PrintPoint (const std::string& , const std::string&, format f = format::hex) ;
    void PrintCoeffs (const std::string&, format f= format::dec); 
    void PrintElem (const std::string& , const std::string&, format f= format::dec); 
    void PrintElem (const std::string& , FE_t, format f= format::dec); 

    void printElem (const std::string&, FE_t e, format f); 
    void printPoint (const std::string& lbl, const pt::struc& P, format fmt);
    

    //  bool EC_IsAtInfinity (const EC_Point& P);
    bool IsPointOnCurve (FE_t, FE_t); 
    // bool ECDSA_verify (const char* sz_z, const char* sz_r, const char* sz_s);
    bool IsPointOnCurve (const std::string& xco, const std::string& yco);  
    bool IsPointOnCurve (const std::string& P);
 
    // -------------------------------------------------------------------
    //
    inline bool ptexs (const std::string& s) { return pointmap.count(s) ? true : false; }
    //inline bool co_exists (const std::string& s) { return curvemap.count(s) ? true : false; }
    inline bool elexs (const std::string& s) { return elmap.count(s) ? true : false; }

    pt::struc& point (const std::string& name) { return pointmap[name]; }
    const pt::struc& point (const std::string& name) const { return pointmap.at(name); }
    FE_t& elem (const std::string& name) { return elmap[name]; }
    const FE_t& elem  (const std::string& name) const { return elmap.at(name); }
 
    inline bool isINF (FE_t x) { return (x < 0 ? true : false); }
    inline bool isINF (const pt::struc& tup) { return isINF (x(tup)) || isINF(y(tup)); }

    inline pt::struc& setINF(pt::struc& P) {
      x(P) = x(P) < 0 ? x(P) : -x(P);
      y(P) = y(P) < 0 ? y(P) : -y(P);
      return P;
    }
 
    inline pt::struc& unINF (pt::struc& tup) {
      x(tup) = isINF(x(tup)) ? -x(tup) : x(tup);
      y(tup) = isINF(y(tup)) ? -y(tup) : y(tup); 
      return tup;
    }
    
    inline bool makePoint (pt::struc& place, FE_t xo, FE_t yo) {
      if (isINF(xo)) {
	// should we say something
      }
      
      if (isINF(yo)) {
	// should we say something
      }

      unINF(place); 
      x(place) = xo;
      y(place) = yo;
      return true;
    }

    
    //inline bool makePoint (pt::struc& place, const pt::struc& rhs) {
    //  return makePoint (place, x(rhs), y(rhs)); 
    //}

    
    // copy value data
    inline bool setPoint(pt::struc& place, FE_t xo, FE_t yo) {
      unINF(place); 
      F->Set (x(place), xo);
      F->Set (y(place), yo);

      return true; 
    }
 
    inline bool copyPoint(pt::struc& lhs, const pt::struc& rhs) {
      //printf ("copeePee[(%i, %i), (%i, %i)] \n", x(lhs), y(lhs), x(rhs), y(rhs)); 
      
      return setPoint (lhs, x(rhs), y(rhs)); 
    }

    bool addPoint (pt::struc& out, const pt::struc& lhs, const pt::struc& rhs);
   
   //    typedef std::map<std::string, elem_tuple> PointMap;
    //typedef std::map<std::string, elem_tuple> CurveMap;
    //typedef std::map<std::string, FE_t> ElementMap; 
    FEConRef F;
    
    pt::map&  pointmap;

    el::map&  elmap;

    FE_t coeff_a;
    FE_t coeff_b;
  private: 

  };


  // 
  ECConRef Create_EC_context (FEConRef F, el::map&  em, pt::map& pm, const char* astr, const char* bstr, const char* order, size_t base) {

    ECConRef ret (std::make_shared<EC_con_imp2> (F, em, pm, astr, bstr, order, base));

   return ret; 
  }

  // EC_con_impl (FEConRef F, const char*, const char*, size_t); 
  EC_con_imp2::EC_con_imp2 (FEConRef fc, el::map& em, pt::map& pm, const char* astr, const char* bstr, const char* n_sz, size_t base)
      : F(fc)
      , elmap (em)
      , pointmap(pm) {

    elmap["n"] = F->New (n_sz, base);
    elmap["Fp"] = F->p();
    
    elmap["coeff.a"] = coeff_a = F->New(astr, base);
    elmap["coeff.b"] = coeff_b = F->New(bstr, base);
    
  }
    
  //
j  //
  EC_con_imp2:: ~EC_con_imp2 () {
    //printf ("%s\n", __FUNCTION__);  

  }


  //
  //
  bool EC_con_imp2::MakePoint (const std::string& sym, FE_t x, FE_t y) {

    if (ptexs(sym)) {
      return false;  
      printf ("%s[sym:%s] : %s already defined\n", __FUNCTION__, sym.c_str(), sym.c_str()); 
    }

    return makePoint (point(sym), x, y); 
  }
 
  //
  //
  bool EC_con_imp2::MakePoint_ui (const std::string& sym, size_t _x, size_t _y) {
    
    if (ptexs(sym)) {
      return false;  
      printf ("%s[sym:%s] : %s already defined\n", __FUNCTION__, sym.c_str(), sym.c_str()); 
    }
    
    return makePoint (point(sym), F->New_ui(_x), F->New_ui(_y)); 
  }

 
 //
  //
  bool EC_con_imp2::MakePoint (const std::string& sym, const char* xstr, const char* ystr, size_t base)  {

    if (ptexs(sym)) {
      printf ("%s|ln:%i[sym:%s] : %s already defined\n", __FUNCTION__, __LINE__, sym.c_str(), sym.c_str()); 
      // already defined, must undef or soemthing
      return false;
    }
   
    return makePoint (point(sym),F->New (xstr, base),F->New(ystr, base));
  }


  // 
  bool EC_con_imp2::MakePoint (const std::string& sym, const std::string& xel, const std::string& yel) {

    if (pointmap.exs(sym)) { 
      printf ("[%s] %s already defined\n", __FUNCTION__, sym.c_str());
      return false;
    }

    if (!elmap.exs(xel)) {
      printf ("[%s] %s DNE\n", __FUNCTION__, xel.c_str());
      return false;
    }
    if (!elmap.exs(yel)) {
      printf ("[%s] %s DNE\n", __FUNCTION__, yel.c_str());
      return false;
    }

    return setPoint (pointmap[sym], elem(xel), elem(yel));
  }


  // 
  bool EC_con_imp2::SetPoint (const std::string& sym, const std::string& xel, const std::string& yel) {

    if (!pointmap.exs(sym)) { 
      printf ("[%s] %s DNE\n", __FUNCTION__, sym.c_str());
      return false;
    }
    if (!elmap.exs(xel)) {
      printf ("[%s] %s DNE\n", __FUNCTION__, xel.c_str());
      return false;
    }
    if (!elmap.exs(yel)) {
      printf ("[%s] %s DNE\n", __FUNCTION__, yel.c_str());
      return false;
    }

    return setPoint (pointmap[sym], elem(xel), elem(yel));
    
    
  }

  //
  bool EC_con_imp2::CopyPoint (const std::string& lhs, const std::string& rhs) {

    if (!pointmap.exs(lhs)) { 
      printf ("[%s] %s DNE\n", __FUNCTION__, lhs.c_str());
      return false;
    }
 
    if (!pointmap.exs(rhs)) { 
      printf ("[%s] %s DNE\n", __FUNCTION__, rhs.c_str());
      return false;
    }
    
    return copyPoint(point(lhs), point(rhs));  
  }
 
  bool EC_con_imp2::MakeElem (const std::string& sym, const std::string& ssym) {

    if (!elexs(ssym)) {
      printf ("%s does not exist\n", ssym.c_str()); 
      return false;
      
    }
    
    if (!ptexs(sym)) {
      elem(sym) = F->New ();  	
    }
    F->Set (elem(sym), elem(ssym));  
    return true; 
  }
  

  
  bool EC_con_imp2::MakeElem (const std::string& sym, const char* strv, size_t base) {
    if (!elexs(sym)) {
      elem(sym) = F->New(strv, base);  	
    }
    else {
      // already defined
      F->Set (elem(sym), strv, base);  
    }
    return true; 
  }
  
  // return to source
  bool EC_con_imp2::MakeElem_ui (const std::string& sym, size_t v) {
    if (!elexs(sym)) {
	elem(sym) = F->New(); 

      }

    F->Set_ui(elem(sym), v);
    return true; 
  }

  bool EC_con_imp2::SetElem (const std::string& dst, const std::string& src) {
    if (!elexs(src)) {
      return false;
    }

    if (!elexs(dst)) {
      return false;
    }

    F->Set (elem(dst), elem(src)); 
    return true; 
  }
 

  bool EC_con_imp2::DelPoint (const std::string& sym)
  {
    if (ptexs (sym)) 
      {
	pt::map::iterator  it = pointmap.find (sym);
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
  
  bool EC_con_imp2::DelElem (const std::string& sym) {
    if (elexs(sym)) {


      el::map::iterator it = elmap.find (sym);

      if (it != elmap.end ()) {
	F->Del (it->second);
	elmap.erase (it);
	return true; 
      }
    }
    return false;
  }
  
  //
  //
  void EC_con_imp2::printPoint (const std::string& lbl, const pt::struc& P, format fmt) {
    char xs_[256];
    char ys_[256];


    if (isINF (P)) {
      printf ("%s|<INF,INF>\n", lbl.c_str());
      return; 
    }
      
    switch (fmt) {
    case format::hex:  
      F->Format (xs_, "%Zx", x(P));
      F->Format (ys_, "%Zx", y(P));
      printf ( "%s<0x%s, 0x%s>\n", lbl.c_str(), xs_, ys_ ); 
      break;
      
    case format::dec: 
      
    default: 
      F->Format (xs_, "%Zd", x(P));
      F->Format (ys_, "%Zd", y(P)); 
      printf ( "%s<%s, %s>\n", lbl.c_str(), xs_, ys_ ); 
      break;
    }	
    
  }
  

  //
  //
  void EC_con_imp2::PrintPoint (const std::string& lbl, const std::string& P, format fmt)
  {
     if (!ptexs(P)) {
      printf ("%s: \"%s\" is not defined\n", lbl.c_str(), P.c_str()); 
      return;
    }

     printPoint (lbl, point(P), fmt); 
     
  } 

  //
  void EC_con_imp2::printElem (const std::string& lbl, FE_t e, format fmt)
  {
     char xs[256];

    switch (fmt)
      {
      case format::hex:
	F->Format (xs, "%Zx", e);
	printf ( "%s[0x%s]\n", lbl.c_str(), xs); 
	break;
	
      case format::dec:
      default: 
	F->Format (xs, "%Zd", e);
	printf ( "%s[%s]\n", lbl.c_str(), xs); 
	break;
      }	
    
  }

  //
  void EC_con_imp2::PrintElem (const std::string& lbl , FE_t e, format fmt )
  {
    printElem (lbl, e, fmt); 
  }
  
  //
  //
  void EC_con_imp2::PrintElem (const std::string& lbl , const std::string& e, format fmt)
  {
    printElem (lbl, elem(e), fmt); 
  }
  
  //
  //
  void EC_con_imp2::PrintCoeffs  (const  std::string& lbl , format fmt)
  {

    char xs_[256];
    char ys_[256];

    switch (fmt)
      {
      case format::hex:
	F->Format (xs_, "%Zx", coeff_a);
	F->Format (ys_, "%Zx", coeff_b);
	printf ( "%s[a:0x%s, b:0x%s]\n", lbl.c_str(), xs_, ys_ ); 
	break;
	
      case format::dec:
      default: 
	F->Format (xs_, "%Zd", coeff_a);
	F->Format (ys_, "%Zd", coeff_b);
	printf ( "%s[a:%s, b:%s]\n", lbl.c_str(), xs_, ys_ ); 
	break;
      }	
  } 
 
  //
  // 
  bool EC_con_imp2::addPoint (pt::struc& out,const pt::struc& lhs,const pt::struc& rhs) {
    //printf ("%s[lhs:<%i, %i>, rhs:<%i, %i>]\n", __FUNCTION__, x(lhs), y(lhs), x(rhs), y(rhs)); 
    ScopeDeleter dr (F);
    FE_t s   = dr(F->New());
    FE_t s_n = dr(F->New());
    FE_t s_d = dr(F->New());
    FE_t ss  = dr(F->New());
    FE_t t   = dr(F->New());
    FE_t u   = dr(F->New());
    FE_t v   = dr(F->New());
    
    // not sure yet 
    FE_t xo  = dr (F->New());
    FE_t yo  = dr (F->New());
    
    //
    //
    if (isINF(pt::x(lhs))) {
      // Case 0.0: self is the point at infinity, return other
      POUT("[case0.0: lhs.x INF]");
      setINF(out);
      return false;
    }
    
    if (isINF(pt::x(rhs))) {
      POUT("[case0.1: rhs.x INF]");
      // Case 0.1: other is the point at infinity, return self
      setINF(out);
      return false;
    }
    
    if (isINF (pt::y(lhs))) {
      POUT ("y.lhs is INF");
      return false;
    }
   
    
    if (isINF (pt::y(rhs))) {
      POUT ("y.rhs is INF");
      return false;
    }

    // O != 0
    char tbuf[32]; 
    
    // comparisons 
    int cmpx = F->Cmp (x(lhs), x(rhs)); 
    int cmpy = F->Cmp (y(lhs), y(rhs));
    
    // Case 1: self.x == other.x, self.y != other.y
    // Result is point at infinity
    // //
    if (cmpx == 0 && cmpy != 0) {
      // POUT("[case1: x1==x2 && y1!=y2");
      //POUT("[case1: INF]"); 
      
      setINF (out);
      
      return false;
    }
    else if (cmpx != 0) {
      //      
      //Case 2: self.x != other.x
      // POUT("[case2: x1!=x2]"); 
      
      F->Sub (s_n, y(rhs), y(lhs)); 
      F->Sub (s_d, x(rhs), x(lhs));
      F->Div (s, s_n, s_d); 
      // s=(y2-y1)/(x2-x1)
      
      F->Pow_ui(ss, s, 2);
      F->Sub (u, ss, x(lhs));
      F->Sub (xo, u, x(rhs));
      // x3=s**2-x1-x2
      
      F->Sub (u, x(lhs), xo);
      F->Mul (v, s, u);
      F->Sub (yo, v, y(lhs));
      // y3=s*(x1-x3)-y1
      
      // xtra check
      if (IsPointOnCurve(xo, yo)) {
	setPoint (out, xo, yo); 
	return true;
      }
      POUT("[case2 breakout]\n"); 
    }
    else if (cmpx == 0 && cmpy != 0) {
      // Case 4: if we are tangent to the vertical lin:
      setINF (out);
      return false;
      
    }
    else if (cmpx == 0 && cmpy == 0) {
      // POUT("[case3:lhs==rhs]"); 
      // Formula (x3,y3)=(x1,y1)+(x1,y1)
      // s=(3*x1**2+a)/(2*y1)
      // x3=s**2-2*x1
      // y3=s*(x1-x3)-y1

      F->Set_ui(v, 3);
      
      F->Pow_ui(u, x(lhs), 2);
      F->Mul(t, u, v);

      F->Add(s_n, t, coeff_a);
      
      F->Set_ui(v, 2);
      F->Mul (s_d, v, y(lhs));  
      
      F->Div (s, s_n, s_d);
      // s = (3 * self.x**2 + self.a) / (2 * self.y)

     
      F->Pow_ui(ss, s, 2);
      F->Add(u, x(lhs),  x(lhs)); 
      
      F->Sub(xo, ss, u);
      // x = s**2 - 2 * self.x
      
      F->Sub(v, x(lhs), xo);
      F->Mul(t, s, v);
      F->Sub(yo, t, y(lhs)); 
      // y3=s*(x1-x3)-y1

       
      if (IsPointOnCurve (xo, yo)) {
	setPoint (out, xo, yo);
	return true; 
      }

      POUT("[case3 breakout]\n"); 
    }
    
    //printf ("%s[out:%s, lhs:%s, rhs:%s] \n", __FUNCTION__, out.c_str(), lhs.c_str(), rhs.c_str());  
    printf ("%s[how did u get to line:%i?]\n", __FUNCTION__, __LINE__);  
    return false; 
  }



  //
  //
  bool EC_con_imp2::AddPoint (const std::string& out, const std::string& lhs, const std::string& rhs)
  {
    //printf ("%s[out:%s, lhs:%s, rhs:%s] \n", __FUNCTION__, out.c_str(), lhs.c_str(), rhs.c_str());  
    ScopeDeleter dr (F);
    if (!ptexs(lhs) ) {
      POUT ("lhs DNE");
      return false;
    }
    
    if ( !ptexs(rhs) ) {
      POUT ("rhs DNE");
      return false;
    }
    
    pt::struc res;
    x(res) = F->New ();
    y(res) = F->New (); 

    if (addPoint (res, point(lhs), point(rhs))) {
      return (ptexs (out) ? setPoint (point(out), dr(x(res)), dr(y(res))) : makePoint (point(out), x(res), y(res)));
    }
    
    printf ("%s[out:%s, lhs:%s, rhs:%s] ....FAILED \n", __FUNCTION__, out.c_str(), lhs.c_str(), rhs.c_str());  
    return false; 
  }

  //
  //

  bool EC_con_imp2::Mul_scalar (const std::string& O, FE_t s, const std::string& P) {

    //printf ("%s[%s, %s, %s]\n", __FUNCTION__, O.c_str(), s.c_str(), P.c_str()); 
    if (!ptexs(P)) {
      printf ("point %s DNE\n", P.c_str());
      return false;
    }

    if (!F->IsValid(s)){
      printf ("multiplier \'s\' is invalid\n" );
      return false;
    }

    ScopeDeleter dr (F);
    pt::struc  curr; 
    x(curr) = F->New ();
    y(curr) = F->New ();
    copyPoint (curr, point(P)); 
    //printPoint ("  curr", curr, Format::HEX); 
    pt::struc acc;
    x(acc) = F->New ();
    y(acc) = F->New ();
    setINF (acc); 
    //printPoint ("  acc", acc, Format::HEX); 
    FE_t coeff = dr (F->New ()); // 
    F->Set (coeff, s);     // initialize to scalar multiplier
    //printElem("  coeff", coeff, Format::HEX); 
    //printElem ("coeff start:", coeff, Format::DEC); 
    //size_t dbg_count = 0;
    const size_t max_iteration_limit = 2048; 
    size_t iteration_count = 0; 

    while (0 != F->Cmp_ui (coeff, 0)) { //  && dbg_count < 260)
      //printElem ("++coeff++", coeff, Format::HEX);
      if (F->TestBit (coeff, 0)) {
	bool res = (isINF(acc) ? copyPoint (acc, curr) : addPoint (acc, acc, curr)); 
      }
      
      bool addres = addPoint (curr, curr, curr);

      if (!addres)
	break; 
      
      iteration_count++;
      
      F->LogiShiftR (coeff, 1); // coeff >>= 1
    
    }
    // printf ( "[%s|iterator_count:%zu]\n", __FUNCTION__, iteration_count);
  
    if (ptexs(O)) {
      dr(x(acc));
      dr(y(acc));
      return copyPoint (point(O), acc); 
    }
    else {
      return makePoint(point(O), x(acc), y(acc));  
    }
    
  
    return false;
  }


  bool EC_con_imp2::Mul_scalar (const std::string& O, const std::string& s, const std::string& P) {
    //printf ("%s[%s, %s, %s]\n", __FUNCTION__, O.c_str(), s.c_str(), P.c_str()); 
    if (!ptexs(P)) {
      printf ("point %s DNE\n", P.c_str());
      return false;
    }

    if (!elexs(s)) {
      printf ("elem %s DNE\n", s.c_str());
      return false;
    }
     
    
    return Mul_scalar (O, elem(s), P); 

  }  
  //
  //
  bool EC_con_imp2:: Mul_scalar_ui (const std::string& O, size_t s_, const std::string& P)
  {
    //    printf ("%s[o:%s, s:%zu, P:%s]\n", __FUNCTION__, O.c_str(), s_, P.c_str());
    if ( !ptexs(P))
      {
	POUT ("P=DNE");
	return false;
      }

    ScopeDeleter dr (F);
    pt::struc acc;
    x(acc) = F->New();
    y(acc) = F->New();
    setINF (acc); 
    
    pt::struc cur;
    x(cur) = dr(F->New());
    y(cur) = dr(F->New()); 
    copyPoint (cur, point(P)); 

    size_t scoeff = s_;
    const size_t max_iteration_limit = 2048; 
    size_t iteration_count = 0; 
    
    
    while (scoeff && iteration_count++ < max_iteration_limit) {
      if (scoeff & 0x1) {
	bool ret =  (is_INF (acc) ? copyPoint (acc, cur) : addPoint (acc, acc, cur));
      }
      
      addPoint (cur, cur, cur); 
      
      scoeff >>= 1; 
    }
    
    
     if (ptexs(O)) {
       dr(x(acc));
       dr(y(acc));
       return copyPoint (point(O), acc); 
     }
     else {
       return  makePoint(point(O), x(acc), y(acc));  
     }
   
     
     return false; 
  }
 
  
  //
  //
  bool EC_con_imp2::IsPointOnCurve (FE_t ptx, FE_t pty)
  {
    //printf ("%s:[%i, %i]|ln:%i\n", __FUNCTION__, ptx, pty, __LINE__);
    //printf ("%s[x:%s, y:%s):", __FUNCTION__, xbuf, ybuf); 
    char xbuf[256];
    char ybuf[256];
    
    // negative element names are define to represent infinity
    if (ptx < 0 || pty < 0)
      {

	
	printf ("%s:[%i, %i]|ln:%i\n", __FUNCTION__, ptx, pty, __LINE__);
	// is this correct?
	return false; 
      }

 
    ScopeDeleter dr (F); 
    
    FE_t lhs = dr(F->New ());
    FE_t rhs = dr(F->New ());
    FE_t t0  = dr(F->New ());  
    FE_t t1  = dr(F->New ());
    FE_t t2  = dr(F->New ()); 
    // y^2 = x^3 + ax + b
    
    F->Pow_ui (t0, ptx, 3); 
    F->Mul    (t1, coeff_a, ptx);
    F->Add    (t2, t0, t1); 
    F->Add    (rhs, t2, coeff_b);
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

    
    //printElem ("lhs", lhs, Format::HEX);
    //printElem ("rhs", rhs, Format::HEX);

    
    F->Format (xbuf, "%Zx", ptx);
    F->Format (ybuf, "%Zx", pty); 

    //printf ("%s[%s, %s]:%s\n", __FUNCTION__, xbuf, ybuf, (res == 0? "true" : "false") ); 
    return res == 0;
  }

   
  bool EC_con_imp2::IsPointOnCurve (const std::string& xco, const std::string& yco) {
    if (!elexs (xco)) {
      return false;
    }
    
    if (!elexs (yco)) {
      return false;
    }

    return IsPointOnCurve(elem(xco), elem(yco));

  }
  
  bool EC_con_imp2::IsPointOnCurve (const std::string& P) {

    if (!ptexs(P)) {
	return false;
      }
    
    return IsPointOnCurve (x(point(P)), y(point(P)));

  }
 
} // FFM namespace 
