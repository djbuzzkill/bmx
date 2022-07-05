#ifndef FFM_UTILITY_H
#define FFM_UTILITY_H

#include "common.h"
#include "utility.h"

// 
template<typename Ty>
inline Ty* swap_endian (Ty* x)
{
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
inline Ty* swap_endian_array (Ty* p, size_t num_el)
{

  for (int i = 0; i < num_el; ++i)
    swap_endian (p+i);
  return p;
}
	

//
//
struct Destructor
{
protected:
  Destructor () {}

public:
  virtual ~Destructor () {}
}; 

#endif
