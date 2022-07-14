#include "ec_math.h"
#include "ffm/fe_math.h"

#include <gmp.h>

#include <tuple>

namespace ffm
{


  typedef std::tuple<FE_t, FE_t> elem_tuple;

  
  inline FE_t& x(elem_tuple& t) { return std::get<0> (t); }
  inline FE_t& y(elem_tuple& t) { return std::get<1> (t); } 
  inline const FE_t& x(const elem_tuple& t) { return std::get<0> (t); }
  inline const FE_t& y(const elem_tuple& t) { return std::get<1> (t); }

    
  inline FE_t& a(elem_tuple& t) { return std::get<0> (t); }
  inline FE_t& b(elem_tuple& t) { return std::get<1> (t); }

  inline const FE_t& a(const elem_tuple& t) { return std::get<0> (t); }
  inline const FE_t& b(const elem_tuple& t) { return std::get<1> (t); }

  //
  //
  //
  class EC_con_imp1 : public EC_context //  , public std::enable_shared_from_this<EC_con_impl> 
  {
  public:
    
    EC_con_imp1 (FEConPtr fc, const char* astr, const char* bstr, const char*, size_t base);
    ~EC_con_imp1 ();  
   // s * P , 
    //bool Add (const std::string& out, const std::string& lhs, const std::string& rhs);
    bool AddPoint (const std::string& out, const std::string& lhs, const std::string& rhs);
    
    bool addPoint (elem_tuple& out, const elem_tuple& lhs, const elem_tuple& rhs);
     
    // bool Mul_scalar (const std::string& O, const std::string& s,  const std::string& P); 
    bool Mul_scalar_ui (const std::string& O, size_t s, const std::string& P); 
    // bool SetCoeffs (const char* a, const char* b, size_t base);
    bool Mul_scalar (const std::string& O, const std::string& s, const std::string& P);   
    // bool SetCoeffs_ui (size_t, size_t);

    bool Mul_scalar (const std::string& O, FE_t s, const std::string& P); 

    // initialize point with elements
    //bool DefPoint (const std::string& sym, const char* a, const char* , size_t) ;
    // bool CopyPoint (const std::string& sym, const std::string& P); 
    //bool DefPoint_ui (const std::string& sym, size_t, size_t); 

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


    void PrintPoint (const std::string& , const std::string&, format f = format::dec) ;
    void PrintCoeffs (const std::string&, format f= format::dec); 
    void PrintElem (const std::string& , const std::string&, format f= format::dec); 
    void PrintElem (const std::string& , FE_t, format f= format::dec); 

    void printElem (const std::string&, FE_t e, format f); 
    void printPoint (const std::string& lbl, const elem_tuple& P, format fmt);
    

//  bool EC_IsAtInfinity (const EC_Point& P);
    bool IsPointOnCurve (FE_t, FE_t); 
    bool IsPointOnCurve (const std::string& xco, const std::string& yco);   
    bool IsPointOnCurve (const std::string& P);  
 
    // bool ECDSA_verify (const char* sz_z, const char* sz_r, const char* sz_s);
    
    // -------------------------------------------------------------------
    //
    inline bool pt_exists (const std::string& s) { return pointmap.count(s) ? true : false; }
    //inline bool co_exists (const std::string& s) { return curvemap.count(s) ? true : false; }
    inline bool el_exists (const std::string& s) { return elmap.count(s) ? true : false; }

    elem_tuple& point (const std::string& name) { return pointmap[name]; }
    const elem_tuple& point (const std::string& name) const { return pointmap.at(name); }
    FE_t& elem (const std::string& name) { return elmap[name]; }
    const FE_t& elem  (const std::string& name) const { return elmap.at(name); }
 
   inline bool is_INF (FE_t x) { return (x < 0 ? true : false); }
    inline bool is_INF (const elem_tuple& tup) { return is_INF (x(tup)) || is_INF(y(tup)); }


    inline elem_tuple& unINF (elem_tuple& tup) {
      x(tup) = is_INF(x(tup)) ? -x(tup) : x(tup);
      y(tup) = is_INF(y(tup)) ? -y(tup) : y(tup); 
      return tup;
    }
    
    inline bool makePoint (elem_tuple& place, FE_t xo, FE_t yo) {
      if (is_INF(xo)) {
	// should we say something
      }
      
      if (is_INF(yo)) {
	// should we say something
      }

      unINF(place); 
      x(place) = xo;
      y(place) = yo;
      return true;
    }

    
    inline bool makePoint (elem_tuple& place, const elem_tuple& rhs) {
      return makePoint (place, x(rhs), y(rhs)); 
    }

    
    // copy value data
    inline bool setPoint(elem_tuple& place, FE_t xo, FE_t yo)
    {
      unINF(place); 
      F->Set (x(place), xo);
      F->Set (y(place), yo);

      return true; 
    }
 
    inline bool copyPoint(elem_tuple& lhs, const elem_tuple& rhs)
    {
      //printf ("copeePee[(%i, %i), (%i, %i)] \n", x(lhs), y(lhs), x(rhs), y(rhs)); 
      
      return setPoint (lhs, x(rhs), y(rhs)); 
    }

   
    inline elem_tuple& set_INF(elem_tuple& P)
    {
      x(P) = x(P) < 0 ? x(P) : -x(P);
      y(P) = y(P) < 0 ? y(P) : -y(P);
      return P;
    }
    
    
    typedef std::map<std::string, elem_tuple> PointMap;
    typedef std::map<std::string, elem_tuple> CurveMap;
    typedef std::map<std::string, FE_t> ElementMap; 
    FEConPtr F;  
    PointMap pointmap;

    ElementMap elmap;
    elem_tuple coeffs;
    FE_t      n; 
  private: 

  };

  // EC_con_impl (FEConPtr F, const char*, const char*, size_t); 
  EC_con_imp1::EC_con_imp1 (FEConPtr fc, const char* astr, const char* bstr, const char* n_sz, size_t base)
      : F(fc)
      , pointmap()
      , coeffs ()
	//, curvemap()dd
      , elmap()
	 {
      n = F->New (n_sz, base); 
      a(coeffs) = F->New(astr, base);
      b(coeffs) = F->New(bstr, base);
      
      printf ("%s\n", __FUNCTION__);
      printElem ("  order(n):", n, format::hex);


      PrintCoeffs ("  coeffs:", format::hex);  
    }
    
  //
  //
  EC_con_imp1:: ~EC_con_imp1 () {
    printf ("%s\n", __FUNCTION__);  

    for (auto& p : pointmap) {
      F->Del (x(p.second));
      F->Del (y(p.second));
    }

    for (auto e : elmap) {
      F->Del(e.second); 
    }
      
    F->Del (a(coeffs)); 
    F->Del (b(coeffs));
    F->Del (n);
    }
 
  // 
  ECConRef Create_EC_context (FEConPtr F, const char* astr, const char* bstr, const char* order, size_t base) {

    ECConRef ret (std::make_shared<EC_con_imp1> (F, astr, bstr, order, base));

   return ret; 
  }

  bool EC_con_imp1::MakePoint (const std::string& sym, const char* strx, const char* stry, size_t base )
  {
    if (pt_exists(sym))
      {
	// must undef first
	return false; 
      }

    ScopeDeleter dr (F); 
    FE_t ptx = F->New(strx, base);
    FE_t pty = F->New(stry, base);

    if (IsPointOnCurve(ptx, pty)) {    
      return makePoint (point(sym), ptx, pty); 
    }

    dr(ptx);
    dr(pty); 
    return false; 
  }
 
  // 
  bool EC_con_imp1::MakePoint (const std::string& sym, const std::string& xel, const std::string& yel) {

    if (pt_exists(sym)) { 
      printf ("[%s] %s already defined \n", __FUNCTION__, sym.c_str());
      return false;
    }
    if (!el_exists(xel)) {
      printf ("[%s] %s DNE\n", __FUNCTION__, xel.c_str());
      return false;
    }
    if (!el_exists(yel)) {
      printf ("[%s] %s DNE\n", __FUNCTION__, yel.c_str());
      return false;
    }

    return setPoint (pointmap[sym], elem(xel), elem(yel));
    
    
  }

  bool EC_con_imp1::MakePoint (const std::string& sym, FE_t x, FE_t y) {
    if (pt_exists(sym)) {
      return false;  
      printf ("%s[sym:%s] : %s already defined\n", __FUNCTION__, sym.c_str(), sym.c_str()); 
    }
 

    return makePoint (point(sym), x, y); 
  }
 
  //
  //
  bool EC_con_imp1::MakePoint_ui (const std::string& sym, size_t px, size_t py)
  {

    if (pt_exists(sym))
      {
	// already defined, must undef or soemthing
	return false;

      }
      
    ScopeDeleter dr (F); 
    FE_t ptx = F->New_ui (px);
    FE_t pty = F->New_ui (py);
    
    if (IsPointOnCurve(ptx, pty)) {
      return makePoint (point(sym), ptx, pty);
    }
    

    dr (ptx);
    dr (pty); 
    return false;
  }
 
  // 
  bool EC_con_imp1::SetPoint (const std::string& sym, const std::string& xel, const std::string& yel) {

    if (!pt_exists(sym)) { 
      printf ("[%s] %s DNE\n", __FUNCTION__, sym.c_str());
      return false;
    }
    if (!el_exists(xel)) {
      printf ("[%s] %s DNE\n", __FUNCTION__, xel.c_str());
      return false;
    }
    if (!el_exists(yel)) {
      printf ("[%s] %s DNE\n", __FUNCTION__, yel.c_str());
      return false;
    }

    return setPoint (pointmap[sym], elem(xel), elem(yel));
    
  }

  bool EC_con_imp1::CopyPoint (const std::string& lhs, const std::string& rhs) {

    if (!pt_exists(lhs)) { 
      printf ("[%s] %s DNE\n", __FUNCTION__, lhs.c_str());
      return false;
    }
 
    if (!pt_exists(rhs)) { 
      printf ("[%s] %s DNE\n", __FUNCTION__, rhs.c_str());
      return false;
    }
    
    return copyPoint(point(lhs), point(rhs));  
  }
 
  bool EC_con_imp1::MakeElem (const std::string& sym, const std::string& ssym) {

    if (!el_exists(ssym)) {
      printf ("%s does not exist\n", ssym.c_str()); 
      return false;
      
    }
    
    if (!pt_exists(sym)) {
      elem(sym) = F->New ();  	
    }
    F->Set (elem(sym), elem(ssym));  
    return true; 
  }
  

  
  bool EC_con_imp1::MakeElem (const std::string& sym, const char* strv, size_t base) {
    if (!el_exists(sym)) {
      elem(sym) = F->New(strv, base);  	
    }
    else {
      // already defined
      F->Set (elem(sym), strv, base);  
    }
    return true; 
  }
  
  // return to source
  bool EC_con_imp1::MakeElem_ui (const std::string& sym, size_t v) {
    if (!el_exists(sym)) {
	elem(sym) = F->New(); 

      }

    F->Set_ui(elem(sym), v);
    return true; 
  }

  bool EC_con_imp1::SetElem (const std::string& dst, const std::string& src) {
    if (!el_exists(src)) {
      return false;
    }

    if (!el_exists(dst)) {
      return false;
    }

    F->Set (elem(dst), elem(src)); 
    return true; 
  }
    

  bool EC_con_imp1::DelPoint (const std::string& sym)
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
  
  bool EC_con_imp1::DelElem (const std::string& sym) {
    if (el_exists(sym)) {

      ElementMap::iterator it = elmap.find (sym);

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
  void EC_con_imp1::printPoint (const std::string& lbl, const elem_tuple& P, format fmt) {
    char xs_[256];
    char ys_[256];


    if (is_INF (P)) {
      printf ("[%s is INF]\n", lbl.c_str());
      return; 
    }
      
    switch (fmt) {
    case format::hex:  
      F->Format (xs_, "%Zx", x(P));
      F->Format (ys_, "%Zx", y(P));
      printf ( "%s[x:0x%s, y:0x%s]\n", lbl.c_str(), xs_, ys_ ); 
      break;
      
    case format::dec: 
      
    default: 
      F->Format (xs_, "%Zd", x(P));
      F->Format (ys_, "%Zd", y(P)); 
      printf ( "%s[x:%s, y:%s]\n", lbl.c_str(), xs_, ys_ ); 
      break;
    }	
    
  }
  

  //
  //
  void EC_con_imp1::PrintPoint (const std::string& lbl, const std::string& P, format fmt)
  {
     if (!pt_exists(P)) {
      printf ("%s: \"%s\" is not defined\n", lbl.c_str(), P.c_str()); 
      return;
    }

     printPoint (lbl, point(P), fmt); 
     
  } 

  //
  void EC_con_imp1::printElem (const std::string& lbl, FE_t e, format fmt)
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
  void EC_con_imp1::PrintElem (const std::string& lbl , FE_t e, format fmt )
  {
    printElem (lbl, e, fmt); 
  }
  
  //
  //
  void EC_con_imp1::PrintElem (const std::string& lbl , const std::string& e, format fmt)
  {
    printElem (lbl, elem(e), fmt); 
  }
  
  //
  //
  void EC_con_imp1::PrintCoeffs  (const  std::string& lbl , format fmt)
  {

    char xs_[256];
    char ys_[256];

    switch (fmt)
      {
      case format::hex:
	F->Format (xs_, "%Zx", a(coeffs));
	F->Format (ys_, "%Zx", b(coeffs));
	printf ( "%s[a:0x%s, b:0x%s]\n", lbl.c_str(), xs_, ys_ ); 
	break;
	
      case format::dec:
      default: 
	F->Format (xs_, "%Zd", a(coeffs));
	F->Format (ys_, "%Zd", b(coeffs));
	printf ( "%s[a:%s, b:%s]\n", lbl.c_str(), xs_, ys_ ); 
	break;
      }	
  } 
 
  //
  // 
  bool EC_con_imp1::addPoint (elem_tuple& out,const elem_tuple& lhs,const elem_tuple& rhs)
  {
    //printf ("%s[lhs:<%i, %i>, rhs:<%i, %i>]\n", __FUNCTION__, x(lhs), y(lhs), x(rhs), y(rhs)); 
    
    ScopeDeleter dr (F);
    
    FE_t s = dr(F->New());
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
    if (is_INF(x(lhs))) {
      // Case 0.0: self is the point at infinity, return other
      POUT("[case0.0: lhs.x INF]");
      set_INF(out);
      return false;
    }
    
    if (is_INF(x(rhs))) {
      POUT("[case0.1: rhs.x INF]");
      // Case 0.1: other is the point at infinity, return self
      set_INF(out);
      return false;
    }
    
    if (is_INF (y(lhs))) {
      POUT ("y.lhs is INF");
      return false;
    }
   
    
    if (is_INF (y(rhs))) {
      POUT ("y.rhs is INF");
      return false;
    }



    
    // const elem_tuple& R = point(rhs); 
    // const elem_tuple& L = point(lhs);
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
      
      set_INF (out);
      
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
      set_INF (out);
      return false;
      
    }
    else if (cmpx == 0 && cmpy == 0) {
      // POUT("[case3:lhs==rhs]"); 
      // Formula (x3,y3)=(x1,y1)+(x1,y1)
      // s=(3*x1**2+a)/(2*y1)
      // x3=s**2-2*x1
      // y3=s*(x1-x3)-y1
      
/*  
    BEGIN_THIS_VERSION_CUR
	  
    F->Mul (u, x(lhs), x(lhs));
    F->Add (t, u, u);
    F->Add (t, t, u);  
    F->Add(s_n, t, a(coeffs));
    
    F->Add (s_d, y(lhs), y(lhs));  
    
    F->Div (s, s_n, s_d);
    // s = (3 * self.x**2 + self.a) / (2 * self.y)
    
    F->Mul (ss, s, s);
    F->Add(u, x(lhs),  x(lhs)); 
    
    F->Sub(xo, ss, u);
    // x = s**2 - 2 * self.x
    
    F->Sub(v, x(lhs), xo);
    F->Mul(t, s, v);
    F->Sub(yo, t, y(lhs)); 
    // y3=s*(x1-x3)-y1

*/  //END_THS_VERSION_CUR


      F->Set_ui(v, 3);
      
      F->Pow_ui(u, x(lhs), 2);
      F->Mul(t, u, v);

      F->Add(s_n, t, a(coeffs));
      
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

      //      printElem ("<s_n>", s_n, Format::HEX);
      //printElem ("<s_d>", s_d, Format::HEX);
      
      //printElem ("<u>", u, Format::HEX);
      //printElem ("<v>", v, Format::HEX); 

       
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
  bool EC_con_imp1::AddPoint (const std::string& out, const std::string& lhs, const std::string& rhs)
  {
    //printf ("%s[out:%s, lhs:%s, rhs:%s] \n", __FUNCTION__, out.c_str(), lhs.c_str(), rhs.c_str());  
    ScopeDeleter dr (F);
    if (!pt_exists(lhs) ) {
      POUT ("lhs DNE");
      return false;
    }
    
    if ( !pt_exists(rhs) ) {
      POUT ("rhs DNE");
      return false;
    }
    
    elem_tuple res;
    x(res) = F->New ();
    y(res) = F->New (); 

    if (addPoint (res, point(lhs), point(rhs))) {
      return (pt_exists (out) ? setPoint (point(out), dr(x(res)), dr(y(res))) : makePoint (point(out), res));
    }
    
    printf ("%s[out:%s, lhs:%s, rhs:%s] ....FAILED \n", __FUNCTION__, out.c_str(), lhs.c_str(), rhs.c_str());  
    return false; 
  }

  //
  //

  bool EC_con_imp1::Mul_scalar (const std::string& O, const std::string& s, const std::string& P) {
    //printf ("%s[%s, %s, %s]\n", __FUNCTION__, O.c_str(), s.c_str(), P.c_str()); 
    if (!pt_exists(P)) {
      printf ("point %s DNE\n", P.c_str());
      return false;
    }
    if (!el_exists(s)) {
      printf ("element %s DNE\n", s.c_str());
      return false; 
    }
    return Mul_scalar (O, elem(s), P); 

  }

  bool EC_con_imp1::Mul_scalar (const std::string& O, FE_t s, const std::string& P) {
    //printf ("%s[%s, %s, %s]\n", __FUNCTION__, O.c_str(), s.c_str(), P.c_str()); 
    if (!pt_exists(P)) {
      printf ("point %s DNE\n", P.c_str());
      return false;
    }
    if (!F->IsValid(s)) {
      printf ("multiplier \'s\' is invalid\n" );
      return false; 
    }

    ScopeDeleter dr (F);
    elem_tuple curr;
    x(curr) = F->New ();
    y(curr) = F->New ();
    copyPoint (curr, point(P)); 
    //printPoint ("  curr", curr, Format::HEX); 
    elem_tuple acc;
    x(acc) = F->New ();
    y(acc) = F->New ();
    set_INF (acc); 
    //printPoint ("  acc", acc, Format::HEX); 
    FE_t coeff = dr (F->New ()); // 
    F->Set (coeff, s);     // initialize to scalar multiplier
    //printElem("  coeff", coeff, Format::HEX); 
    //printElem ("coeff start:", coeff, Format::DEC); 
    //size_t dbg_count = 0;
    const size_t max_iteration_limit = 2048; 
    size_t iteration_count = 0; 

    while (0 != F->Cmp_ui (coeff, 0) && iteration_count++ < max_iteration_limit ) { //  && dbg_count < 260)
      //printElem ("++coeff++", coeff, Format::HEX);
      if (F->TestBit (coeff, 0)) {
	bool res = (is_INF(acc) ? copyPoint (acc, curr) : addPoint (acc, acc, curr)); 
      }
    
      addPoint (curr, curr, curr);   
    
      F->LogiShiftR (coeff, 1); // coeff >>= 1
    
      //PrintElem ("coeff loop:", coeff, Format::DEC);
      //printf ("debug_count:%zu\n", dbg_count); 
      //dbg_count++;
    }
    printf ( "[%s|iterator_count:%zu]\n", __FUNCTION__, iteration_count);
  
    if (pt_exists(O)) {
      dr(x(acc));
      dr(y(acc));
      return copyPoint (point(O), acc); 
    }
    else {
      return makePoint(point(O), acc);  
    }
    
  
    return false;
  }

  //
  //
  bool EC_con_imp1:: Mul_scalar_ui (const std::string& O, size_t s_, const std::string& P)
  {
    printf ("%s[o:%s, s:%zu, P:%s]\n", __FUNCTION__, O.c_str(), s_, P.c_str());
    if ( !pt_exists(P))
      {
	POUT ("P=DNE");
	return false;
      }

    ScopeDeleter dr (F);
    elem_tuple acc;
    x(acc) = F->New();
    y(acc) = F->New();
    set_INF (acc); 
    
    elem_tuple cur;
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
    
    
     if (pt_exists(O)) {
       dr(x(acc));
       dr(y(acc));
       return copyPoint (point(O), acc); 
     }
     else {
       return  makePoint(point(O), acc);  
     }
   
     
     return false; 
  }
 
  
  //
  //
  bool EC_con_imp1::IsPointOnCurve (FE_t ptx, FE_t pty)
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

    //F->Format (xbuf, "%Zx", ptx);
    //F->Format (ybuf, "%Zx", pty); 

    //printf ("%s[%s, %s]:%s\n", __FUNCTION__, xbuf, ybuf, (res == 0? "true" : "false") ); 
    return res == 0;
  }
  
  bool EC_con_imp1::IsPointOnCurve (const std::string& xco, const std::string& yco) {
    if (!el_exists (xco)) {
      return false;
    }
    
    if (!el_exists (yco)) {
      return false;
    }

    return IsPointOnCurve(elem(xco), elem(yco));

  }
  
  bool EC_con_imp1::IsPointOnCurve (const std::string& P) {

    if (!pt_exists(P)) {
	return false;
      }
    
    return IsPointOnCurve (x(point(P)), y(point(P)));

  }
 
  /* 
     
  //bool EC_con_impl::Add (const elem_tuple& out,const elem_tuple& lhs,const elem_tuple& rhs)
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
    
    // not sure yet 
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
     
    const elem_tuple& R = point(rhs); 
    const elem_tuple& L = point(lhs);
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
	    if (pt_exists(out)) {
		setPoint (point(out), xo, yo); 
		dr (xo);
		dr (yo);
	      }
	    else {
		makePoint (point(out), xo, yo); 
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

	if (IsPointOnCurve (xo, yo)) {
	  if (pt_exists(out)) {
	    setPoint (point(out), xo, yo); 
	    dr (xo);
	    dr (yo); 
	  }
	  else {
	    makePoint(point(out), xo, yo); 
	  }
	  return true; 
	}
	else {
	  printf ("POINT NOT ON CURVE|ln:%i\n", __LINE__);
	  
	}
      }
 
    POUT("return end");  
    return false; 
  }


  */
  class EC_con_imp2 : public EC_context //  , public std::enable_shared_from_this<EC_con_impl> 
  {
  public:
    
    EC_con_imp2 (FEConPtr fc, el::map& em, pt::map& pm, const char* astr, const char* bstr, const char*, size_t base);
    ~EC_con_imp2 ();  
   // s * P , 
    //bool Add (const std::string& out, const std::string& lhs, const std::string& rhs);
    bool AddPoint (const std::string& out, const std::string& lhs, const std::string& rhs);
    
     
    // bool Mul_scalar (const std::string& O, const std::string& s,  const std::string& P); 
    bool Mul_scalar_ui (const std::string& O, size_t s, const std::string& P); 
    // bool SetCoeffs (const char* a, const char* b, size_t base);
    bool Mul_scalar (const std::string& O, const std::string& s, const std::string& P);   
    // bool SetCoeffs_ui (size_t, size_t);

    bool Mul_scalar (const std::string& O, FE_t s, const std::string& P); 

    // initialize point with elements
    //bool DefPoint (const std::string& sym, const char* a, const char* , size_t) ;
    // bool CopyPoint (const std::string& sym, const std::string& P); 
    // bool DefPoint_ui (const std::string& sym, size_t, size_t); 

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

    inline pt::struc& setINF(pt::struc& P)
    {
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
    inline bool setPoint(pt::struc& place, FE_t xo, FE_t yo)
    {
      unINF(place); 
      F->Set (x(place), xo);
      F->Set (y(place), yo);

      return true; 
    }
 
    inline bool copyPoint(pt::struc& lhs, const pt::struc& rhs)
    {
      //printf ("copeePee[(%i, %i), (%i, %i)] \n", x(lhs), y(lhs), x(rhs), y(rhs)); 
      
      return setPoint (lhs, x(rhs), y(rhs)); 
    }

    bool addPoint (pt::struc& out, const pt::struc& lhs, const pt::struc& rhs);
   
   //    typedef std::map<std::string, elem_tuple> PointMap;
    //typedef std::map<std::string, elem_tuple> CurveMap;
    //typedef std::map<std::string, FE_t> ElementMap; 
    FEConPtr F;
    
    pt::map&  pointmap;

    el::map&  elmap;

    FE_t coeff_a;
    FE_t coeff_b;
  private: 

  };


  // 
  ECConRef Create_EC_context (FEConPtr F, el::map&  em, pt::map& pm, const char* astr, const char* bstr, const char* order, size_t base) {

    ECConRef ret (std::make_shared<EC_con_imp2> (F, em, pm, astr, bstr, order, base));

   return ret; 
  }

  // EC_con_impl (FEConPtr F, const char*, const char*, size_t); 
  EC_con_imp2::EC_con_imp2 (FEConPtr fc, el::map& em, pt::map& pm, const char* astr, const char* bstr, const char* n_sz, size_t base)
      : F(fc)
      , elmap (em)
      , pointmap(pm) {

    elmap["n"] = F->New (n_sz, base);
    elmap["Fp"] = F->p();
    
    elmap["coeff.a"] = coeff_a = F->New(astr, base);
    elmap["coeff.b"] = coeff_b = F->New(bstr, base);
    
    
    //   printf ("%s\n", __FUNCTION__);
    printElem ("  order(n):", elmap["n"], format::hex);
    //   PrintCoeffs ("  coeffs:", format::hex);  
  }
    
  //
  //
  EC_con_imp2:: ~EC_con_imp2 () {
    //printf ("%s\n", __FUNCTION__);  
    // for (auto& p : pointmap) {
    //   F->Del (x(p.second));
    //   F->Del (y(p.second));
    // }
    
    // for (auto e : elmap) {
    //   F->Del(e.second); 
    // }
    
    // F->Del (a(coeffs)); 
    // F->Del (b(coeffs));

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



    
    // const elem_tuple& R = point(rhs); 
    // const elem_tuple& L = point(lhs);
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
      
/*  
    BEGIN_THIS_VERSION_CUR
	  
    F->Mul (u, x(lhs), x(lhs));
    F->Add (t, u, u);
    F->Add (t, t, u);  
    F->Add(s_n, t, a(coeffs));
    
    F->Add (s_d, y(lhs), y(lhs));  
    
    F->Div (s, s_n, s_d);
    // s = (3 * self.x**2 + self.a) / (2 * self.y)
    
    F->Mul (ss, s, s);
    F->Add(u, x(lhs),  x(lhs)); 
    
    F->Sub(xo, ss, u);
    // x = s**2 - 2 * self.x
    
    F->Sub(v, x(lhs), xo);
    F->Mul(t, s, v);
    F->Sub(yo, t, y(lhs)); 
    // y3=s*(x1-x3)-y1

*/  //END_THS_VERSION_CUR


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

      //      printElem ("<s_n>", s_n, Format::HEX);
      //printElem ("<s_d>", s_d, Format::HEX);
      
      //printElem ("<u>", u, Format::HEX);
      //printElem ("<v>", v, Format::HEX); 

       
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
