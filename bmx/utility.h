
#ifndef BMX_UTILITY_H
#define BMX_UTILITY_H

#include "common.h"
#include "script_types.h"
#include "aframe/af.h"


namespace bmx {


  //
  //
  inline command_list& append (command_list& out, const command_list& l) {

    std::copy (l.begin(), l.end (), std::back_inserter (out));
    return out; 

  }


  
  namespace util {

    // 
    size_t read_varint (size_t& out, af::ReadStreamRef rs, const char* trace = 0); 


  
    //
    size_t write_varint (af::WriteStreamRef ws, size_t v) ;
  }

}


#endif
