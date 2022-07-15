#include "binary_IO.h"


namespace af {
  //
  // READ STREAM
  struct read_stream_impl : public read_stream {
    //
    read_stream_impl (const void* src, size_t len)
      : read_stream ()
      , offs(0)
      , max_len (len)
      , mem (reinterpret_cast<const  unsigned char*>(src)) {

    }
    
    void SetPos (size_t offs, SeekMode mode) {

    }

    size_t GetPos () {
      return offs;

    }
    
    size_t Read (void* out, size_t len) {
    

      return 0; 
    }

  protected:
    size_t               max_len; 
    size_t               offs; 
    const unsigned char* mem;
    
  };


  //
  //
  // WRITE STREAM
  struct write_stream_impl : public write_stream {

    write_stream_impl (void* src, size_t len)
      : write_stream ()
      , offs(0)
      , max_len(len)
      , mem(reinterpret_cast<unsigned char*>(src)) {


    }
    
    
    void SetPos (size_t pos, SeekMode mode) {
      
    }
    
   
    size_t GetPos () {
      return offs; 
      
    }
    
    size_t Write (void* src, size_t len) {
      return 0; 

    }
    size_t         max_len ; 
    size_t         offs; 
    unsigned char* mem; 
    
  };

  //
  //
  //
  ReadStreamRef CreateReadMemStream  (const void* mem, size_t len) {

    ReadStreamRef ref (new read_stream_impl (mem, len));
    return ref; 

  }

  //
  //
  //
  WriteStreamRef CreateWriteMemStream (void* mem, size_t len) {
    WriteStreamRef ref (new write_stream_impl(mem, len)); 
    return ref;
    
  }
} // af 
