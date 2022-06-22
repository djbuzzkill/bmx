#include "FE_Math.h"


#include <gmp.h>


const unsigned kFE_bit_count = 256; 







class FE_ctx_impl : public  FE_context
{
public:
  FE_ctx_impl (size_t bitcount); 
  ~FE_ctx_impl ();

  enum { LocalElementStackSize = 32 } ; 
  
public:
  
  void Print (const char* msg, FE_t x);


  // get new element
  size_t New  (size_t v);
  size_t New  (const char *hexv, size_t base);

  
  FE_t Set (FE_t place, const char* strv, size_t base = 0 ); 
  FE_t Set (FE_t place, size_t v); 
 
  // returning element; 
  void   Del  (FE_t id); 

  FE_t Add (FE_t out, FE_t lhs, FE_t rhs);
  FE_t Sub (FE_t out, FE_t lhs, FE_t rhs); 
  FE_t Mul (FE_t out, FE_t lhs, FE_t rhs);
  FE_t Div (FE_t out, FE_t lhs, FE_t rhs);

  unsigned char* Raw (unsigned char* BE_bytes_out, FE_t); 

  inline __mpz_struct* el (FE_t x) { return &elems[x]; }
  inline __mpz_struct* loc(FE_t x) { return &locals[x]; }

  inline bool check (FE_t x) {return (x < elems.size () ? true : false); }


 
  typedef std::vector<__mpz_struct> mpz_array;  
public: 
  size_t num_field_bits;
  mpz_t  prime; 

  mpz_array elems; 
  mpz_array locals; 

};

FE_ctx_impl::  FE_ctx_impl (size_t bitcount) 
    : num_field_bits (bitcount)
  {
    mpz_init2 (prime, bitcount);
    elems.reserve (256); 

    if (elems.size() == 0)
      elems.push_back (__mpz_struct()); 
 
    mpz_init2 (prime, bitcount);

   // local temp 
    locals.resize (LocalElementStackSize ); 
    for (size_t i = 0; i < LocalElementStackSize; ++i)
      {
	mpz_init2 (loc(i), num_field_bits); 

      }
  } 

FE_ctx_impl::~FE_ctx_impl ()
  {
  }


void FE_ctx_impl:: Print (const char* msg, FE_t x)
{

  if (!check (x))
    return; 

  char strbuf_256[256];
  memset (strbuf_256, 0, 256);

  mpz_get_str (strbuf_256, 10, el(x));
  printf ("%s dec: %s\n", msg, strbuf_256) ;
  
  memset (strbuf_256, 0, 256); 
  mpz_get_str (strbuf_256, 2, el(x));
  printf ("%s hex : %s\n", msg, strbuf_256) ;

 
}

size_t FE_ctx_impl::New  (size_t v)
{
  elems.push_back (__mpz_struct()); 
  mpz_init (&elems.back ()); 
  mpz_set_ui (&elems.back (), v); 
  return elems.size () - 1; 
}


size_t FE_ctx_impl::New  (const char *strv, size_t base)
{
  elems.push_back (__mpz_struct()); 
  mpz_init (&elems.back ()); 
  mpz_set_str (&elems.back (), strv, base);
  return elems.size () - 1; 
}



FE_t FE_ctx_impl::Set (FE_t place, size_t v)
{
  if (!check(place))
      return 0; 

  mpz_set_ui (el(place), v); 
  return place; 
}

FE_t FE_ctx_impl::Set (FE_t place, const char* strv, size_t base)
{
  if (!check (place))
    return 0; 


  mpz_set_str (el(place), strv, base);
  return place; 

}

// returning element; 
void FE_ctx_impl::Del(FE_t id)
{
  

}

FE_t FE_ctx_impl::Add (FE_t out, FE_t lhs, FE_t rhs)
{
  if (!check (lhs) || !check (rhs) || !check (out))
    {
      return 0; 
    }

  
  enum { x = 0, y, z } ; 

  mpz_mod (loc(x), el(lhs), prime); 
  mpz_mod (loc(y), el(rhs), prime); 
  
  mpz_add (loc(z), loc(x), loc(y) ); 
  mpz_mod (el(out),loc(z), prime);
  return out;
  
}

FE_t FE_ctx_impl::Sub (FE_t out, FE_t lhs, FE_t rhs)
{
  return 0; 
}


FE_t FE_ctx_impl::Mul (FE_t out, FE_t lhs, FE_t rhs)
{
  return 0;
}

FE_t FE_ctx_impl::Div (FE_t out, FE_t lhs, FE_t rhs)
{
return 0;


}


unsigned char* FE_ctx_impl::Raw (unsigned char* BE_bytes_out, FE_t)
{
return 0;
}



FEContextPtr Create_FE_context (size_t field_bitcount )
{
  FEContextPtr ret (new FE_ctx_impl ( field_bitcount )); 
  
   return ret;
}



// convert this in to ::Raw ()
//

unsigned mpz_to_binary (std::vector<unsigned char>& out, mpz_t n, bool out_LE = true)
{
 
  assert (out.size () > 0);
  
  std::string rawmem (out.size() + 4, 0); 

  FILE* memfile = fmemopen (&rawmem[0], 128, "w+");

 if ( !memfile )
    {

      printf ("fmemopen failed\n"); 
      
      return 0; 
    }
  
  unsigned numbytes = 0;

  char* ch = reinterpret_cast<char*> (&numbytes); 
  int out_size =  mpz_out_raw (memfile, n);
  fclose  (memfile);

  std::any_of(ch, ch+out_size, isalnum);

  
  printf ("out_size:%i\n", out_size);
  
  ch[0] = rawmem[3];
  ch[1] = rawmem[2];
  ch[2] = rawmem[1];
  ch[3] = rawmem[0];
  
  printf ("numbytes:%u\n", numbytes);

  for (unsigned i = 0; i < numbytes; ++i)
  {
    
    out[i] = rawmem[4 + numbytes - 1 - i]; 
  }

  
  return numbytes;
}

void print_mpz (const char* msg, mpz_t n)
{

   
  char strbuf_256[256];
  memset (strbuf_256, 0, 256);

  mpz_get_str (strbuf_256, 10, n);
  printf ("%s dec: %s\n", msg, strbuf_256) ;
  
  memset (strbuf_256, 2, 256); 
  mpz_get_str (strbuf_256, 2, n);
  printf ("%s bin : %s\n", msg, strbuf_256) ;

  // memset (strbuf_256, 0, 256); 
  // mpz_get_str (strbuf_256, 16, n);
  // printf ("%s hex: %s\n", msg, strbuf_256) ;
 
  mpz_add_ui;
  mpz_mul_ui;
  mpz_sub_ui;
  mpz_mod_ui;
}


