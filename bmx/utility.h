
#ifndef BMX_UTILITY_H
#define BMX_UTILITY_H

#include "common.h"
#include "script_types.h"
//#include "aframe/af.h"
#include "aframe/shared_types.h"


namespace bmx {

  typedef af::uint8   uint8;
  typedef af::uint16  uint16; 
  typedef af::uint32  uint32; 
  typedef af::uint64  uint64; 
  
  typedef af::int8    int8;
  typedef af::int16   int16; 
  typedef af::int32   int32; 
  typedef af::int64   int64;
  typedef af::byte    byte;


  // ---------------------------------------------------
  //
  // ---------------------------------------------------
  typedef af::bytearray bytearray; 
  typedef af::byte32    byte32   ; // 
  typedef af::byte20    byte20   ; //  
  typedef af::bigint32  bigint32 ;
  typedef af::fixnum32  fixnum32 ;


  
  
  constexpr std::uint32_t SIGHASH_ALL    = 1; 
  constexpr std::uint32_t SIGHASH_NONE   = 2; 
  constexpr std::uint32_t SIGHASH_SINGLE = 3; 
  //
  //
  inline command_list& append (command_list& out, const command_list& l) {

    std::copy (l.begin(), l.end (), std::back_inserter (out));
    return out; 

  }


  
  namespace util {

    // 
    size_t         read_varint  (size_t& out, af::ReadStreamRef rs, const char* trace = 0); 
    size_t         write_varint (af::WriteStreamRef ws, size_t v) ;

    af::bytearray& encode_num   (af::bytearray& enc_out, ffm::FE_t num, ffm::FEConRef F); 
    ffm::FE_t      decode_num   (ffm::FE_t num, ffm::FEConRef F, const af::bytearray& n_enc); 

  }

}


#endif
