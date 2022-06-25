#include "FE_Math.h"


#include <gmp.h>


namespace FFM
{

  class FE_ctx_impl : public  FE_context, public std::enable_shared_from_this<FE_ctx_impl>
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
    FE_t New  ();
    // FE_t New  (const char *hexv, size_t base);
    
    
    void Set (FE_t place, const char* strv, size_t base = 0); 
    void Set_ui (FE_t place, size_t v); 
    void Set (FE_t lval, FE_t rval);
    
    // returning element; 
    void   Del  (FE_t id); 
    
    void Add (FE_t out, FE_t lhs, FE_t rhs);
    void Sub (FE_t out, FE_t lhs, FE_t rhs); 
    void Mul (FE_t out, FE_t lhs, FE_t rhs);
    void Div (FE_t out, FE_t lhs, FE_t rhs);
    void Pow (FE_t out, FE_t b, FE_t exp); 
    void Pow_ui (FE_t out, FE_t b, size_t exp); 
    void Inv (FE_t out, FE_t x);


    int Cmp (FE_t lhs, FE_t rhs);
    //int Cmp (FE_var lhs, FE_var rhs);
    

    
    ByteArray& Raw (ByteArray& out, FE_t); 
    
    inline __mpz_struct* el (FE_t x) { return &elems[x]; }
    //inline __mpz_struct* el (FE_t x) { return el(x.name()); }
    inline __mpz_struct* loc(size_t x) { return &locals[x]; }
    
    inline bool check (FE_t x) { return elems.count (x) ? true : false;  } 
    //inline bool check (FE_t x) {return check(x.name()); }
 
    typedef std::vector<__mpz_struct> mpz_array;
    
    typedef std::map<size_t, __mpz_struct> mpz_map;
    
  public: 
    // size_t num_field_bits;
    mpz_t  Fp; 
    mpz_t Fp_minus_two; 
    mpz_map elems; 
    mpz_array locals; 

  };



  
  FE_ctx_impl::FE_ctx_impl (const char* strv, size_t base) 
    : elems ()
    , locals (LocalElementStackSize)
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
    printf ("%s:%s\n", msg, strbuf_256) ;
    
 }

  bool FE_ctx_impl::IsValid (FE_t x)
  {
    return check (x); 
  }



  
  // allocate new field element
  FE_t FE_ctx_impl::New ()
  {
    // 0 is forbidden name
    FE_t name = 1; 
    for (; elems.count (name); ++name);
    //FE_t ret;
    //ret.name() = name;

    //std::get<1>(ret) = shared_from_this (); 
    mpz_init (el(name));
    
    return name; 
  }
  
 // 
  void FE_ctx_impl::Set_ui (FE_t place, size_t v)
  {
    if (!check(place))
      return ; 
    
    mpz_set_ui (el(place), v); 
  }

  // 
  void FE_ctx_impl::Set (FE_t place, const char* strv, size_t base)
  {
    if (!check (place))
      return ; 
    
    
    mpz_set_str (el(place), strv, base);
    
  }

  void FE_ctx_impl ::Set (FE_t lv, FE_t rv)
  {
    if (!check (lv) || !check (rv))
      {
	// error
      return; 
      }
    
    mpz_set(el(lv), el(rv)); 

  }
  
  // returning element; 
  void FE_ctx_impl::Del(FE_t x)
  {
    mpz_map::iterator it = elems.find (x); 
    
    if (it != elems.end())
      {
	mpz_clear ( el(x) ); 
	elems.erase (it); 
      }
  }

  //
  //  out = (lhs + rhs)%Fp
  // out = (lhs - rhs)%Fp
  // out = 1/x
  void FE_ctx_impl::Add (FE_t out, FE_t lhs, FE_t rhs)
  {
    
    enum { x=0, y, z };  
    
    mpz_add (loc(z), el(lhs), el(rhs) ); 
    mpz_mod (el(out),loc(z), Fp);
     
  }
 
  void FE_ctx_impl::Sub (FE_t out, FE_t lhs, FE_t rhs)
  {
     enum {x= 0, y, z};
    
    mpz_sub (loc(y), el(lhs), el(rhs));
    
    mpz_mod (el(out), loc(y), Fp); 
    
  }
 
  void FE_ctx_impl::Mul (FE_t out, FE_t lhs, FE_t rhs)
  {
    enum { x = 0, y , z};
    
    mpz_mul (loc(x), el(lhs), el(rhs));
    mpz_mod (el(out), loc(x), Fp);
  }
  //
  // out = (lhs / rhs)%Fp
  void FE_ctx_impl::Div (FE_t out, FE_t lhs, FE_t rhs)
  {
    
    enum {x=0, y, z}; 
  
    mpz_powm (loc(x), el(rhs), Fp_minus_two, Fp);

    mpz_mul (loc(z), el(lhs), loc(x)); 

    mpz_mod (el(out), loc(z), Fp);
 
  }

  void FE_ctx_impl::Inv (FE_t out, FE_t x)
  {
    mpz_powm (el(out), el(x), Fp_minus_two, Fp);  
  }

  void FE_ctx_impl::Pow (FE_t out, FE_t b, FE_t exp)
  {
    mpz_powm (el(out), el(b), el(exp), Fp); 
  }


  void FE_ctx_impl::Pow_ui (FE_t out, FE_t b, size_t exp)
  {
    mpz_pow_ui (el(out), el(b), exp);
    mpz_mod (el(out), el(out), Fp);
  }
 


  int FE_ctx_impl::Cmp (FE_t lhs, FE_t rhs)
  {
    return mpz_cmp (el(lhs), el(rhs));
  }
  

 
  ByteArray& FE_ctx_impl::Raw (ByteArray& out, FE_t x)
  {
    if (!check (x))
      return out;


    const size_t buffersize = 256;  
    ByteArray tmp (buffersize, 0);

    
   FILE* memfile = fmemopen (&tmp[0], 256, "w+");
   
   if ( !memfile )
     {
       
       printf ("fmemopen failed\n"); 
       
       return out; 
     }


   char numbytes_BE[4]; 
   unsigned& numbytes = reinterpret_cast<unsigned&> (numbytes_BE);

   int out_size =  mpz_out_raw (memfile, el(x));
   fclose  (memfile);
   
   printf ("out_size:%i\n", out_size);
  
   numbytes_BE[0] = tmp[3];
   numbytes_BE[1] = tmp[2];
   numbytes_BE[2] = tmp[1];
   numbytes_BE[3] = tmp[0];
   
   printf ("numbytes:%u\n", numbytes);


   out.resize(numbytes);    
   for (unsigned i = 0; i < numbytes; ++i)
     {
    
       out[i] = tmp[4 + numbytes - 1 - i]; 
     }

   return out;
  }

  //
  //
  FEConPtr Create_FE_context (const char* strv, size_t base )
  {
    FEConPtr ret (std::make_shared<FE_ctx_impl>( strv, base )); 
    
    return ret;
  }
  


  // convert this in to ::Raw ()
  //

unsigned mpz_to_binary (std::vector<unsigned char>& out, mpz_t n, bool out_LE )
{
 
  assert (out.size () > 0);

  std::string rawmem (out.size() + 4, 0); 

  return 0;
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


