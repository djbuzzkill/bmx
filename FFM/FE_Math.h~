#ifndef FE_MATH_H
#define FE_MATH_H


#include "Common.h"
#include "Utility.h"




typedef size_t FE_t;

template<unsigned N>
using FE_bytes = std::array<unsigned char, N>; 




class FE_context : public Destructor
{
 protected:
  FE_context () {}
 public:
  
  virtual void Print (const char* msg, FE_t x) = 0; 
 
  // get new element
  virtual FE_t New (size_t v = 0) = 0; 
  virtual FE_t New (const char *strv, size_t base = 0) = 0; 
  //  virtual FE_t New (bytearr& LEraw) = 0;

 // give it back 
  virtual void   Del  (FE_t id) = 0; 
  /* 0=hex, 2=bin, 10=dec,...  */ 
  virtual FE_t Set (FE_t place, const char* strv, size_t base = 0 ) = 0; 
  virtual FE_t Set (FE_t place, size_t v) = 0; 
  //virtual FE_t Set (FE_t place, bytearr& LEraw) = 0;

  
  virtual unsigned char* Raw (unsigned char* BE_bytes_out, FE_t) = 0; 

  virtual FE_t Add (FE_t out, FE_t lhs, FE_t rhs) = 0;
  virtual FE_t Sub (FE_t out, FE_t lhs, FE_t rhs) = 0; 
  virtual FE_t Mul (FE_t out, FE_t lhs, FE_t rhs) = 0;
  virtual FE_t Div (FE_t out, FE_t lhs, FE_t rhs) = 0;

};


typedef std::shared_ptr<FE_context> FEContextPtr;  
// 

FEContextPtr Create_FE_context (size_t field_bitcount );
//



#endif
