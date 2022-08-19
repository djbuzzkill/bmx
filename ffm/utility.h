#ifndef FFM_UTILITY_H
#define FFM_UTILITY_H

#include "common.h"
#include <aframe/shared_types.h>

#define POUT(s) {printf("%s|ln:%i\n", (s), __LINE__);}
#define PR(s) printf("%s", (s));  


namespace ffm
{
  // 
  typedef af::fixnum32 fixnum32;  

  //
  // copy a binary number to out , fail if size(bytes) > 32
  template<typename SeqTy> 
  inline SeqTy& copy_BE (SeqTy& out, af::bytearray &bytes) {

    if (bytes.size() > out.max_size())
      return out;

    out.fill(0);
    std::copy (bytes.begin(), bytes.end(), out.end () - bytes.size());

    // printf ("out.max_size:%zu\n", out.max_size());
    // printf ("bytes.size:%zu\n", bytes.size());
    return out; 
  }
 
  // 
  template<typename Ty>
  inline Ty* swap_endian (Ty* x) {
    unsigned char* p = reinterpret_cast<unsigned char*> (x); 

    for (int i = 0; i < sizeof(Ty); ++i)
      printf ("b4[%i]:0x%x\n",i, p[i] ); 

    std::reverse (p, p + sizeof (Ty));

    for (int i = 0; i < sizeof(Ty); ++i)
      printf ("4f[%i]:0x%x\n",i, p[i] ); 

    return x;
  }  
  
// 
  template<typename Ty>
  inline Ty* swap_endian_array (Ty* p, size_t num_el) {
    for (int i = 0; i < num_el; ++i)
      swap_endian (p+i);
    return p;
  }
  
  
  //
  //
  struct Destructor {
  protected:
    Destructor () {}
    
  public:
    virtual ~Destructor () {}
  }; 



} // FM
#endif
