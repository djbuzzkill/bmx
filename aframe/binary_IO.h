#ifndef AF_BINARY_IO_H
#define AF_BINARY_IO_H



#include "common.h"
#include "shared_types.h"

namespace af {

  //
  // 
  struct byte_stream {
    
    enum struct SeekMode : int  {
      Abs,  
      Rel,
      End, 

    }; 

    virtual void   SetPos (ptrdiff_t offs, SeekMode mode) = 0; 
    virtual size_t GetPos () = 0;

  protected:

    byte_stream () {}
  }; 


  //
  //
  struct reader {

    virtual size_t Read (void* out, size_t len) = 0;


    reader () {}
  };

  //
  // 
  struct writer {
  
    virtual size_t Write (const void* src, size_t len) = 0; 
    
  protected:
    writer () {} 
  };
  



  struct read_stream : public byte_stream, public reader, public destructor {

  protected:
    read_stream () : byte_stream(), reader () {}
  }; 

  struct write_stream : public byte_stream, public writer, public destructor {
  protected:
    write_stream () : byte_stream(), writer () {}
  };
  
  typedef std::shared_ptr<read_stream>  ReadStreamRef;
  typedef std::shared_ptr<write_stream> WriteStreamRef;
  
  ReadStreamRef  CreateReadMemStream  (const void* mem, size_t len);
  WriteStreamRef CreateWriteMemStream (void* mem, size_t len);





  
  
  //
  //
  inline size_t write_byte32 (WriteStreamRef ws, const byte32& src) {

    return ws->Write ( std::data(src), sizeof(byte32)); 

  }

  //
  //
  inline size_t read_byte32 (byte32& out, ReadStreamRef rs) {

    return rs->Read (std::data (out), sizeof(byte32)); 
  }
  
  
  
} // namespace af


#endif
