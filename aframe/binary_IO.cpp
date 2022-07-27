#include "binary_IO.h"
#include "utility.h"

namespace af {
  //
  // READ STREAM
  struct read_stream_impl : public read_stream {
    //
    read_stream_impl (const void* src, size_t len)
      : read_stream ()
      , max_size (len)
      , offs(0)
      , mem (reinterpret_cast<const  unsigned char*>(src)) {

    }
    
    void SetPos (ptrdiff_t offset, SeekMode mode) {

      switch (mode) {
	// except if >  max_size
      case SeekMode::Abs:
	offs = offset; 
	break;
	
      case SeekMode::Rel:

	if (in_bounds_incl<size_t> (offs+offset, 0, max_size))
	  offs += offset; 

	break;

      case SeekMode::End:

	if (in_bounds_excl<size_t> (offs + offset , 0, max_size))
	  offs = max_size + offset;

	break;
	
      }      
    
    }

    size_t GetPos () {
      return offs;

    }

    //
    //
    size_t Read (void* out, size_t len) {

      unsigned char*  ucout = reinterpret_cast<unsigned char*>(out);

      size_t read_len = len; 
      if ((offs + len) > max_size)
	read_len = max_size - offs;

      std::copy (mem+offs, mem+offs+read_len, ucout); 
      
      return read_len; 
    }

  protected:
    size_t               max_size; 
    size_t               offs; 
    const unsigned char* mem;
    
  };


  //
  //
  // WRITE STREAM
  struct write_stream_impl : public write_stream {

    write_stream_impl (void* src, size_t len)
      : write_stream()
      , max_size     (len)
      , offs        (0)
      , mem(reinterpret_cast<unsigned char*>(src)) {


    }
    
    //
    //
    void SetPos (ptrdiff_t offset, SeekMode mode) {

      switch (mode) {
	// except if >  max_size
      case SeekMode::Abs:
	offs = offset; 
	break;
	//
      case SeekMode::Rel:
	if (in_bounds_incl<size_t> (offs+offset, 0, max_size))
	  offs += offset; 
	break;
	// 
      case SeekMode::End:
	if (in_bounds_excl<size_t> (offs + offset , 0, max_size))
	  offs = max_size + offset;
	break;
      }      
    }
    
    // 
    size_t GetPos () {
      return offs; 
    }

    //
    size_t Write (const void* src, size_t len) {

      const unsigned char* ucsrc = reinterpret_cast<const unsigned char*> (src);

      size_t write_len = len; 
      if (in_bounds_excl<size_t> (offs+len, 0, max_size))
	write_len = max_size - offs; 
	  
      std::copy (ucsrc, ucsrc+write_len, mem+offs); 
      offs += write_len; 

      return write_len; 

    }
    // 
    size_t         max_size; 
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
