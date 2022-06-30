#ifndef EC_MATH_H
#define EC_MATH_H



#include "Common.h"
#include "Utility.h"
#include "FE_Math.h"


namespace FFM
{
  //
  //
  enum class Format : int {
    DEC, HEX } ; 

  //
  //

  class EC_context : public FFM::Destructor 
  {
  public:


    //virtual ~EC_context (); 
    
    // A+B => O

    virtual bool Add (const std::string& O, const std::string& lhs, const std::string& rhs) = 0;

    // s*P => R
    //virtual bool Mul_scalar (const std::string& O, const std::string& s,  const std::string& P) = 0; 
    virtual bool Mul_scalar_ui (const std::string& O, size_t s, const std::string& P) = 0;
    virtual bool Mul_scalar (const std::string& O, const std::string& s, const std::string& P) = 0;  
    /* virtual bool Coeffs(const char* a, const char* b, size_t base) = 0; */
    /* virtual bool Coeffs_ui (size_t, size_t) = 0; */

  
    virtual bool DefPoint (const std::string& sym, const char* strx, const char* stry, size_t base) = 0;
    virtual bool CopyPoint (const std::string& sym, const std::string& P) = 0; 
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
   
    virtual void PrintPoint (const std::string& , const std::string&, Format f = Format::DEC) = 0;
    virtual void PrintCoeffs (const std::string& , Format fmt= Format::DEC) = 0; 
    virtual void PrintElem (const std::string& , const std::string&, Format f= Format::DEC) = 0; 
    
    // virtual ByteArray& PointBinary ()
    // virtual ByteArray& ElemBinary ()
    
     
    //  bool EC_IsAtInfinity (const EC_Point& P);
    // virtual bool IsPointOnCurve (const std::string& eq, const std::string& P) = 0;  

    //  
  protected:

    EC_context () {}

  };
  
  typedef std::shared_ptr<EC_context> ECConRef;

  ECConRef Create_EC_context (FEConPtr, const char* a, const char* b, size_t base);

  
}

#endif
