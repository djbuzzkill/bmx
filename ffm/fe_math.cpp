

#include "fe_math.h"


#include <gmp.h>


namespace ffm
{

  
  inline bool is_INF (FE_t x) { return (x < 0 ? true : false); }   

  class FE_ctx_impl : public  FE_context, public std::enable_shared_from_this<FE_ctx_impl>
  {
public:
    // FE_ctx_impl (FE_t& Fp, const char*, size_t); 
    FE_ctx_impl (const char*, size_t); 
    ~FE_ctx_impl ();
    
    
  public:
    FE_t p (); 

    char* Format (char* out, const char* fmt, FE_t x); 
    void Print (const char* msg, FE_t x);
    bool IsValid (FE_t); 


    
    // get new element
    FE_t New  ();
    // FE_t New  (const char *hexv, size_t base);
    
    
    void Set (FE_t place, const char* strv, size_t base); 

    void Set_ui (FE_t place, size_t v); 
    void Set_si (FE_t place, long int v); 

    void Set_bin (FE_t lval, const void* bin, unsigned len, bool isLE); 
    void Set (FE_t lval, FE_t rval);
    
    // returning element; 
    void   Del  (FE_t id); 
    
    void Add (FE_t out, FE_t lhs, FE_t rhs);
    void Sub (FE_t out, FE_t lhs, FE_t rhs);
    void Sub_ui (FE_t, FE_t, size_t);  
    void Mul (FE_t out, FE_t lhs, FE_t rhs);
    void MulM (FE_t out, FE_t lhs, FE_t rhs, FE_t mod);
    void Div (FE_t out, FE_t lhs, FE_t rhs);
  
    void Div_ui (FE_t, FE_t, size_t); 
    void DivMod (FE_t, FE_t, FE_t, FE_t);

    void FDiv    (FE_t out, FE_t num, FE_t den); 
    void FDiv_ui (FE_t out, FE_t num, size_t den); 
    //     KFunction: unsigned long int mpz_fdiv_q_ui (mpz_t q, const mpz_t n, unsigned long int d)

    void Sqrt (FE_t out, FE_t n); 
  
    size_t DivMod_ui (FE_t q, FE_t r, FE_t n, size_t d);

    void SAdd (FE_t out, FE_t lhs, FE_t rhs);
    void SMul (FE_t out, FE_t lhs, FE_t rhs); 
    void SSub (FE_t out, FE_t lhs, FE_t rhs); 

    void SAdd_ui   (FE_t out, FE_t lhs, unsigned long int rhs);
    void SMul_ui   (FE_t out, FE_t lhs, unsigned long int rhs); 
    void SSub_ui   (FE_t out, FE_t lhs, unsigned long int rhs); 
  
    void SNeg      (FE_t out, FE_t num); 
    void SAbs      (FE_t out, FE_t num); 

    void Pow       (FE_t out, FE_t b, FE_t exp); 
    void Pow_ui    (FE_t out, FE_t b, size_t exp); 
    void Pow_si    (FE_t out, FE_t base, long int exp);

    // void Inv (FE_t out, FE_t x);
    void PowM      (FE_t out, FE_t base, FE_t exp, FE_t mod);
    bool Rand      (FE_t out, FE_t f);
      
    int Cmp (FE_t lhs, FE_t rhs);
    //int Cmp (FE_var lhs, FE_var rhs);
    
    int Cmp_ui (FE_t lhs, size_t rhs); 

    void LogiAnd (FE_t O, FE_t lhs , FE_t rhs);
    void LogiShiftR (FE_t x, size_t shift);
    void LogiShiftL (FE_t x, size_t shift);
    bool LogiBit (FE_t v, size_t pos);
    bool TestBit (FE_t v, size_t pos);  

    bytearray& Raw (bytearray& out, int& sgn, FE_t, bool isLE); 
    //
    //
    inline void powM (mpz_t out, const mpz_t base, const mpz_t exp, const mpz_t mod) {
      mpz_powm (out, base, exp, mod);
    }

    inline void sub_ui (mpz_t out, const mpz_t lhs, size_t rhs, mpz_t tmp) {
      mpz_sub_ui(tmp, lhs, rhs);
      mpz_mod (out, tmp, el(Fp));  
    }
    
    inline __mpz_struct* el (FE_t x) { return &elems[x]; }
    //inline __mpz_struct* el (FE_t x) { return el(x.name()); }
    
    inline bool check (FE_t x) { return elems.count (x) ? true : false;  } 
    //inline bool check (FE_t x) {return check(x.name()); }
 
    typedef std::vector<__mpz_struct> mpz_array;
    
    typedef std::map<size_t, __mpz_struct> mpz_map;
    
  public: 
    // size_t num_field_bits;
    FE_t Fp;
    FE_t Fp_minus_one;
    FE_t Fp_minus_two; 
    mpz_map elems; 
    gmp_randstate_t randstate;
    
 };



  
  FE_ctx_impl::FE_ctx_impl (const char* strv, size_t base) : elems () {
    char strb[256];
    // printf ("%s(strv:%s, base:%zu)\n", __FUNCTION__, strv, base);  
    // 
    gmp_randinit_default (randstate); 
    //mpz_set_str (Fp, strv, base);

    Fp = New ();
    Fp_minus_one = New ();
    Fp_minus_two = New ();
    FE_t tmp = New ();
    
    Set (Fp, strv, base); 
    gmp_sprintf  (strb, "%Zu", el(Fp)); 
    // printf ("  Fp:%s\n", strb); 
   
    sub_ui (el(Fp_minus_one), el(Fp), 1, el(tmp));
    gmp_sprintf  (strb, "%Zu", el(Fp_minus_one)); 
    // printf ("  Fp-1:%s\n", strb); 

    sub_ui (el(Fp_minus_two), el(Fp), 2, el(tmp));
    gmp_sprintf  (strb, "%Zu", el(Fp_minus_two)); 
    // printf ("  Fp-2:%s\n", strb); 

    Del (tmp); 

  } 

  FE_ctx_impl::~FE_ctx_impl () {
    //printf ("%s\n", __FUNCTION__);  
  }

  FE_t FE_ctx_impl::p () {
    return Fp;
  } 
  
  char* FE_ctx_impl::Format (char* out, const char* fmt, FE_t x) {
    gmp_sprintf (out, fmt, el(x));
    return out; 
  }


  void FE_ctx_impl:: Print (const char* msg, FE_t x) {
    
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
    FE_t name = 1; // 0 is forbidden name
    for (; elems.count (name); ++name);
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
    if (!check (lv) || !check (rv)) {
	// error
      return; 
    }
    
    mpz_set(el(lv), el(rv)); 

  }
  
  void FE_ctx_impl:: Set_bin (FE_t lval, const void* bin, unsigned len, bool isLE) {


    union {
      unsigned lenbin; 
      unsigned char uclen[4];
    }; 
    
    lenbin = len; 
    bytearray bytes (len+6, 0); 
    bytes[0] = uclen[3];
    bytes[1] = uclen[2];
    bytes[2] = uclen[1];
    bytes[3] = uclen[0];

    //printf ("bytes.size: %zu\n", bytes.size()); 

    FILE* fp = fmemopen (std::data(bytes), bytes.size(), "rb") ;

    const unsigned char* ucbin = reinterpret_cast<const unsigned char*> (bin); 
    if (isLE) { // strv is LE
      std::reverse_copy (ucbin, ucbin+len, &bytes[4]); 
    }
    else {
      std::copy (ucbin, ucbin+len, &bytes[4]); 
    }

    size_t res = mpz_inp_raw (el(lval), fp);
   // Function: size_t mpz_inp_raw (mpz_t rop, FILE *stream)
    // Input from stdio stream stream in the format written by mpz_out_raw, and put
    // the result in rop. Return the number of bytes read, or if an error occurred, return 0.
    // This routine can read the output from mpz_out_raw also from GMP 1, in spite of changes
    // necessary for compatibility between 32-bit and 64-bit machines.
    fflush (fp); 
    fclose (fp); 
  }

  // returning element; 
  void FE_ctx_impl::Del(FE_t x) {
    //printf ("DELETER[%i]\n", x);
    mpz_map::iterator it = elems.find (x); 
    // 
    if (it != elems.end()) {
      mpz_clear ( el(x) ); 
      elems.erase (it); 
    }
  }
  
  //
  //  out = (lhs + rhs)%Fp
  // out = (lhs - rhs)%Fp
  // out = 1/x
  void FE_ctx_impl::Add (FE_t out, FE_t lhs, FE_t rhs) {
    ScopeDeleter dr  (shared_from_this());
    FE_t z = dr (New()); 

    mpz_add (el(z), el(lhs), el(rhs) ); 
    mpz_mod (el(out),el(z), el(Fp));
  }
 
  void FE_ctx_impl::Sub (FE_t out, FE_t lhs, FE_t rhs) {
    if (is_INF(lhs)) {
    }
    ScopeDeleter dr (shared_from_this());
    FE_t y = dr(New());
   
    mpz_sub (el(y), el(lhs), el(rhs));
    mpz_mod (el(out), el(y), el(Fp)); 
  }

  //
  //

 
  void FE_ctx_impl::Sub_ui (FE_t out, FE_t lhs, size_t rhs) {
    ScopeDeleter dr (shared_from_this());
    FE_t tmp = dr(New());

    sub_ui (el(out), el(lhs), rhs, el(tmp)); 
  }

  //
  //
  void FE_ctx_impl::Mul (FE_t out, FE_t lhs, FE_t rhs) {
    ScopeDeleter dr (shared_from_this());
    FE_t r = dr(New());  
    mpz_mul (el(r), el(lhs), el(rhs));
    mpz_mod (el(out), el(r), el(Fp));

  }




  void FE_ctx_impl::SAdd (FE_t out, FE_t lhs, FE_t rhs) {
    mpz_add (el(out), el(lhs), el(rhs) ); 

  }
  void FE_ctx_impl::SMul (FE_t out, FE_t lhs, FE_t rhs) {

    mpz_mul (el(out), el(lhs), el(rhs));

  }

  void FE_ctx_impl::SSub (FE_t out, FE_t lhs, FE_t rhs) {
    mpz_sub (el(out), el(lhs), el(rhs)); 
  }

  
  void FE_ctx_impl::SAdd_ui   (FE_t out, FE_t lhs, unsigned long int rhs) {

    mpz_add_ui (el(out), el(lhs), rhs);
    
  }
  void FE_ctx_impl::SMul_ui   (FE_t out, FE_t lhs, unsigned long int rhs) {
    mpz_mul_ui (el(out), el(lhs), rhs);
    
  }

  void FE_ctx_impl::SSub_ui   (FE_t out, FE_t lhs, unsigned long int rhs) {
    mpz_sub_ui (el(out), el(lhs), rhs); 
    
  }
  
  void FE_ctx_impl::SNeg (FE_t out, FE_t rhs) {
    mpz_neg (el(out), el(rhs)); 
  }

  void FE_ctx_impl::SAbs(FE_t out, FE_t num) {
    //Function: void
    mpz_abs (el(out), el(num)); 
  }
    


  //
  //
  void FE_ctx_impl::MulM (FE_t out, FE_t lhs, FE_t rhs, FE_t mod) {
    ScopeDeleter dr (shared_from_this());
    FE_t r = dr (New());  
    mpz_mul (el(r), el(lhs), el(rhs));
    mpz_mod (el(out), el(r), el(mod));
  }
  //
  // out = (lhs / rhs)%Fp
  void FE_ctx_impl::Div (FE_t out, FE_t lhs, FE_t rhs) {
    
    ScopeDeleter dr (shared_from_this());
    FE_t inv_r   = dr(New());
    FE_t z       = dr(New());
    
    mpz_powm (el(inv_r), el(rhs), el(Fp_minus_two), el(Fp));

    mpz_mul (el(z), el(lhs), el(inv_r)); 

    mpz_mod (el(out), el(z), el(Fp));
  }

  // out:mpz_t = num:mpz_t / den:size_t
  void FE_ctx_impl::Div_ui (FE_t out, FE_t num, size_t den) {
    // Function: unsigned long int mpz_fdiv_q_ui (mpz_t q, const mpz_t n, unsigned long int d)   
    ScopeDeleter dr (shared_from_this());
    FE_t tmp = dr(New());
    mpz_fdiv_q_ui (el(tmp), el(num), den);
    mpz_mod(el(out), el(tmp), el(Fp)); 
  }

  //
  void FE_ctx_impl::DivMod (FE_t q, FE_t r, FE_t num, FE_t den) {
    // void mpz_fdiv_qr (mpz_t q, mpz_t r, const mpz_t n, const mpz_t d)
    mpz_fdiv_qr (el(q), el(r), el(num), el(den));  

  }
  size_t FE_ctx_impl::DivMod_ui (FE_t q, FE_t r, FE_t num, size_t den) {
    // unsigned long int mpz_fdiv_qr_ui (mpz_t q, mpz_t r, const mpz_t n, unsigned long int d)
    return mpz_fdiv_qr_ui (el(q), el(r), el(num), den);  
  }

  void FE_ctx_impl::FDiv (FE_t out, FE_t num, FE_t den) {
    mpz_fdiv_q (el(out), el(num), el(den)); 
    //     KFunction: unsigned long int mpz_fdiv_q_ui (mpz_t q, const mpz_t n, unsigned long int d)
  }

  void FE_ctx_impl::FDiv_ui (FE_t out, FE_t num, size_t den) {
    mpz_fdiv_q_ui (el (out), el(num), den); 
  }


  void FE_ctx_impl::Sqrt (FE_t out, FE_t n) {

    ScopeDeleter dr (shared_from_this());

    FE_t Fp_plus_one = dr (New());
    FE_t p0          = dr (New());
    
    mpz_add_ui (el(Fp_plus_one), el(Fp), 1); 
    mpz_fdiv_q_ui (el (p0), el(Fp_plus_one), 4);
    powM (el(out), el(n), el(p0), el(Fp)); 
    // def sqrt(self):
    //     return self**((P + 1) // 4)
  }


  // void FE_ctx_impl::Inv (FE_t out, FE_t x)
  // {
    
  //   mpz_powm (el(out), el(x), Fp_minus_two, Fp);  

  // }

  void FE_ctx_impl::Pow (FE_t out, FE_t b, FE_t exp) {
    mpz_powm (el(out), el(b), el(exp), el(Fp)); 
  }


  void FE_ctx_impl::Pow_ui (FE_t out, FE_t base, size_t exp_ui) {
    //def __pow__(self, exponent):
    //  n = exponent % (self.prime - 1)
    //  num = pow(self.num, n, self.prime)
    //  return self.__class__(num, self.prime)

    ScopeDeleter dr (shared_from_this()); 
    FE_t exp = dr (New_ui(exp_ui)); 
    FE_t n   = dr (New ()); 

    mpz_mod (el(n), el(exp), el(Fp_minus_one)); 
    mpz_powm (el(out), el(base), el(n), el(Fp)); 
  }
 
  void FE_ctx_impl:: Pow_si (FE_t out, FE_t base, long int exp) {
    //def __pow__(self, exponent):
    //  n = exponent % (self.prime - 1)
    //  num = pow(self.num, n, self.prime)
    //  return self.__class__(num, self.prime)

    ScopeDeleter dr (shared_from_this());

    FE_t n = dr (New ());
    FE_t exponent =  dr(New_si(exp)); 
    mpz_mod (el(n), el(exponent), el(Fp_minus_one)); 
    mpz_powm (el(out), el(base), el(n), el(Fp));
    
  }

  //
  //
  void FE_ctx_impl::PowM (FE_t out, FE_t base, FE_t exp, FE_t mod) {
    if (!check(out)) {printf("%s:invalid out\n", __FUNCTION__);}
    if (!check(base)){printf("%s:invalid base\n", __FUNCTION__);}
    if (!check(exp)) {printf("%s:invalid exp\n", __FUNCTION__);}
    if (!check(mod)) {printf("%s:invalid mod\n", __FUNCTION__);}

    powM (el(out), el(base), el(exp), el(mod)); 
  }
  
  bool FE_ctx_impl::Rand (FE_t out, FE_t f) {


    Print ("Rand:out", out); 
    Print ("Rand:f", f); 

    printf ("%s|%i\n", __FUNCTION__, __LINE__); 
    mpz_urandomm (el(out), randstate, el(f)); 
    printf ("%s|%i\n", __FUNCTION__, __LINE__);
    return true; 
  }
  //
  //
  int FE_ctx_impl::Cmp (FE_t lhs, FE_t rhs) {

    // Function: int mpz_cmp (const mpz_t op1, const mpz_t op2)
    // Function: int mpz_cmp_d (const mpz_t op1, double op2)
    // Macro: int mpz_cmp_si (const mpz_t op1, signed long int op2)
    // Macro: int mpz_cmp_ui (const mpz_t op1, unsigned long int op2)
    // Compare op1 and op2. Return a positive value if op1 > op2, zero if op1 = op2, or a negative value if op1 < op2.
    return mpz_cmp (el(lhs), el(rhs));
  }
  

  int FE_ctx_impl::Cmp_ui (FE_t lhs, size_t rhs) {
    return mpz_cmp_ui (el(lhs), rhs); 
  }

  void FE_ctx_impl::LogiAnd (FE_t O, FE_t lhs, FE_t rhs) {
    if (!(O) || !check (lhs) || !check (rhs))
      return;

    mpz_and (el(O), el(lhs), el(rhs));  
  }
  
  void FE_ctx_impl::LogiShiftR (FE_t x, size_t shift) {
    if (!check(x))
      return; 

    ScopeDeleter dr (shared_from_this()); 
    FE_t base = dr (New_ui (2));
    FE_t den  = dr (New ()); 
    
    // close enuf?
    mpz_powm_ui (el(den), el(base), shift , el(Fp));
    mpz_fdiv_q (el(x), el(x), el(den)); 
   
  }
  
  void FE_ctx_impl::LogiShiftL (FE_t x, size_t shift) {
    if (!check(x))
      return;
    ScopeDeleter dr (shared_from_this()); 
    FE_t base = dr (New_ui (2));
    FE_t out  = dr (New ()); 

    mpz_powm_ui (el(out), el(base), shift , el(Fp));
    mpz_mul (el(x), el(x), el(out)); 
    
  }

  bool FE_ctx_impl::LogiBit (FE_t x , size_t pos) {
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

  //
  //
  bytearray& FE_ctx_impl::Raw (bytearray& out, int& sgn, FE_t x, bool want_LE) {
    
    if (!check (x))
      return out;

    //  Function: size_t mpz_size (const mpz_t op)
    //  Return the size of op measured in number of limbs. If op is zero, the returned value will be zero.
    const size_t sizeOf_limb = sizeof(mp_limb_t);
    size_t       num_limbs   = mpz_size (el(x));
    size_t       bufsize     = (num_limbs * sizeOf_limb)+8; // +8 for null char
    
    // printf ("sizeOf_limb:%zu\n", sizeOf_limb); 
    // printf ("num_limbs[%zu] \n", num_limbs); 
    // printf ("bufsize[%zu] \n", bufsize); 
    
    bytearray tmp (bufsize, 0);
    FILE* memf  = fmemopen (std::data(tmp), tmp.size(), "w"); 
    
    if (!memf ) {
      printf ("fmemopen failed\n"); 
      return out; 
    }
    
    
    size_t out_size = mpz_out_raw (memf, el(x));
    
    fflush (memf);  
    fclose (memf);
    
    //printf ("out_size[%zu] \n", out_size); 
      
    // Output op on stdio stream stream, in raw binary format. The
    // integer is written in a portable format, with 4 bytes of size
    // information, and that many bytes of limbs. Both the size and the
    // limbs are written in decreasing significance order (i.e., in
    // big-endian).
    union {
      int  numbytes;
      unsigned char _bytes[4]; 
    };
    
    // b/c we x86
    _bytes[0] = tmp[3];
    _bytes[1] = tmp[2];
    _bytes[2] = tmp[1];
    _bytes[3] = tmp[0];
    
    
    sgn = numbytes; 

    unsigned absnum = std::abs (numbytes); 
    assert ( (out_size - 4) == absnum);
    //printf ("[union numbytes : %i] \n", numbytes); 
    
    // printf ("out_size:%i\n", out_size);
    // printf ("[%s]numbytes[%u]\n", __FUNCTION__, numbytes); 
    
    out.resize(absnum, 0);
    
    std::copy (&tmp[4], &tmp[4+absnum], out.begin ());
    
    if (want_LE)  {
      std::reverse (out.begin(), out.end());
    }
    
    return out;
  }
  
  //
  //
  FEConRef Create_FE_context (const char* strv, size_t base ) {
    
    FEConRef ret (std::make_shared<FE_ctx_impl>( strv, base )); 
    
    return ret;
  }
  


  // convert this in to ::Raw ()
  //

unsigned mpz_to_binary (std::vector<unsigned char>& out, mpz_t n, bool out_LE ) {
 
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


