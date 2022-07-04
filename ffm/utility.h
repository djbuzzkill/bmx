#ifndef EC_UTILITY_H
#define EC_UTILITY_H

#include "Common.h"
#include "Utility.h"


#include <array>
#include <vector>
#include <string>
#include <algorithm>




#define POUT(s) {printf("%s|ln:%i\n", (s), __LINE__);}
#define PR(s) printf("%s", (s));  


namespace FFM
{

  typedef std::map<std::string, std::string> propmap; 
  
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


} // FM
#endif
