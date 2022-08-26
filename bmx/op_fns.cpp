
//
//
#include "op_fns.h"
#include "secp256k1.h"

#include "aframe/af.h"



namespace priv_op {

  using namespace ffm;
  using namespace af; 

  //  encode x to put on stack 
  bytearray& encode_num (bytearray& enc_out,  const bytearray& num_BE) {
    // num_BE[0]      = most significant
    // num_BE[size-1] = least significant

    pt::map  pm;
    el::map  em;
    FEConRef F  (nullptr);
    ECConRef EC (nullptr); 

    enc_out.clear ();

    bool all_zeros = std::all_of (num_BE.begin(), num_BE.end(), [](auto c) -> bool { return c==0; }); 
    if (all_zeros || num_BE.size () == 0) {
      return enc_out;
    }

    if (!bmx::Init_secp256k1_Env (F, EC, em, pm)) { 
      return enc_out; 
    }

    FE_t num = F->New_bin (&num_BE[0], num_BE.size (), false);
    // if num_BE < 0
    if (F->Cmp_ui (num, 0) < 0) {
      // Compare op1 and op2. Return a positive value if op1 > op2, zero
      // if op1 = op2, or a negative value if op1 < op2.
      FE_t abs_n = F->New (); 
      F->Neg (abs_n, num);  //Function: void mpz_neg (mpz_t rop, const mpz_t o)p

      bytearray abs_bin; // now get the bits
      F->Raw ( abs_bin, abs_n , false );  // false == BE?

      size_t nbytes = abs_bin.size () ;
      while (nbytes) {
	size_t ind = nbytes - 1;
	enc_out.push_back ( abs_bin[ind] );
	--nbytes; 
      }


      if (enc_out.back () & 0x80) {
	// if hi bit already on, put another byte
	enc_out.push_back (0x80);
        }
      
      return enc_out; 
      
    }

    
    // num > 0
    size_t nbytes  = num_BE.size ();
    while (nbytes) {
      enc_out.push_back (num_BE[nbytes-1]);
      --nbytes;
    }      

    if (enc_out.back () & 0x80) {
      // if hi bit is on append 0
      enc_out.push_back (0x00);
    }

    
    return enc_out;
  }
  
  //
  // decode to read thing from stack
  af::bytearray& decode_num (af::bytearray& dec_out, const af::bytearray& n_enc) {

    pt::map  pm;
    el::map  em;

    FEConRef F  (nullptr);
    ECConRef EC (nullptr); 

    dec_out.clear ();

    bool all_zeros = std::all_of (n_enc.begin(), n_enc.end(), [] (auto c) -> bool { return (c == 0); }); 

    if (n_enc.size () == 0 || all_zeros) {
      return dec_out;
    }

    // def decode_num(element):
    //     if element == b'':
    //         return 0

    if (!bmx::Init_secp256k1_Env (F, EC, em, pm)) { 
      return dec_out; 
    }

    bytearray n_BE = n_enc;
    std::reverse (n_BE.begin(), n_BE.end ());
    //     # reverse for big endian
    //     big_endian = element[::-1]
    //     # top bit being 1 means it's negative


    FE_t num = F->New_bin (&n_BE[1], n_BE.size() - 1, false);

    if (n_BE[0] & 0x80) {
      F->Neg (num, num); 
    }
    //     if big_endian[0] & 0x80:
    //         negative = True
    //         result = big_endian[0] & 0x7f
    else {

    }
    //     else:
    //         negative = False
    //         result = big_endian[0]

    //     for c in big_endian[1:]:
    //         result <<= 8
    //         result += c



    //     if negative:
    //         return -result
    //     else:
    //         return result
    return F->Raw (dec_out, num, false); 
  }

}



using namespace priv_op;


void bmx::test_encode_decode() {

  // encode x to put on stack 
  //bytearray& encode_num (bytearray& enc_out,  const bytearray& num_BE) {

  // decode to read thing from stack
  //af::bytearray& decode_num (af::bytearray& dec_out, const af::bytearray& n_enc) {
  
}



//
//
int bmx::proc_OP_0 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  script_stack&       s    = env.stack; 
  script_stack&       alt  = env.alts;
  const command_list& cmds = env.cmds; 

  
  s.push (af::bytearray(1,0)); 
  return 0;
}

//
//
int bmx::proc_OP_1NEGATE(script_env &env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  return 0;
}

//
//
int bmx::proc_OP_1 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  script_stack&       s    = env.stack; 
  script_stack&       alt  = env.alts;
  const command_list& cmds = env.cmds; 

  s.push (af::bytearray (1, 1)); 

	  return 0;
  }


//
//
int bmx::proc_OP_2 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  return 0;
}

int bmx::proc_OP_3 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  return 0;
}

int bmx::proc_OP_4 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  return 0;
}

int bmx::proc_OP_5 (script_env& env) {
  printf ( "I am  %s.\n" , __FUNCTION__);
  return 0;
}

int bmx::proc_OP_6 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  return 0;
}

int bmx::proc_OP_7 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  return 0;
}

int bmx::proc_OP_8 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  return 0;
}

int bmx::proc_OP_9 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  return 0;
}

int bmx::proc_OP_10 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  return 0;
}

int bmx::proc_OP_11 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  return 0;
}

int bmx::proc_OP_12 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  return 0;
}

int bmx::proc_OP_13 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  return 0;
}

int bmx::proc_OP_14 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  return 0;
}

int bmx::proc_OP_15 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  return 0;
}

//
//
int bmx::proc_OP_16 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__); 
  script_stack&       s    = env.stack; 
  script_stack&       alt  = env.alts;
  const command_list& cmds = env.cmds; 

  return 0;
}



//
//
int bmx::proc_OP_NOP (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__); 
  script_stack&       s    = env.stack; 
  script_stack&       alt  = env.alts;
  const command_list& cmds = env.cmds; 

   return 0;
 }          


int bmx::proc_OP_IF (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }
int bmx::proc_OP_NOTIF (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }
int bmx::proc_OP_VERIFY (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }
int bmx::proc_OP_RETURN (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }
int bmx::proc_OP_TOALTSTACK (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }
int bmx::proc_OP_FROMALTSTACK (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }
int bmx::proc_OP_2DROP (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }
int bmx::proc_OP_2DUP (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }
int bmx::proc_OP_3DUP (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }
int bmx::proc_OP_2OVER (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }
int bmx::proc_OP_2ROT (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }
int bmx::proc_OP_2SWAP (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }
int bmx::proc_OP_IFDUP (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }
int bmx::proc_OP_DEPTH (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }
int bmx::proc_OP_DROP (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }

//
//
int bmx::proc_OP_DUP (script_env& env) {

  printf ( "I am  %s.\n" , __FUNCTION__);
 
  script_stack&       s    = env.stack; 
  script_stack&       alt  = env.alts;
  const command_list& cmds = env.cmds; 

  return 0;
}

 int bmx::proc_OP_NIP          (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }      
 int bmx::proc_OP_OVER         (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }      
 int bmx::proc_OP_PICK         (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }      
 int bmx::proc_OP_ROLL         (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }      
 int bmx::proc_OP_ROT          (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }      
 int bmx::proc_OP_SWAP         (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }      
 int bmx::proc_OP_TUCK         (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }      
 int bmx::proc_OP_SIZE         (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }      
 int bmx::proc_OP_EQUAL        (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }      
 int bmx::proc_OP_EQUALVERIFY  (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }      
 int bmx::proc_OP_1ADD         (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }      
 int bmx::proc_OP_1SUB         (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }      
 int bmx::proc_OP_NEGATE       (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }      
 int bmx::proc_OP_ABS          (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }      
 int bmx::proc_OP_NOT          (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }  
 int bmx::proc_OP_0NOTEQUAL    (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }  
//
//
int bmx::proc_OP_ADD (script_env& env) {

  printf ( "I am  %s.\n" , __FUNCTION__); 
  
  script_stack&       s    = env.stack; 
  script_stack&       alt  = env.alts;
  const command_list& cmds = env.cmds; 

  return 0;
}

 int bmx::proc_OP_SUB               (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }  
 int bmx::proc_OP_MUL               (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }  
 int bmx::proc_OP_BOOLAND           (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }  
 int bmx::proc_OP_BOOLOR            (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }  
 int bmx::proc_OP_NUMEQUAL          (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }  
 int bmx::proc_OP_NUMEQUALVERIFY    (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }  
 int bmx::proc_OP_NUMNOTEQUAL       (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }  
 int bmx::proc_OP_LESSTHAN          (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }  
 int bmx::proc_OP_GREATERTHAN       (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }  
 int bmx::proc_OP_LESSTHANOREQUAL   (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }  
 int bmx::proc_OP_GREATERTHANOREQUAL(script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; } 
 int bmx::proc_OP_MIN               (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }  
 int bmx::proc_OP_MAX               (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }  
 int bmx::proc_OP_WITHIN            (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }  
 int bmx::proc_OP_RIPEMD160         (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }  
 int bmx::proc_OP_SHA1              (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }  
 int bmx::proc_OP_SHA256            (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }  

//
//
int bmx::proc_OP_HASH160 (script_env& env) {
  //printf ( "I am  %s...restore plz\n" , __FUNCTION__); 
  
  script_stack&       s    = env.stack; 

  if (s.empty ())
    return __LINE__; 

  const af::bytearray& top_el  = s.top ();
  
  af::digest20 dig20;
  af::hash160 (dig20, &top_el[0],  top_el.size ());
  
  s.pop ();
  s.push (af::bytearray  (dig20.begin(), dig20.end()) ); 
  
  return 0;

}

int bmx::proc_OP_HASH256 (script_env& env) {
printf ( "I am  %s.\n" , __FUNCTION__);
 return 0;
} 

//
//
int bmx::proc_OP_CHECKSIG (script_env& env) {

  printf ( "I am  %s.\n" , __FUNCTION__); 

  script_stack&       s    = env.stack; 
  script_stack&       alt  = env.alts;
  const command_list& cmds = env.cmds; 


  return 0;

}

int bmx::proc_OP_CHECKSIGVERIFY      (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }
int bmx::proc_OP_CHECKMULTISIG       (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }
int bmx::proc_OP_CHECKMULTISIGVERIFY (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }

// int bmx::proc_OP_NOP                 (script_env& env) { return 0; }
int bmx::proc_OP_CHECKLOCKTIMEVERIFY (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }
int bmx::proc_OP_CHECKSEQUENCEVERIFY (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }

