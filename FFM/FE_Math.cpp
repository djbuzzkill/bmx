#include "FE_Math.h"


#include <gmp.h>


const unsigned kFE_bit_count = 256; 









namespace FFM
{

  class FE_ctx_impl : public  FE_context
  {
public:
    FE_ctx_impl (const char*, size_t); 
    ~FE_ctx_impl ();
    
    enum { LocalElementStackSize = 32 } ; 
    
  public:
    char* Format (char* out, const char* fmt, FE_t x); 
    void Print (const char* msg, FE_t x);
    bool IsValid (FE_t); 
    
    // get new element
    size_t New  ();
    size_t New  (const char *hexv, size_t base);
    
    
    FE_t Set (FE_t place, const char* strv, size_t base = 0 ); 
    FE_t Set (FE_t place, size_t v); 
    
    // returning element; 
    void   Del  (FE_t id); 
    
    FE_t Add (FE_t out, FE_t lhs, FE_t rhs);
    FE_t Sub (FE_t out, FE_t lhs, FE_t rhs); 
    FE_t Mul (FE_t out, FE_t lhs, FE_t rhs);
    FE_t Div (FE_t out, FE_t lhs, FE_t rhs);
    
    FE_t Inv (FE_t out, FE_t x);
    
    unsigned char* Raw (unsigned char* BE_bytes_out, FE_t); 
    
    inline __mpz_struct* el (FE_t x) { return &elems[x]; }
    inline __mpz_struct* loc(FE_t x) { return &locals[x]; }
    
    inline bool check (FE_t x) {return (elems.count (x) ? true : false); }
    

 
    typedef std::vector<__mpz_struct> mpz_array;
    
    typedef std::map<size_t, __mpz_struct> mpz_map;
    
  public: 
    // size_t num_field_bits;
    mpz_t  Fp; 
    mpz_t Fp_minus_two; 
    mpz_map elems; 
    mpz_array locals; 

  };



  
  FE_ctx_impl::  FE_ctx_impl (const char* strv, size_t base) 
    : locals (LocalElementStackSize)
    , elems ()
  {
 
    mpz_init (Fp); 
    mpz_set_str (Fp, strv, base);


    mpz_init (Fp_minus_two);
    
    mpz_sub_ui (Fp_minus_two, Fp, 2); 
    
    // local temp 
    for (size_t i = 0; i < LocalElementStackSize; ++i)
      {
	mpz_init (loc(i)); 
      }
  } 

FE_ctx_impl::~FE_ctx_impl ()
  {
  }


  
  char* FE_ctx_impl::Format (char* out, const char* fmt, FE_t x)
  {

    
    gmp_sprintf (out, fmt, el(x));
    return out; 
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

  bool FE_ctx_impl::IsValid (FE_t x)
  {
    return check (x); 
  }

  // allocate new field element
  size_t FE_ctx_impl::New ()
  {
    size_t name = 1; 
    for (; elems.count (name); ++name);
    
    mpz_init (el(name));
    
    return name; 
  }
  
  
  size_t FE_ctx_impl::New (const char *strv, size_t base)
  {
    size_t name = New (); 
    mpz_set_str (el(name), strv, base);
    
    return name; 
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
    
    mpz_map::iterator it = elems.find (id); 
    
    if (it != elems.end())
      {
	mpz_clear ( el(id) ); 
	elems.erase (it); 
      }
  }
  
  FE_t FE_ctx_impl::Add (FE_t out, FE_t lhs, FE_t rhs)
  {
    if (!check (lhs) || !check (rhs) || !check (out))
      {
	return 0; 
      }
    
    enum { x=0, y, z };  
    
    mpz_add (loc(z), el(lhs), el(rhs) ); 
    mpz_mod (el(out),loc(z), Fp);
    return out;
    
  }
  
  FE_t FE_ctx_impl::Sub (FE_t out, FE_t lhs, FE_t rhs)
  {
    if (!check(out) || !check(lhs) || !check(rhs))
      return 0;
    
    enum {x= 0, y, z};
    
    mpz_sub (loc(y), el(lhs), el(rhs));
    
    mpz_mod (el(out), loc(y), Fp); 
    
    return 0; 
  }
  
  //
  //
  FE_t FE_ctx_impl::Mul (FE_t out, FE_t lhs, FE_t rhs)
  {
    if (!check(out) || !check(lhs) || !check(rhs))
      return 0;
    
    enum { x = 0, y , z};
    
    mpz_mul (loc(x), el(lhs), el(rhs));
    mpz_mod (el(out), loc(x), Fp);
 
    return out;
  }

  //
  //
  FE_t FE_ctx_impl::Inv(FE_t out, FE_t x)
  {
    if (!check(out) || !check(x))
      return 0;

    mpz_powm (el(out), el(x), Fp_minus_two, Fp);  
    return out; 
    
    
  }


  //
  //

  FE_t FE_ctx_impl::Div (FE_t out, FE_t lhs, FE_t rhs)
  {

    if (!check (lhs) || !check (rhs))
      return 0;

    enum {x=0, y, z}; 
  
    mpz_powm (loc(x), el(rhs), Fp_minus_two, Fp);

    mpz_mul (loc(z), el(lhs), loc(x)); 

    mpz_mod (el(out), loc(z), Fp);
  
    return out;
  }


  unsigned char* FE_ctx_impl::Raw (unsigned char* BE_bytes_out, FE_t)
  {
    return 0;
  }

  //
  //
  FEConPtr Create_FE_context (const char* strv, size_t base )
  {
    FEConPtr ret (new FE_ctx_impl ( strv, base )); 
    
    return ret;
  }
  


  // convert this in to ::Raw ()
  //

unsigned mpz_to_binary (std::vector<unsigned char>& out, mpz_t n, bool out_LE )
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

} // FM


