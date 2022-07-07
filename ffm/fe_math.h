#ifndef FFM_FE_MATH_H
#define FFM_FE_MATH_H


#include "common.h"
#include "utility.h"
#include "aframe/utility.h"

namespace ffm
{
  struct FE_context;

  typedef std::shared_ptr<FE_context> FEConRef;  
  typedef int FE_t;
  typedef FEConRef FEConPtr;  


  typedef af::bytearray bytearray; 

  //
  //

  //
  // if an element is infinity we flip the sign of the element index
  inline bool FE_Infinity (FE_t x) { return (x < 0 ? true : false); }

  //
  struct FE_context : public ffm::Destructor {
    // scalar math done here 
 public:
  
    virtual void Print (const char* msg, FE_t x) = 0; 
    virtual char* Format (char* out, const char* fmt, FE_t x) = 0; 

    virtual bool IsValid (FE_t) = 0; 
    // get new element

    virtual FE_t p () = 0; 
    // 
    virtual FE_t New () = 0; 
    inline FE_t New (const char *strv, size_t base = 0) {FE_t x = New(); if (x) { Set (x, strv, base); return x; } return 0; }
    inline FE_t New_ui (size_t v) { FE_t ui = New(); if (ui) { Set_ui(ui, v); } return ui; }
    inline FE_t New_si (int v) { FE_t si = New (); if (si) { Set_si(si, v); } return si; } 
    inline FE_t New_bin (const unsigned char* bin, bool be, size_t len) { FE_t x = New (); if(x) { Set_bin (x, bin, be, len); return x;} return 0; }

    //  virtual FE_t New (bytearr& LEraw) = 0;

    // 
    virtual void Del (FE_t) = 0;

    
    /* 0=hex, 2=bin, 10=dec,...  */ 
    virtual void Set (FE_t place, const char* strv, size_t base = 0 ) = 0; 
    virtual void Set (FE_t lval, FE_t rval) = 0;
    virtual void Set_bin (FE_t lval, const unsigned char* bin, unsigned len, bool isLE) = 0;
    
    virtual void Set_ui (FE_t place, size_t ui) = 0;
    virtual void Set_si (FE_t place, long int si)  = 0; 
    
    virtual int Cmp (FE_t lhs, FE_t rhs) = 0; 
    virtual int Cmp_ui (FE_t lhs, size_t rhs) = 0;
    
    virtual void LogiAnd (FE_t O, FE_t lhs, FE_t rhs) = 0;
    virtual void LogiShiftR (FE_t x, size_t shift) = 0;
    virtual void LogiShiftL (FE_t x, size_t shift) = 0;
    virtual bool LogiBit (FE_t v, size_t pos) = 0;
    virtual bool TestBit (FE_t v, size_t pos) = 0;
    
    virtual bytearray& Raw (bytearray& out, FE_t, bool LE_else_BE) = 0; 
    
    virtual void Add (FE_t out, FE_t lhs, FE_t rhs) = 0;
    virtual void Sub (FE_t out, FE_t lhs, FE_t rhs) = 0; 
    virtual void Sub_ui (FE_t out, FE_t lhs, size_t) = 0; 
    virtual void Mul (FE_t out, FE_t lhs, FE_t rhs) = 0;
    virtual void MulM (FE_t out, FE_t lhs, FE_t rhs, FE_t mod) = 0;
    virtual void Div (FE_t out, FE_t lhs, FE_t rhs) = 0;
    virtual void Pow (FE_t out, FE_t b, FE_t exp) = 0; 
    virtual void Pow_ui (FE_t out, FE_t base, size_t exp) = 0; 
    virtual void Pow_si (FE_t out, FE_t base, long int exp) = 0; 

    virtual void PowM (FE_t out, FE_t base, FE_t exp, FE_t mod) = 0;
    virtual bool Rand (FE_t out, FE_t f) = 0; 

  protected:

    FE_context () {}
  };
  

// 

  FEConRef Create_FE_context (const char* strv, size_t base = 0 /* 0=strdef'd, 2=bin, 10=dec, 16=hex */ );
  
  //
  // 
   struct ScopeDeleter : public std::vector<FE_t>   {
     
     explicit ScopeDeleter (FEConRef f) : naked(false), F(f), ptr(nullptr), std::vector<FE_t>() {}
     explicit ScopeDeleter (FE_context* f) : naked(true), F(nullptr), ptr(f), std::vector<FE_t>() {}


     FE_t operator() (FE_t e) { push_back (e); return e; }

     ~ScopeDeleter () {
       if (naked)
	 while (size()) {ptr->Del(back()); pop_back ();}
       else
	 while (size()) {F->Del(back()); pop_back();}
     }

     
     bool naked;  
     FEConPtr F;
     FE_context* ptr; 
    //~ScopeDeleter () {while (size()) { printf("deleting[%i]\n", back()); F->Del(back()); pop_back (); }}
  };

} // FM

#endif
