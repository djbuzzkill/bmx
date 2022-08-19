
#include "script.h"

namespace evalution {

  int encode_num (size_t num) {
  }
  

  int decode_num () {

// def decode_num(element):
//     if element == b'':
//         return 0
//     # reverse for big endian
//     big_endian = element[::-1]
//     # top bit being 1 means it's negative
//     if big_endian[0] & 0x80:
//         negative = True
//         result = big_endian[0] & 0x7f
//     else:
//         negative = False
//         result = big_endian[0]
//     for c in big_endian[1:]:
//         result <<= 8
//         result += c
//     if negative:
//         return -result
//     else:
//         return result



  }
  
}



namespace curv {


  size_t ReadSript (CommandList& out, af::ReadStreamRef rs) {

    out.clear (); 
    
    size_t readlen = 0; 

    size_t scriptlen = 0;
    readlen += read_varint (scriptlen, rs)
    
    size_t accum = 0; 
    while (accum < scriptlen) {

      unsigned char leader = 0; 
      accum = rs->Read (&leader, 1); 

      if (leader > 0 && leader < 76) {
	out.push_back (      ); 

      }
      else if (leader == 76) {

      }
      else if (leader == 77) {
      }
      else {
	
	
      }
      
      
    }
    readlen += accum; 
    
    if (accum != scriptlen) {
      // error
      printf ("%s(accum != scriptlen)\n", __FUNCTION__); 
    }
    
    
    return readlen; 
  }


  //
  //
    typedef std::stack<af::bytearray> ScriptStack;

  //
  int EvalScript (const CommandList& commands) {

    ScriptStack sstack;
    ScriptStack altstack; 

    //
    //
    for (const auto& cmd : commands) {

      // figure out what it is
      if (cmd.size () == 1){
      }
      else {

	
      }
	
      

      
    }
    

    return 0; 
  }

}
