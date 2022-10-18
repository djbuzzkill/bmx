
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

  typedef std::vector<int32>    nodearray;
  typedef std::vector<digest32> hasharray;
  
  constexpr std::uint32_t SIGHASH_ALL    = 1; 
  constexpr std::uint32_t SIGHASH_NONE   = 2; 
  constexpr std::uint32_t SIGHASH_SINGLE = 3; 
  //
  //
  inline command_list& append (command_list& out, const command_list& l) {

    std::copy (l.begin(), l.end (), std::back_inserter (out));
    return out; 

  }

  
  // script_ut
  namespace util {

    //
    uint64         SizeOf_varint (uint64  x); 
    size_t         read_varint  (size_t& out, af::ReadStreamRef rs, const char* trace = 0); 
    size_t         write_varint (af::WriteStreamRef ws, size_t v) ;

    af::bytearray& encode_num   (af::bytearray& enc_out, ffm::FE_t num, ffm::FEConRef F); 
    ffm::FE_t      decode_num   (ffm::FE_t num, ffm::FEConRef F, const af::bytearray& n_enc); 

  }

    
  // expand into flags 0b00000101 -> [0, 0, 0, 0, 0, 0, 1, 0, 1]  
  bytearray& bytes_to_bitfield (bytearray &oflags, const bytearray &bitfield);

  // pack into bytes into bits:  [0, 0, 0, 0, 0, 0, 1, 0, 1] -> 0b00000101 
  bytearray& bitfield_to_bytes (bytearray &bitfield, const bytearray &flags);



  // flags_from_bits = bytes_to_bitfield
  inline bytearray& flags_from_bits (bytearray &oflags, const bytearray &bitfield) {
    return bytes_to_bitfield(oflags, bitfield);
  }
  // bits_from_flags = bitfield_to_bytes 
  inline bytearray& bits_from_flags (bytearray &bitfield, const bytearray &flags) {
    return bitfield_to_bytes (bitfield, flags); 
  }

  
}


#endif
