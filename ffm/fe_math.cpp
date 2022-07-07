#include "fe_math.h"


#include <gmp.h>


namespace ffm
{

  
  inline bool is_INF (FE_t x) { return (x < 0 ? true : false); }   

  class FE_ctx_impl : public  FE_context, public std::enable_shared_from_this<FE_ctx_impl>
  {
public:
    FE_ctx_impl (const char*, size_t); 
    ~FE_ctx_impl ();
    
    
  public:
    char* Format (char* out, const char* fmt, FE_t x); 
    void Print (const char* msg, FE_t x);
    bool IsValid (FE_t); 
    
    // get new element
    FE_t New  ();
    // FE_t New  (const char *hexv, size_t base);
    
    
    void Set (FE_t place, const char* strv, size_t base = 0); 

    void Set_ui (FE_t place, size_t v); 
    void Set_si (FE_t place, long int v); 

    void Set_bin (FE_t lval, const unsigned char* bin, bool BE, size_t len); 
    void Set (FE_t lval, FE_t rval);
    
    // returning element; 
    void   Del  (FE_t id); 
    
    void Add (FE_t out, FE_t lhs, FE_t rhs);
    void Sub (FE_t out, FE_t lhs, FE_t rhs);
    void Sub_ui (FE_t, FE_t, size_t);  
    void Mul (FE_t out, FE_t lhs, FE_t rhs);
    void MulM (FE_t out, FE_t lhs, FE_t rhs, FE_t mod);
    void Div (FE_t out, FE_t lhs, FE_t rhs);
    void Pow (FE_t out, FE_t b, FE_t exp); 
    void Pow_ui (FE_t out, FE_t b, size_t exp); 
    void Pow_si (FE_t out, FE_t base, long int exp);
    // void Inv (FE_t out, FE_t x);

    void PowM (FE_t out, FE_t base, FE_t exp, FE_t mod);

    
    void powM ( mpz_t ou, const mpz_t ba, const mpz_t ex, const mpz_t mo);

    bool Rand (FE_t out, FE_t f);
    

           

    
    int Cmp (FE_t lhs, FE_t rhs);
    //int Cmp (FE_var lhs, FE_var rhs);
    
    int Cmp_ui (FE_t lhs, size_t rhs); 

    void LogiAnd (FE_t O, FE_t lhs , FE_t rhs);
    void LogiShiftR (FE_t x, size_t shift);
    void LogiShiftL (FE_t x, size_t shift);
    bool LogiBit (FE_t v, size_t pos);
    bool TestBit (FE_t v, size_t pos);  

    ByteArray& Raw (ByteArray& out, bool, FE_t); 
    
    inline __mpz_struct* el (FE_t x) { return &elems[x]; }
    //inline __mpz_struct* el (FE_t x) { return el(x.name()); }
    
    inline bool check (FE_t x) { return elems.count (x) ? true : false;  } 
    //inline bool check (FE_t x) {return check(x.name()); }
 
    typedef std::vector<__mpz_struct> mpz_array;
    
    typedef std::map<size_t, __mpz_struct> mpz_map;
    
  public: 
    // size_t num_field_bits;
    mpz_t Fp; 
    mpz_t Fp_minus_one;
    mpz_t Fp_minus_two; 
    mpz_map elems; 
    gmp_randstate_t randstate;

    
 };



  
  FE_ctx_impl::FE_ctx_impl (const char* strv, size_t base) 
    : elems ()
  {
    char strb[128];
    
    mpz_init (Fp); 
    mpz_set_str (Fp, strv, base);

    gmp_randinit_default (randstate); 

    
    gmp_sprintf  (strb, "%Zu", Fp); 
    printf ("Fp:%s\n", strb); 

    mpz_init (Fp_minus_one);
    mpz_sub_ui (Fp_minus_one, Fp, 1);

    gmp_sprintf  (strb, "%Zu", Fp_minus_one); 
    printf ("Fp-1:%s\n", strb); 

    mpz_init (Fp_minus_two);
    mpz_sub_ui (Fp_minus_two, Fp, 2); 
    gmp_sprintf  (strb, "%Zu", Fp_minus_two); 
    printf ("Fp-2:%s\n", strb); 

    
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


  void FE_ctx_impl::Set_si (FE_t place, long int si)
  {
    if (!check(place))
      return ;


    mpz_set_si (el(place), si); 
    

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
  
  void FE_ctx_impl:: Set_bin (FE_t lval, const unsigned char* bin, bool BE, size_t len) {
    printf ("%s:IMPLEMNTS PLZ\n", __FUNCTION__);  
     }


  
  // returning element; 
  void FE_ctx_impl::Del(FE_t x)
  {
    //printf ("DELETER[%i]\n", x);
    
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
    ScopeDeleter dr (shared_from_this());

    FE_t z = dr(New()); 
    mpz_add (el(z), el(lhs), el(rhs) ); 
    mpz_mod (el(out),el(z), Fp);
     
  }
 
  void FE_ctx_impl::Sub (FE_t out, FE_t lhs, FE_t rhs)
  {
    //    printf ("%s[out:%i, lhs:%i, rhs:%i]\n",__FUNCTION__, out, lhs, rhs); 
    ScopeDeleter dr (shared_from_this());
    FE_t y = dr (New());


    if (is_INF(lhs)) {
    }
    
    //printf (" [y:%i]\n", y); 


 
    mpz_sub (el(y), el(lhs), el(rhs));
    
    mpz_mod (el(out), el(y), Fp); 
    
  }

  //
  //
  void FE_ctx_impl::Sub_ui (FE_t out, FE_t lhs, size_t rhs)
  {
    ScopeDeleter dr (shared_from_this());
    FE_t tmp = dr(New());
    
    mpz_sub_ui(el(tmp), el(lhs), rhs);

    mpz_mod (el(out), el(tmp), Fp);  
  }

  //
  //
  void FE_ctx_impl::Mul (FE_t out, FE_t lhs, FE_t rhs)
  {
    ScopeDeleter dr (shared_from_this());
    FE_t r = dr (New());  
    mpz_mul (el(r), el(lhs), el(rhs));
    mpz_mod (el(out), el(r), Fp);
  }
  //
  //
  void FE_ctx_impl::MulM (FE_t out, FE_t lhs, FE_t rhs, FE_t mod)
  {
    ScopeDeleter dr (shared_from_this());
    FE_t r = dr (New());  
    mpz_mul (el(r), el(lhs), el(rhs));
    mpz_mod (el(out), el(r), el(mod));
  }
  //
  // out = (lhs / rhs)%Fp
  void FE_ctx_impl::Div (FE_t out, FE_t lhs, FE_t rhs)
  {
    
    ScopeDeleter dr (shared_from_this());
    FE_t inv_r = dr (New());
    FE_t z = dr (New());
    
    mpz_powm (el(inv_r), el(rhs), Fp_minus_two, Fp);

    mpz_mul (el(z), el(lhs), el(inv_r)); 

    mpz_mod (el(out), el(z), Fp);
 
  }

  // void FE_ctx_impl::Inv (FE_t out, FE_t x)
  // {
    
  //   mpz_powm (el(out), el(x), Fp_minus_two, Fp);  

  // }

  void FE_ctx_impl::Pow (FE_t out, FE_t b, FE_t exp)
  {
    mpz_powm (el(out), el(b), el(exp), Fp); 
  }


  void FE_ctx_impl::Pow_ui (FE_t out, FE_t base, size_t exp_ui)
  {
    //    def __pow__(self, exponent):
    //  n = exponent % (self.prime - 1)
    //  num = pow(self.num, n, self.prime)
    //  return self.__class__(num, self.prime)

    ScopeDeleter dr(shared_from_this()); 

    FE_t exp = dr(New_ui(exp_ui)); 
    FE_t n = dr (New ()); 

    mpz_mod (el(n), el(exp), Fp_minus_one); 
    mpz_powm (el(out), el(base), el(n), Fp); 
      
  }
 
  void FE_ctx_impl:: Pow_si (FE_t out, FE_t base, long int exp)
  {
    //def __pow__(self, exponent):
    //  n = exponent % (self.prime - 1)
    //  num = pow(self.num, n, self.prime)
    //  return self.__class__(num, self.prime)

    ScopeDeleter dr (shared_from_this());

    FE_t n = dr(New ());
    FE_t exponent = dr (New_si(exp)); 
    mpz_mod (el(n), el(exponent), Fp_minus_one); 
    mpz_powm (el(out), el(base), el(n), Fp);
    
  }

  //
  //
  void FE_ctx_impl::powM (mpz_t out, const mpz_t base, const mpz_t exp, const mpz_t mod) {
    mpz_powm (out, base, exp, mod);
  }

  //
  //
  void FE_ctx_impl::PowM (FE_t out, FE_t base, FE_t exp, FE_t mod)
  {
    if (!check(out)) {printf("%s:invalid out\n", __FUNCTION__);}
    if (!check(base)){printf("%s:invalid base\n", __FUNCTION__);}
    if (!check(exp)) {printf("%s:invalid exp\n", __FUNCTION__);}
    if (!check(mod)) {printf("%s:invalid mod\n", __FUNCTION__);}

    powM (el(out), el(base), el(exp), el(mod)); 
  }
  
  bool FE_ctx_impl::Rand (FE_t out, FE_t f)
  {
    mpz_urandomm (el(out), randstate, el(f)); 
    return true; 
  }
  //
  //
  int FE_ctx_impl::Cmp (FE_t lhs, FE_t rhs)
  {
    return mpz_cmp (el(lhs), el(rhs));
  }
  

  int FE_ctx_impl::Cmp_ui (FE_t lhs, size_t rhs)
  {
    return mpz_cmp_ui (el(lhs), rhs); 
  }

  void FE_ctx_impl::LogiAnd (FE_t O, FE_t lhs, FE_t rhs)
  {

    if (!(O) || !check (lhs) || !check (rhs))
      return;

    mpz_and (el(O), el(lhs), el(rhs));  
    
    
  }
  
  void FE_ctx_impl::LogiShiftR (FE_t x, size_t shift)
  {
    if (!check(x))
      return; 

    // close enuf?
    mpz_fdiv_q_ui (el(x), el(x), 2*shift); 
   
  }
  
  void FE_ctx_impl::LogiShiftL (FE_t x, size_t shift)
  {
    if (!check(x))
      return; 

    mpz_mul_ui (el(x), el(x), 2*shift); 
    
  }

  bool FE_ctx_impl::LogiBit (FE_t x , size_t pos)
  {
    if (!check(x))
      return false; 

    return (mpz_tstbit (el(x), pos) ? true : false);


  }

  bool FE_ctx_impl::TestBit (FE_t x, size_t pos)
  {

    if (!check(x))
      {
	printf ("%s failed..no such element:%i\n", __FUNCTION__, x);  
	
	return false;
      }

    
    return mpz_tstbit (el(x), pos) == 1 ? true : false; 
    
  }

  
  ByteArray& FE_ctx_impl::Raw (ByteArray& out, bool isBE, FE_t x)
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


