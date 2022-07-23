#ifndef FFM_UTILITY_H
#define FFM_UTILITY_H

#include "common.h"
#include <aframe/utility.h>

#define POUT(s) {printf("%s|ln:%i\n", (s), __LINE__);}
#define PR(s) printf("%s", (s));  


namespace ffm
{
  // 
  typedef af::byte32 fixnum32;  

  //
  // copy a binary number to out , fail if size(bytes) > 32
  inline void copy_BE (fixnum32& out, af::bytearray &bytes) {
    out.fill(0);
    if (bytes.size() > out.max_size())
      return;
    std::copy (bytes.begin(), bytes.end(), out.end () - bytes.size());
    // printf ("out.max_size:%zu\n", out.max_size());
    // printf ("bytes.size:%zu\n", bytes.size()); 
  }
 
  // 
  template<typename Ty>
  inline Ty* swap_endian (Ty* x) {
    unsigned char* p = reinterpret_cast<unsigned char*> (x); 
    for (int i = 0; i < sizeof(Ty); ++i)
      printf ("c[%i]:0x%x\n",i, p[i] ); 
    std::reverse (p, p + sizeof (Ty));
    for (int i = 0; i < sizeof(Ty); ++i)
      printf ("c[%i]:0x%x\n",i, p[i] ); 
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
