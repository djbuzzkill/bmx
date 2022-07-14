#ifndef FFM_EC_MATH_H
#define FFM_EC_MATH_H



#include "common.h"
#include "utility.h"
#include "fe_math.h"


namespace ffm
{
  //
  //
  enum class format : int {
    dec, hex } ; 

  //enum class format : int {
  //  DEC, HEX } ; 


  namespace el {

    inline bool is_INF (FE_t x) { return (x < 0 ? true : false); }
    

    struct map : public std::map<std::string, FE_t> {
      inline bool exs (const std::string& s) { return count(s) ? true : false; }
      //inline FE_t& el (const std::string& name) { return (*this)[name]; }
      inline const FE_t& el (const std::string& name) const { return at(name); }
    }; 


  }
  //
  //
  namespace pt {
    // 
    struct struc : public std::tuple<FE_t, FE_t> {
    };

    // 
    inline FE_t& x(struc& t) { return std::get<0> (t); }
    inline FE_t& y(struc& t) { return std::get<1> (t); } 
    inline const FE_t& x(const struc& t) { return std::get<0> (t); }
    inline const FE_t& y(const struc& t) { return std::get<1> (t); }
    
    //  
    struct map : public  std::map< std::string, struc> {
      inline bool exs (const std::string& s) { return count(s) ? true : false; }
      //inline bool co_exists (const std::string& s) { return curvemap.count(s) ? true : false; }

      //pt::struc& p (const std::string& name) { return (*this)[name]; }
      const pt::struc& pt (const std::string& name) const { return at(name); }
    }; 

    inline bool is_INF (FE_t x) { return (x < 0 ? true : false); }
    inline bool is_INF (const pt::struc& tup) { return is_INF (x(tup)) || is_INF(y(tup)); }
  }
  
  //
  //
  class EC_context : public Destructor {
  public:
    // virtual bool Add (const std::string& O, const std::string& lhs, const std::string& rhs) = 0;
    // O <= A+B 
    virtual bool AddPoint (const std::string& out, const std::string& lhs, const std::string& rhs) = 0;
    
    // s*P => R
    virtual bool Mul_scalar_ui (const std::string& O, size_t s, const std::string& P) = 0;
    virtual bool Mul_scalar (const std::string& O, const std::string& s, const std::string& P) = 0;  
    virtual bool Mul_scalar (const std::string& O, FE_t s, const std::string& P) = 0;  
  
    virtual bool MakeElem  (const std::string& sym, const std::string& src) = 0;
    virtual bool MakeElem  (const std::string& sym, const char* strv, size_t base) = 0;
    virtual bool MakeElem_ui (const std::string& , size_t x) = 0;
    virtual bool SetElem (const std::string& dst, const std::string& src) = 0;
    
    virtual bool MakePoint (const std::string& sym, FE_t, FE_t) = 0;
    virtual bool MakePoint_ui (const std::string& sym, size_t x, size_t y) = 0;
    virtual bool MakePoint (const std::string& sym, const char* xstr, const char* ystr, size_t base) = 0;
    virtual bool MakePoint (const std::string& sym, const std::string& xel, const std::string& yel) = 0;
    virtual bool SetPoint  (const std::string& sym, const std::string& xel, const std::string& yel) = 0;
    virtual bool CopyPoint (const std::string& lhs, const std::string& rhs) = 0; 
    // virtual bool Verify (const std::string& P, const std::string& z, const std::string& r, const std::string& s) = 0;
    
    // initialize point with elements
    // return to source
    virtual bool DelPoint (const std::string&) = 0;
    virtual bool DelElem  (const std::string&) = 0;

   
    virtual void PrintPoint (const std::string& , const std::string&, format f = format::dec) = 0;
    virtual void PrintCoeffs (const std::string& , format fmt= format::dec) = 0; 
    virtual void PrintElem (const std::string& lbl, const std::string&, format f= format::dec) = 0; 
    virtual void PrintElem (const std::string& lbl , FE_t e, format fmt ) = 0; 
    // virtual ByteArray& PointBinary ()
    // virtual ByteArray& ElemBinary ()
    
    virtual bool IsPointOnCurve (FE_t, FE_t) = 0;  
    virtual bool IsPointOnCurve (const std::string& xco, const std::string& yco) = 0;  
    virtual bool IsPointOnCurve (const std::string& P) = 0;  
   //  bool EC_IsAtInfinity (const EC_Point& P);
    // virtual bool IsPointOnCurve (const std::string& eq, const std::string& P) = 0;  
    // virtual bool ECDSA_verify (const char* sz_z, const char* sz_r, const char* sz_s) = 0; 
    //  
  protected:

    EC_context () {}

  };
  
  typedef std::shared_ptr<EC_context> ECConRef;

  ECConRef Create_EC_context (FEConPtr,
			      const char* a,
			      const char* b,
			      const char* order,
			      size_t base);

  ECConRef Create_EC_context (FEConPtr,
			      el::map& em,
			      pt::map& pm,
			      const char* a,
			      const char* b,
			      const char* order,
			      size_t base);

  
}

#endif
