
#ifndef BMX_UTILITY_H
#define BMX_UTILITY_H

#include "common.h"

#include "aframe/af.h"


namespace bmx {

  namespace util {

    // 
    size_t read_varint (size_t& out, af::ReadStreamRef rs, const char* trace = 0); 


  
    //
    size_t write_varint (af::WriteStreamRef ws, size_t v) ;
  }

}


#endif
