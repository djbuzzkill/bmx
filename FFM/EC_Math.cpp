#include "EC_Math.h"


#include <gmp.h>

#include <tuple>

namespace FFM
{

#define GETCTX(x) std::get<2>((x))

  



  typedef std::tuple<FE_t, FE_t> ElemTuple; 

  inline FE_t& x(ElemTuple& t) { return std::get<0> (t); }
  inline FE_t& y(ElemTuple& t) { return std::get<1> (t); }
  
  inline const FE_t& x(const ElemTuple& t) { return std::get<0> (t); }
  inline const FE_t& y(const ElemTuple& t) { return std::get<1> (t); }



  
  inline FE_t& a(ElemTuple& t) { return std::get<0> (t); }
  inline FE_t& b(ElemTuple& t) { return std::get<1> (t); }

  inline const FE_t& a(const ElemTuple& t) { return std::get<0> (t); }
  inline const FE_t& b(const ElemTuple& t) { return std::get<1> (t); }


  struct EC_con_impl : public EC_context , public std::enable_shared_from_this<EC_con_impl> 
  {
    EC_con_impl (FEConPtr F); 


    ~EC_con_impl (); 
    // s * P , 
     bool Mul (const std::string& out, const std::string& s,  const std::string& P); 
     bool Add (const std::string& out, const std::string& lhs, const std::string& rhs);

    // initialize point with elements
     bool DefPoint (const std::string& sym) ;
     bool DefCoeffs (const std::string& sym);
     bool DefElem  (const std::string& sym) ;
    
   // return to source
     bool UndefPoint (const std::string&) ;
     bool UndefCoeffs (const std::string& ); 
     bool UndefElem  (const std::string&) ;
   
     void PrintPoint (const std::string& , const std::string&, EC_Format f = EC_Format::DEC) ;
     void PrintCoeffs (const std::string& , const std::string&, EC_Format f= EC_Format::DEC); 
     void PrintElem (const std::string& , const std::string&, EC_Format f= EC_Format::DEC); 
    
    bool SetPoint (const std::string&, const char*, const char*, size_t base ) ; 
    bool SetCoeffs (const std::string& , const char* strv_a, const char* strv_b, size_t base ) ;
    bool SetElem (const std::string& , const char* strv, size_t base ) ;
    
    bool SetPoint_ui (const std::string& , size_t, size_t) ;
    bool SetCoeffs_ui (const std::string& , size_t, size_t) ;
    bool SetElem_ui (const std::string& , size_t x) ;
    
    //  bool EC_IsAtInfinity (const EC_Point& P);
    bool IsPointOnCurve (const std::string& eq, const std::string& P);

    // -------------------------------------------------------------------
    //
    inline bool pt_exists (const std::string& s) { return pointmap.count(s) ? true : false; }
    inline bool co_exists (const std::string& s) { return curvemap.count(s) ? true : false; }
    inline bool el_exists (const std::string& s) { return elmap.count(s) ? true : false; }


    ElemTuple& curve (const std::string& name) { return curvemap[name]; }
    const ElemTuple& ccurve (const std::string& name) { return curvemap.at(name); }
    
    ElemTuple& point (const std::string& name) { return pointmap[name]; }
    const ElemTuple& cpoint (const std::string& name) { return pointmap.at(name); }


    FE_t& elem (const std::string& name) { return elmap[name]; }
    const FE_t& celem  (const std::string& name) { return elmap.at(name); }
 
    
    typedef std::map<std::string, ElemTuple> PointMap;
    typedef std::map<std::string, ElemTuple> CurveMap;
    typedef std::map<std::string, FE_t> ElementMap; 
    
    FEConPtr F;  
    PointMap pointmap;
    CurveMap curvemap; 
    ElementMap elmap;

    std::vector<FE_t> localtemp; 

    enum { kTempStackSize = 32 } ; 

    inline const FE_t& loc (size_t i) { return localtemp[i]; }

  };

  
  // 
  EC_con_impl::EC_con_impl (FEConPtr fc)
    : F(fc)
    , pointmap()
    , curvemap()
    , elmap()
    , localtemp(kTempStackSize)
  {

    for (size_t i = 0; i < localtemp.size (); ++i)
      localtemp[i] = F->New (); 

	  }

  //
  EC_con_impl::~EC_con_impl ()
  {

    for (auto& el : localtemp)
      {
	F->Del (el);
      }
    
  }

  bool EC_con_impl:: Mul (const std::string& out, const std::string& s,  const std::string& P)
  {
    return false; 

  }
  
  bool EC_con_impl::Add (const std::string& out, const std::string& lhs, const std::string& rhs)
  {
    return false;

  }

 //
  //
  bool EC_con_impl::IsPointOnCurve (const std::string& eq, const std::string& P)
  {

    if (!pt_exists (P) || !co_exists(eq))
      return false; 
      
    enum { x0, x1, rhs, y0, y1, lhs } ; 

    // y^2 = x^3 + ax + b
    
    F->Pow_ui (loc(x0), x(point(P)), 3); 

    F->Mul (loc(x1), a(curve(eq)), x(point(P)));
    F->Add (loc(rhs), loc(x0), loc(x1)); 
    F->Add (loc(rhs), loc(rhs), b(curve(eq)));
    
    F->Mul (loc(lhs), y(point(P)), y(point(P)));

    // lhs > rhs :
    // +
    // lhs = rhs : 0
    // lhs < rhs : - 
    int res = F->Cmp (loc(lhs), loc(rhs));

    return res == 0;
  }

  ECConRef Create_EC_context (FEConPtr F) {

    ECConRef ret (std::make_shared<EC_con_impl> (F));

   return ret; 
  }
    

  // initialize point with elements
  bool  EC_con_impl::DefPoint (const std::string& sym)
  {
   // 
    if (pt_exists (sym))
      return false; 
    
    ElemTuple& out = point(sym);

    x(out) = F->New ();
    y(out) = F->New ();

    return true; 
  }

 
  bool EC_con_impl::DefCoeffs (const std::string& sym)
  {
    if (co_exists (sym))
    {
      return false; 
    }

    ElemTuple& out = curve(sym);
    a(out) = F->New ();
    b(out) = F->New (); 
    return true; 

  }


  bool EC_con_impl::DefElem (const std::string& sym)
  {
    if (pt_exists(sym))
      return false;

    elmap[sym] = F->New (); 
    
    return true; 
  }
  
  // return to source
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
  
    
  bool EC_con_impl::UndefCoeffs (const std::string& sym) {


    if (co_exists (sym))
      {
	CurveMap::iterator it = curvemap.find (sym); 
	if (it != curvemap.end ())
	  {
	    F->Del (a(it->second));
	    F->Del (b(it->second));
	    curvemap.erase (it);
	    return true; 

	  }

      }
    return false; 
  }

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
  


void EC_con_impl::PrintCoeffs  (const  std::string& lbl, const std::string& eq , EC_Format fmt)
  {

    char xs_[256];
    char ys_[256];

    switch (fmt)
      {
      case EC_Format::HEX:
	F->Format (xs_, "%Zx", a(curve(eq)));
	F->Format (ys_, "%Zx", b(curve(eq)));
	printf ( "%s[a:0x%s, b:0x%s]\n", lbl.c_str(), xs_, ys_ ); 
	break;
	
      case EC_Format::DEC:
      default: 
	F->Format (xs_, "%Zd", a(curve(eq)));
	F->Format (ys_, "%Zd", b(curve(eq)));
	printf ( "%s[a:%s, b:%s]\n", lbl.c_str(), xs_, ys_ ); 
	break;
      }	
  } 

   
  bool EC_con_impl::SetPoint (const std::string& sym, const char* strv_x, const char* strv_y, size_t base )
  {
    if (!pt_exists (sym))
      return false; 
    
    ElemTuple& out = point(sym); 
    F->Set (x(out), strv_x, base);
    F->Set (y(out), strv_y, base);
    return true; 

  }
   
  bool EC_con_impl::SetCoeffs (const std::string& out, const char* strv_a, const char* strv_b, size_t base )
  {
    if (!co_exists(out))
      return false;

    
    ElemTuple& tup = curve (out);
    
    F->Set (a(tup), strv_a, base);
    F->Set (b(tup), strv_b, base);
    return true; 


  } 


  bool EC_con_impl::SetElem (const std::string& el, const char* str_v, size_t base)
  {
    if (!el_exists(el))
      return false;

    F->Set (elem(el), str_v, base);
    return true; 
    
  }
 

  bool EC_con_impl::SetPoint_ui (const std::string& sym, size_t x_, size_t y_)
  {
    if (!pt_exists (sym))
      return false; 
    
    const ElemTuple& out = point(sym); 
    F->Set_ui (x(out), x_);
    F->Set_ui (y(out), y_);
    return true; 

  }
   
  bool EC_con_impl::SetCoeffs_ui (const std::string& out, size_t a_, size_t b_ )
  {
    if (!co_exists(out))
      return false;

    
    const ElemTuple& tup = curve (out);
    
    F->Set_ui (a(tup), a_);
    F->Set_ui (b(tup), b_);
    return true; 


  } 


  
  bool EC_con_impl::SetElem_ui (const std::string& el, size_t v)
  {
    if (!el_exists(el))
      return false;

    
    F->Set_ui (elem(el), v);
    return true; 

    
  }
  

 
}
