#ifndef FFM_FE_MATH_H
#define FFM_FE_MATH_H


#include "common.h"
#include "utility.h"
#include "aframe/utility.h"

namespace ffm
{

  

  typedef int FE_t;
  const FE_t fe_null  = 0;

  struct FE_context;
  typedef std::shared_ptr<FE_context> FEConRef;  
  //typedef FEConRef FEConPtr;  
  typedef af::bytearray bytearray; 

  //
  //

  //
  // if an element is infinity we flip the sign of the element index
  inline bool FE_INF (FE_t x) { return (x < 0 ? true : false); }

  //
  struct FE_context : public ffm::Destructor {
    // scalar math done here 
 public:
    
    virtual void
    Print  (const char* msg, FE_t x) = 0; 


    virtual char*         Format     (char* out, const char* fmt, FE_t x) = 0; 
    inline  char*         fmt        (char* out, const char* fmt, FE_t x) { return Format (out, fmt, x); } 

    virtual bool          IsValid    (FE_t) = 0; 
    // get new element

    virtual FE_t          p          () = 0; 
    virtual FE_t          New        () = 0; 
    inline  FE_t          New        (const char *strv, size_t base = 0) {FE_t x = New(); if (x) { Set (x, strv, base); return x; } return 0; }
    inline  FE_t          New_ui     (size_t v) { FE_t ui = New(); if (ui) { Set_ui(ui, v); } return ui; }
    inline  FE_t          New_si     (int v) { FE_t si = New (); if (si) { Set_si(si, v); } return si; } 
    inline  FE_t          New_bin    (const void* bin, size_t len, bool LE) { FE_t x = New (); if(x) { Set_bin (x, bin, len, LE); return x;} return 0; }
    //  virtual FE_t New (bytearr& LEraw) = 0;
    virtual void         Del         (FE_t) = 0;
    /* 0=pref, 2=bin, 10=dec,...  */ 
    virtual void         Set         (FE_t place, const char* strv, size_t base = 0) = 0; 
    virtual void         Set         (FE_t lval, FE_t rval) = 0;
    virtual void         Set_bin     (FE_t lval, const void* bin, unsigned len, bool isLE) = 0;
    		         	     
    virtual void         Set_ui      (FE_t place, size_t ui) = 0;
    virtual void         Set_si      (FE_t place, long int si)  = 0; 
    		         	     
    virtual int          Cmp         (FE_t lhs, FE_t rhs) = 0; 
    virtual int          Cmp_ui      (FE_t lhs, size_t rhs) = 0;
    				     
    virtual void         LogiAnd     (FE_t O, FE_t lhs, FE_t rhs) = 0;
    virtual void         LogiShiftR  (FE_t x, size_t shift) = 0;
    virtual void         LogiShiftL  (FE_t x, size_t shift) = 0;
    virtual bool         LogiBit     (FE_t v, size_t pos) = 0;
    virtual bool         TestBit     (FE_t v, size_t pos) = 0;
    				     
    virtual bytearray&   Raw         (bytearray& out, int& sgn, FE_t, bool LE_else_BE) = 0; 
    inline  bytearray&   Raw         (bytearray& out, FE_t n, bool LE_else_BE) { int dontcare = 0; return Raw (out, dontcare, n, LE_else_BE); }
    				     
    virtual void         Add         (FE_t out, FE_t lhs, FE_t rhs) = 0;
    virtual void         Sub         (FE_t out, FE_t lhs, FE_t rhs) = 0; 
    virtual void         Sub_ui      (FE_t out, FE_t lhs, size_t) = 0; 
    virtual void         Mul         (FE_t out, FE_t lhs, FE_t rhs) = 0;
    virtual void         MulM        (FE_t out, FE_t lhs, FE_t rhs, FE_t mod) = 0;
    virtual void         Div         (FE_t out, FE_t lhs, FE_t rhs) = 0;
    virtual void         Div_ui      (FE_t, FE_t, size_t) = 0; 
				     

    virtual void         Pow         (FE_t out, FE_t b, FE_t exp) = 0; 
    virtual void         Pow_ui      (FE_t out, FE_t base, size_t exp) = 0; 
    virtual void         Pow_si      (FE_t out, FE_t base, long int exp) = 0; 
    virtual void         PowM        (FE_t out, FE_t base, FE_t exp, FE_t mod) = 0;
    virtual bool         Rand        (FE_t out, FE_t f) = 0; 

    virtual void         DivMod      (FE_t outq, FE_t outr, FE_t num, FE_t den) = 0; 
    virtual size_t       DivMod_ui   (FE_t q, FE_t r, FE_t n, size_t d) = 0; 

    virtual void         FDiv        (FE_t out, FE_t num, FE_t den) = 0; 
    virtual void         FDiv_ui     (FE_t out, FE_t num, size_t den) = 0; 
				      
    virtual void         Sqrt        (FE_t out, FE_t n) = 0; 


    // Simple versions
    virtual void         SAdd        (FE_t out, FE_t lhs, FE_t rhs) = 0;
    virtual void         SMul        (FE_t out, FE_t lhs, FE_t rhs) = 0; 
    virtual void         SSub        (FE_t out, FE_t lhs, FE_t rhs) = 0; 
    virtual void         SDiv        (FE_t out, FE_t lhs, FE_t rhs) = 0; 
    
    virtual void         SAdd_ui     (FE_t out, FE_t lhs, unsigned long int rhs) = 0;
    virtual void         SMul_ui     (FE_t out, FE_t lhs, unsigned long int rhs) = 0; 
    virtual void         SSub_ui     (FE_t out, FE_t lhs, unsigned long int rhs) = 0; 
    			  
    virtual void         SNeg        (FE_t out, FE_t num) = 0; 
    virtual void         SAbs        (FE_t out, FE_t num) = 0; 
    virtual void         SPow_ui     (FE_t out, FE_t b, uint64 exp) = 0;




    
    
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
    //ScopeDeleter () {}

    FE_t operator() (FE_t e) { push_back (e); return e; }
    
    ~ScopeDeleter () {
      //auto delr = std::mem_fn(&FE_context::Del);
      //while (size()) {delr(back()); pop_back ();} 
      if (naked) while (size()) {ptr->Del(back()); pop_back ();} 
      else       while (size()) {F->Del(back()); pop_back();} 
    }
    
    bool naked;  
    FEConRef F;
    FE_context* ptr; 
    //~ScopeDeleter () {while (size()) { printf("deleting[%i]\n", back()); F->Del(back()); pop_back (); }}
  };


  // ----------------------------------------------------------------------------------------
  //
  // ----------------------------------------------------------------------------------------
  struct Formatter {

    Formatter (FEConRef F, uint32 buffsize = FFM_DEFAULT_TEXT_BUFFER_SIZE) :f (F), buf(buffsize, ' ') {
    }

    const char* fmt_hex (FE_t x) { return f->fmt (&buf[0], "%Zx", x) ; }
    const char* fmt_dec (FE_t x) { return f->fmt (&buf[0], "%Zd", x) ; }

    inline const char* hx (FE_t x) { return fmt_hex(x) ; }
    inline const char* dc (FE_t x) { return fmt_dec(x) ; }

    std::vector<char> buf; 
    FEConRef f;

  protected:

    Formatter () : f(nullptr), buf (0, 0) {
      }
  }; 

  
  
} // FM

#endif
