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
  

  unsigned char uc_hex (const std::string& chars) {
   
    std::map<char, unsigned char> hexmap = {
      {'0', 0 },
      {'1', 1 }, 
      {'2', 2 },
      {'3', 3 }, 
      {'4', 4 },
      {'5', 5 }, 
      {'6', 6 },
      {'7', 7 }, 
      {'8', 8 },
      {'9', 9 }, 
      {'a', 10},
      {'b', 11}, 
      {'c', 12},
      {'d', 13}, 
      {'e', 14}, 
      {'f', 15}, 
      {'A', 10},
      {'B', 11}, 
      {'C', 12},
      {'D', 13}, 
      {'E', 14}, 
      {'F', 15}, 
    };  

    const unsigned char hi = 0;
    const unsigned char lo = 1;
 
    if (chars.size () != 2) {
      return 0; 
    }

    if (0 == hexmap.count (chars[hi])) {
      return 0;
    }
    
    if (0 == hexmap.count (chars[lo])) {
      return 0;
    }
    
    unsigned char out =  hexmap[chars[hi]] << 4; 
    out += hexmap[chars[lo]];
    return out; 
    
  }




}
