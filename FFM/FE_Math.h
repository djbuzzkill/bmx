#ifndef FE_MATH_H
#define FE_MATH_H


#include "Common.h"
#include "Utility.h"


namespace FFM
{
  class FE_context;

  typedef std::shared_ptr<FE_context> FEConPtr;  

  typedef int FE_t;
  //
  //
  typedef std::vector<unsigned char> ByteArray; 

  //
  // if an element is infinity we flip the sign of the element index
  inline bool FE_Infinity (FE_t x) { return (x < 0 ? true : false); }
  //inline bool FE_Infinity (FE_t x) { return FE_Infinity (x.name()); }
  //
  class FE_context : public FFM::Destructor
  {
  protected:
    FE_context () {}
  public:
  
    virtual void Print (const char* msg, FE_t x) = 0; 
    virtual char* Format (char* out, const char* fmt, FE_t x) = 0; 

    virtual bool IsValid (FE_t) = 0; 
    // get new element

    //virtual FE_var Alloc () = 0;
    virtual FE_t New () = 0; 
    inline FE_t New (const char *strv, size_t base = 0) { FE_t x = New(); if (x) { Set (x, strv, base); } return x; }
    inline FE_t New_ui (size_t v) { FE_t i = New(); if (i) { Set_ui(i, v); } return i; }
    //  virtual FE_t New (bytearr& LEraw) = 0;

    // 
    virtual void Del (FE_t) = 0;

    
    /* 0=hex, 2=bin, 10=dec,...  */ 
    virtual void Set (FE_t place, const char* strv, size_t base = 0 ) = 0; 
    virtual void Set (FE_t lval, FE_t rval) = 0;
    virtual void Set_ui (FE_t place, size_t v) = 0; 
    
    virtual int Cmp (FE_t lhs, FE_t rhs) = 0; 
    virtual int Cmp_ui (FE_t lhs, size_t rhs) = 0;
    
    
    virtual ByteArray& Raw (ByteArray& out, FE_t) = 0; 
    
    virtual void Add (FE_t out, FE_t lhs, FE_t rhs) = 0;
    virtual void Sub (FE_t out, FE_t lhs, FE_t rhs) = 0; 
    virtual void Mul (FE_t out, FE_t lhs, FE_t rhs) = 0;
    virtual void Div (FE_t out, FE_t lhs, FE_t rhs) = 0;
    virtual void Pow (FE_t out, FE_t b, FE_t exp) = 0; 
    virtual void Pow_ui (FE_t out, FE_t b, size_t exp) = 0; 
    virtual void Inv (FE_t out, FE_t x) = 0; 
  };
  

// 

  FEConPtr Create_FE_context (const char* strv, size_t base = 0 /* 0=hex, 2=bin, 10=dec */ );
  
  //
  // 
  struct ScopeDeleter : public std::vector<FE_t>
  {
    ScopeDeleter (FEConPtr f) : F(f), std::vector<FE_t>() {}
    FE_t operator() (FE_t e) { push_back (e); return e; }
    ~ScopeDeleter () {while (size()) { F->Del(back()); pop_back (); }}
    FEConPtr F;
  };



} // FM

#endif
