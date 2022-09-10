
#ifndef FFM_UTILITY_H
#define FFM_UTILITY_H


#include "common.h"
#include <aframe/shared_types.h>

namespace ffm
{
  //
  typedef af::int8 int8 ; 
  typedef af::int16 int16 ; 
  typedef af::int32 int32 ; 
  typedef af::int64 int64 ;

  typedef af::uint8 uint8 ; 
  typedef af::uint16 uint16 ; 
  typedef af::uint32 uint32 ; 
  typedef af::uint64 uint64 ; 

  typedef af::fixnum32 fixnum32;  
  typedef af::byte byte; 
  
  //
  // copy a binary number to out , fail if size(bytes) > 32
  template<typename SeqTy> 
  inline SeqTy& copy_BE (SeqTy& out, af::bytearray &bytes) {

    if (bytes.size() <  out.max_size()) {
      printf ("<%s> bytes[%zu] < outseq[%zu]\n" , __FUNCTION__, bytes.size(), out.max_size());
    }

    if (bytes.size() > out.max_size())
      return out;

    out.fill(byte{0});
    std::copy (bytes.begin(), bytes.end(), out.end () - bytes.size());

    // printf ("out.max_size:%zu\n", out.max_size());
    // printf ("bytes.size:%zu\n", bytes.size());
    return out; 
  }


  template<typename OutSeq, typename InSeq>
  inline OutSeq& copy (OutSeq& out, InSeq& obj) {
    out.resize (obj.size());
    std::copy (obj.begin(), obj.end(), obj.begin());
    return out;
  }
    
  // 
  template<typename Ty>
  inline Ty* swap_endian (Ty* x) {
    unsigned char* p = reinterpret_cast<unsigned char*> (x); 

    for (int i = 0; i < sizeof(Ty); ++i)
      printf ("b4[%i]:0x%x\n",i, p[i] ); 

    std::reverse (p, p + sizeof (Ty));

    for (int i = 0; i < sizeof(Ty); ++i)
      printf ("4f[%i]:0x%x\n",i, p[i] ); 

    return x;
  }  
  
// 
  template<typename Ty>
  inline Ty* swap_endian_array (Ty* p, size_t num_el) {
    for (int i = 0; i < num_el; ++i)
      swap_endian (p+i);
    return p;
  }
  
  
  //
  //
  struct Destructor {
  protected:
    Destructor () {}
    
  public:
    virtual ~Destructor () {}
  }; 



} // FM
#endif
