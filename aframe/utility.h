
#ifndef AF_UTILITY_INCLUDE
#define AF_UTILITY_INCLUDE


#include "common.h"




namespace af
{
  
  typedef std::array<unsigned char, 32> byte32; 

  //
  inline void checkres (const std::string& label, bool cond) {

    printf ("%s:%s\n", label.c_str(), (cond ? "true" : "false")); 
      
  }
  
  // uc -> hex
  std::string hex_uc (unsigned char c);

  // hex -> uc
  unsigned char uc_hex (const std::string& chars); 

}


#endif
