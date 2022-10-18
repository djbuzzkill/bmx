

#include "common.h"
#include "utility.h"
#include "aframe/binary_IO.h"

//
namespace bmx {


  const uint32 kBIP0037_CONSTANT = 0xfba4c795; 
  
  namespace BloomFilter {

    struct Struc {
      uint8     size;
      bytearray bitfield;
      uint32    function_count; 
      uint32    tweak; 
    };

    Struc& Init (Struc& bf, uint8 size, uint8 numfns, uint32 tweak);
    Struc& Add  (Struc& bf, const bytearray& item);

    uint64 Write (af::WriteStreamRef ws, const Struc& bf, byte flag = byte{1}); 
    uint64 Read  (Struc& bf, byte& flag, af::ReadStreamRef rs);

  } // BloomFilter 

  typedef BloomFilter::Struc bloomfilter; 
} // knamespace bmx
