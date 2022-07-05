#include "utility.h"

namespace af
{



 std::string hex_uc (unsigned char c) {

    const std::string hexdigits = "0123456789abcdef";
      std::string ret = "";
      
      unsigned char lo = 0x0f & c;
      unsigned char hi = c >> 4;
      
      //printf ("%s[hi:%i|lo:%i|c:%i]\n", __FUNCTION__, hi, lo, c);
      
      ret += hexdigits[hi] ;
      ret += hexdigits[lo] ; 
      
      return ret; 
    }
  






}
