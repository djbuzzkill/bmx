#ifndef EC_MATH_H
#define EC_MATH_H



#include "Common.h"
#include "Utility.h"
#include "FE_Math.h"


namespace FFM
{
  //
  //
  enum class format : int {
    dec, hex } ; 

  //enum class format : int {
  //  DEC, HEX } ; 


  namespace el {

    typedef std::map<std::string, FE_t> map; 
  }
  //
  //
  namespace pt
  {
    struct struc : public std::tuple<FE_t, FE_t> {
 
    };

    typedef std::map<std::string, struc> map; 

    inline FE_t& x(struc& t) { return std::get<0> (t); }
    inline FE_t& y(struc& t) { return std::get<1> (t); } 
    inline const FE_t& x(const struc& t) { return std::get<0> (t); }
    inline const FE_t& y(const struc& t) { return std::get<1> (t); }

   

  }
  
 
  class EC_context : public FFM::Destructor 
  {
  public:
    // virtual bool Add (const std::string& O, const std::string& lhs, const std::string& rhs) = 0;
    // O <= A+B 
    virtual bool AddPoint (const std::string& out, const std::string& lhs, const std::string& rhs) = 0;
    
    // s*P => R
    virtual bool Mul_scalar_ui (const std::string& O, size_t s, const std::string& P) = 0;
    virtual bool Mul_scalar (const std::string& O, const std::string& s, const std::string& P) = 0;  

  
    virtual bool DefPoint (const std::string& sym, const char* strx, const char* stry, size_t base) = 0;
    // virtual bool CopyPoint (const std::string& sym, const std::string& P) = 0; 
    virtual bool DefPoint_ui (const std::string&, size_t, size_t) = 0;
   
    virtual bool DefElem  (const std::string& nsym, const std::string& sym2) = 0;
    virtual bool DefElem  (const std::string& sym, const char* strv, size_t base) = 0;
    virtual bool DefElem_ui (const std::string& , size_t x) = 0;

    virtual FE_t El (const std::string& sym) = 0;  
    // virtual bool Verify (const std::string& P, const std::string& z, const std::string& r, const std::string& s) = 0;
    
    // initialize point with elements
    // return to source
    virtual bool UndefPoint (const std::string&) = 0;
    virtual bool UndefElem  (const std::string&) = 0;
   
    virtual void PrintPoint (const std::string& , const std::string&, format f = format::dec) = 0;
    virtual void PrintCoeffs (const std::string& , format fmt= format::dec) = 0; 
    virtual void PrintElem (const std::string& , const std::string&, format f= format::dec) = 0; 
    virtual void PrintElem (const std::string& lbl , FE_t e, format fmt ) = 0; 
    // virtual ByteArray& PointBinary ()
    // virtual ByteArray& ElemBinary ()
    
     
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