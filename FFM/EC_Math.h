#ifndef EC_MATH_H
#define EC_MATH_H



#include "Common.h"
#include "Utility.h"
#include "FE_Math.h"


namespace FFM
{
  //
  //
  enum class EC_Format : int {
    DEC, HEX
  } ;



  struct EC_context : public Destructor
  {
    // s * P , 
    virtual bool Mul (const std::string& out, const std::string& s,  const std::string& P) = 0; 
    virtual bool Add (const std::string& out, const std::string& lhs, const std::string& rhs) = 0;

    virtual bool DefPoint (const std::string& sym) = 0;
    virtual bool DefCoeffs (const std::string& sym) = 0;
    virtual bool DefElem  (const std::string& sym) = 0;
    // initialize point with elements
   // return to source
    virtual bool UndefPoint (const std::string&) = 0;
    virtual bool UndefCoeffs (const std::string& ) = 0; 
    virtual bool UndefElem  (const std::string&) = 0;
   
    virtual void PrintPoint (const std::string& , const std::string&, EC_Format f = EC_Format::DEC) = 0;
    virtual void PrintCoeffs (const std::string& , const std::string&, EC_Format f= EC_Format::DEC)= 0; 
    virtual void PrintElem (const std::string& , const std::string&, EC_Format f= EC_Format::DEC)= 0; 
    
    virtual bool SetPoint (const std::string&, const char* str_x, const char* str_y, size_t base ) = 0; 
    virtual bool SetCoeffs (const std::string& , const char* strv_a, const char* strv_b, size_t base ) = 0;
    virtual bool SetElem (const std::string& , const char* strv, size_t base ) = 0;

    virtual bool SetPoint_ui (const std::string& , size_t, size_t) = 0;
    virtual bool SetCoeffs_ui (const std::string& , size_t, size_t) = 0;
    virtual bool SetElem_ui (const std::string& , size_t x) = 0;
     
    //  bool EC_IsAtInfinity (const EC_Point& P);
    virtual bool IsPointOnCurve (const std::string& eq, const std::string& P) = 0;  

    //  
    inline bool DefPoint (const std::string& sym, const char* strx, const char* stry, size_t base) { return (DefPoint(sym) ? SetPoint(sym, strx, stry, base) : false); }
    inline bool DefCoeffs (const std::string& sym, const char* stra, const char* strb, size_t base) { return (DefCoeffs(sym) ? SetCoeffs(sym, stra, strb, base) : false); } 
    inline bool DefElem  (const std::string& sym, const char* strv, size_t base) { return (DefElem(sym) ? SetElem(sym, strv, base) : false); }

    inline bool DefPoint_ui (const std::string& sym, size_t _x, size_t _y) { return (DefPoint(sym) ? SetPoint_ui(sym,_x,_y) : false); } 
    inline bool DefCoeffs_ui (const std::string& sym, size_t _a, size_t _b) { return (DefCoeffs (sym) ? SetCoeffs_ui(sym,_a,_b) : false); } 
    inline bool DefElem_ui (const std::string& sym, size_t v) { return (DefElem(sym) ? SetElem_ui(sym, v) : false); }

  };
  
  typedef std::shared_ptr<EC_context> ECConRef;

  ECConRef Create_EC_context (FEConPtr);

  
  // s * P , 
  // EC_Point& EC_Mul (EC_Point& out, FE_t s, const EC_Point& P); 
  // //  
  // EC_Point& EC_Add (EC_Point& out, const EC_Point& lhs, const EC_Point& rhs);
  // EC_Point& EC_Sub (EC_Point& out, const EC_Point& lhs, const EC_Point& rhs);
  // // initialize point with elements
  // EC_Point  EC_MakePoint (FEConPtr);
  // EC_Coeffs EC_MakeCoeffs(FEConPtr); 
  // // return to source
  // void Free (EC_Point& x);
  // void Free (EC_Coeffs& x); 



  // enum EC_FORMAT { EC_FORMAT_DEC, EC_FORMAT_HEX } ;  

  // void EC_Print (const char* lbl, const EC_Point& x, EC_FORMAT f = EC_FORMAT_DEC);
  // void EC_Print (const char* lbl, const EC_Coeffs& x , EC_FORMAT f= EC_FORMAT_DEC); 
  
  // EC_Point& EC_Set (EC_Point& out, const char* strv_x, const char* strv_y, size_t base ); 
  // EC_Coeffs& EC_Set (EC_Coeffs& out, const char* strv_a, const char* strv_b, size_t base ); 

  // EC_Point& EC_Set (EC_Point& out, size_t x, size_t y);



  //  bool EC_IsAtInfinity (const EC_Point& P);

  
  //bool EC_IsPointOnCurve ( EC_Coeffs& eq, EC_Point& p);  

}

#endif
