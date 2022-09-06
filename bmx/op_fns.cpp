
//
//
#include "op_fns.h"
#include "utility.h"
#include "secp256k1.h"

#include "aframe/af.h"
#include "ffm/ffm.h"


using namespace ffm;
using namespace af; 

//
using bmx::util::encode_num;
using bmx::util::decode_num;

//
bool bmx::proc_OP_0 (script_env& env) {
  //printf ( "I AM  %s.\n" , __FUNCTION__);
  bytearray tmp;

  FEConRef F = env.ffme.F; 
  env.stack.push_back (encode_num(tmp, F->New_si (0), F)); 
  return true;
}

// bool bmx::proc_OP_PUSH1 (script_env &env) {  printf ( "I AM  %s.\n" , __FUNCTION__); return false; } 
// bool bmx::proc_OP_PUSH2 (script_env &env) {  printf ( "I AM  %s.\n" , __FUNCTION__); return false; } 
// bool bmx::proc_OP_PUSH4 (script_env &env) {  printf ( "I AM  %s.\n" , __FUNCTION__); return false; } 

//
//
bool bmx::proc_OP_1NEGATE(script_env &env) {
  //printf ( "I AM  %s.\n" , __FUNCTION__);
  bytearray tmp;
  FEConRef F = env.ffme.F; 
  env.stack.push_back (encode_num(tmp, F->New_si (-1), F)); 
  return true;
}

//
//
bool bmx::proc_OP_1 (script_env& env) {
  //printf ( "I AM  %s.\n" , __FUNCTION__);
  bytearray tmp;
  FEConRef F = env.ffme.F; 
  env.stack.push_back (encode_num(tmp, F->New_si(1), F));
  return true;
  }

//
//
bool bmx::proc_OP_2 (script_env& env) {
  //printf ( "I AM  %s.\n" , __FUNCTION__);
  bytearray tmp; 
  FEConRef F = env.ffme.F; 
  env.stack.push_back (encode_num(tmp, F->New_ui(2), F)); 
  return true;
}

bool bmx::proc_OP_3 (script_env& env) {
  //printf ( "I AM  %s.\n" , __FUNCTION__);
  bytearray tmp; 
  FEConRef F = env.ffme.F; 
  env.stack.push_back (encode_num(tmp, F->New_si(3), F)); 
  return true;
}

//
//
bool bmx::proc_OP_4 (script_env& env) {
  //printf ( "I AM  %s.\n" , __FUNCTION__);
  bytearray tmp; 
  FEConRef F = env.ffme.F; 
  env.stack.push_back (encode_num(tmp, F->New_si(4), F)); 
  return true;
}

//
//
bool bmx::proc_OP_5 (script_env& env) {
  //printf ( "I AM  %s.\n" , __FUNCTION__);
  bytearray tmp;
  FEConRef F = env.ffme.F; 
  env.stack.push_back (encode_num(tmp, F->New_si(5), F)); 
  return true;
}

//
//
bool bmx::proc_OP_6 (script_env& env) {
  //printf ( "I AM  %s.\n" , __FUNCTION__);
  bytearray tmp; 
  FEConRef F = env.ffme.F; 
  env.stack.push_back (encode_num(tmp, F->New_si(6), F)); 
  return true;
}

//
bool bmx::proc_OP_7 (script_env& env) {
  //printf ( "I AM  %s.\n" , __FUNCTION__);
  bytearray tmp; 
  FEConRef F = env.ffme.F; 
  env.stack.push_back (encode_num(tmp, F->New_si(7), F)); 
  return true;
}

//
bool bmx::proc_OP_8 (script_env& env) {
  //printf ( "I AM  %s.\n" , __FUNCTION__);
  bytearray tmp; 
  FEConRef F = env.ffme.F; 
  env.stack.push_back (encode_num(tmp, F->New_si(8), F)); 
  return true;
}

//
bool bmx::proc_OP_9 (script_env& env) {
  //printf ( "I AM  %s.\n" , __FUNCTION__);
  bytearray tmp; 
  FEConRef F = env.ffme.F; 
  env.stack.push_back (encode_num(tmp, F->New_si(9), F)); 
  return true;
}

//
bool bmx::proc_OP_10 (script_env& env) {
  //printf ( "I AM  %s.\n" , __FUNCTION__);
  FEConRef F = env.ffme.F; 
  bytearray tmp; 
  env.stack.push_back (encode_num(tmp, F->New_si(10), F)); 
  return true;
}

//
//
bool bmx::proc_OP_11 (script_env& env) {
  //printf ( "I AM  %s.\n" , __FUNCTION__);
  bytearray tmp; 
  FEConRef F = env.ffme.F; 
  env.stack.push_back (encode_num(tmp, F->New_si(11), F)); 
  return true;
}

//
//
bool bmx::proc_OP_12 (script_env& env) {
  //printf ( "I AM  %s.\n" , __FUNCTION__);
  bytearray tmp; 
  FEConRef F = env.ffme.F; 
  env.stack.push_back (encode_num(tmp, F->New_si(12), F)); 
  return true;
}

//
//
bool bmx::proc_OP_13 (script_env& env) {
  //printf ( "I AM  %s.\n" , __FUNCTION__);
  bytearray tmp; 
  FEConRef F = env.ffme.F; 
  env.stack.push_back (encode_num(tmp, F->New_si(13), F)); 
  return true;
}

//
//
bool bmx::proc_OP_14 (script_env& env) {
  //printf ( "I AM  %s.\n" , __FUNCTION__);
  bytearray tmp; 
  FEConRef F = env.ffme.F; 
  env.stack.push_back (encode_num(tmp, F->New_si(14), F)); 
  return true;
}

//
//
bool bmx::proc_OP_15 (script_env& env) {
  //printf ( "I AM  %s.\n" , __FUNCTION__);
  bytearray tmp; 
  FEConRef F = env.ffme.F; 
  env.stack.push_back (encode_num(tmp, F->New_si(15), F)); 
  return true;
}

//
//
bool bmx::proc_OP_16 (script_env& env) {
  //printf ( "I AM  %s.\n" , __FUNCTION__); 
  bytearray tmp; 
  FEConRef F = env.ffme.F; 
  env.stack.push_back (encode_num(tmp, F->New_si(16), F)); 
  return true;
}

//
//
bool bmx::proc_OP_NOP (script_env& env) {
  printf ( "I AM %s.\n" , __FUNCTION__); 
  return true;
}          

//
//
bool bmx::proc_OP_IF (script_env& env) {
  // https://en.bitcoin.it/wiki/Script
  printf ( "I AM  %s.\n" , __FUNCTION__);
  return false;

}

//
//
bool bmx::proc_OP_NOTIF (script_env& env) {
  printf ( "I AM  %s.\n" , __FUNCTION__);
  return false;
}


//
//
bool bmx::proc_OP_VERIFY (script_env& env) {

  printf ( "I AM  %s.\n" , __FUNCTION__); 

  if (env.stack.size () < 1) {
    return false; 
  }

  FEConRef F = env.ffme.F; 
  ScopeDeleter dr (F); 

  const bytearray el = std::move (env.stack.back ()); 
  env.stack.pop_back();
  
  if (0 == F->Cmp_ui (decode_num (dr(F->New()), F, el), 0)) {
    return false; 
  }

  return true;
}

//
//
bool bmx::proc_OP_RETURN (script_env& env) {
  printf ( "I AM  %s.\n" , __FUNCTION__); return 0;
  return false; 

}

//
//
bool bmx::proc_OP_TOALTSTACK (script_env& env){
  printf ( "I AM  %s.\n" , __FUNCTION__);

  if (env.stack.empty()) {
    return false;
  }
    
  env.alts.push_back (env.stack.back()); 
  env.stack.pop_back (); 
  return true;
}

//
//
bool bmx::proc_OP_FROMALTSTACK (script_env& env) {
  printf ( "I AM  %s.\n" , __FUNCTION__);

  if (env.alts.empty ())
    return false;

  env.stack.push_back (env.alts.back ());
  env.alts.pop_back();
  
  return true;
}

//
//
bool bmx::proc_OP_2DROP (script_env& env) {
  printf ( "I AM  %s.\n" , __FUNCTION__);

  if (env.stack.size () < 2)
    return false;

  env.stack.pop_back(); 
  env.stack.pop_back();
  
  return true;
}

//
//
bool bmx::proc_OP_2DUP (script_env& env) {
  printf ( "I AM  %s.\n" , __FUNCTION__);

  size_t stacksize = env.stack.size (); 
  if (stacksize < 2)
    return false;


  const bytearray el0 = env.stack[stacksize-1]; // first thiing on top
  const bytearray el1 = env.stack[stacksize-2]; // second thing

  env.stack.push_back (el1);  // same order as orig elements
  env.stack.push_back (el0);  

  return true;
}

//
//
bool bmx::proc_OP_3DUP (script_env& env) {
  printf ( "I AM  %s.\n" , __FUNCTION__);

  size_t stacksize = env.stack.size (); 
  if (stacksize < 3)
    return false;


  const bytearray el0 = env.stack[stacksize-1]; // first thiing on top
  const bytearray el1 = env.stack[stacksize-2]; // second thing
  const bytearray el2 = env.stack[stacksize-3]; // second thing

  env.stack.push_back (el2);  // same order as orig elements
  env.stack.push_back (el1);  // same order as orig elements
  env.stack.push_back (el0);  

  return true;

}


bool bmx::proc_OP_2OVER (script_env& env)       { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }
bool bmx::proc_OP_2ROT (script_env& env)        { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }
bool bmx::proc_OP_2SWAP (script_env& env)       { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }
bool bmx::proc_OP_IFDUP (script_env& env)       { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }
//
//
bool bmx::proc_OP_DEPTH (script_env& env) {
  printf ( "I AM  %s.\n" , __FUNCTION__);

  bytearray encnum; 
  FEConRef F = env.ffme.F;
  ScopeDeleter dr (F);

  FE_t stacksize = dr (F->New_ui (env.stack.size ())); 
  env.stack.push_back (encode_num (encnum , stacksize, F));
  return true; 

}


//
//
bool bmx::proc_OP_DROP (script_env& env) {
  printf ( "I AM  %s.\n" , __FUNCTION__);

  if (env.stack.empty())
    return false; 

  env.stack.pop_back (); 

  return false;
}

//
//
bool bmx::proc_OP_DUP (script_env& env) {

  printf ( "I AM  %s.\n" , __FUNCTION__);
 
  script_stack&       s    = env.stack; 
  script_stack&       alt  = env.alts;
  const command_list& cmds = env.cmds; 

  if (env.stack.size () < 1)
    return false;

  env.stack.push_back (env.stack.back());
  
  return true;
}

//
bool bmx::proc_OP_NIP          (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }      
bool bmx::proc_OP_OVER         (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }      
bool bmx::proc_OP_PICK         (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }      
bool bmx::proc_OP_ROLL         (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }      
bool bmx::proc_OP_ROT          (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }      
bool bmx::proc_OP_SWAP         (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }      
bool bmx::proc_OP_TUCK         (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }      
bool bmx::proc_OP_SIZE         (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }      

//
//
bool bmx::proc_OP_EQUAL (script_env& env) {
  printf ( "I AM  %s.\n" , __FUNCTION__);

  if (env.stack.size () < 2)
    return false; 


  bytearray element1 = env.stack.back();
  env.stack.pop_back ();
  
  bytearray element2 = env.stack.back ();
  env.stack.pop_back (); 

  bytearray tmp; 
  FEConRef F = env.ffme.F; 
  if (eql (element1, element2))
    env.stack.push_back (encode_num(tmp, F->New_si(1), F)); 
  else
    env.stack.push_back (encode_num(tmp, F->New_si(0), F)); 
  
  return true;
}

//
bool bmx::proc_OP_EQUALVERIFY  (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }      
bool bmx::proc_OP_1ADD         (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }      
bool bmx::proc_OP_1SUB         (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }      
bool bmx::proc_OP_NEGATE       (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }      
bool bmx::proc_OP_ABS          (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }      
bool bmx::proc_OP_NOT          (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }  
bool bmx::proc_OP_0NOTEQUAL    (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }  

//
//
bool bmx::proc_OP_ADD (script_env& env) {

  printf ( "I AM  %s.\n" , __FUNCTION__); 
  
  if (env.stack.size () < 2)
    return false; 

  bytearray element1 = env.stack.back();
  env.stack.pop_back ();
  
  bytearray element2 = env.stack.back ();
  env.stack.pop_back (); 

  FEConRef F = env.ffme.F; 
  ScopeDeleter dr (F); 

  FE_t lhs = dr(F->New_bin (&element1[0], element1.size (), false));
  FE_t rhs = dr(F->New_bin (&element2[0], element2.size (), false));
  FE_t out = dr(F->New ());

  F->SAdd (out, lhs, rhs); 

  bytearray tmp;
  env.stack.push_back (encode_num (tmp, out, F)); 

  return true;
}


 bool bmx::proc_OP_SUB               (script_env& env) {
   printf ( "I AM  %s.\n" , __FUNCTION__);

  if (env.stack.size () < 2)
    return false; 

  bytearray element1 = env.stack.back();
  env.stack.pop_back ();
  
  bytearray element2 = env.stack.back ();
  env.stack.pop_back (); 

  FEConRef F = env.ffme.F; 

  ScopeDeleter dr (F); 
  FE_t lhs = dr(F->New_bin (&element1[0], element2.size (), false));
  FE_t rhs = dr(F->New_bin (&element2[0], element1.size (), false));
  FE_t out = dr(F->New ());

  //out = el2 - el1
  F->SSub (out, lhs, rhs); 

  bytearray tmp;
  env.stack.push_back (encode_num (tmp, out, F)); 

  return true;
// def op_sub(stack):
//     if len(stack) < 2:
//         return False
//     element1 = decode_num(stack.pop_back())
//     element2 = decode_num(stack.pop_back())
//     stack.append(encode_num(element2 - element1))
//     return True
 }  
 bool bmx::proc_OP_MUL               (script_env& env) {
   printf ( "I AM  %s.\n" , __FUNCTION__);

// def op_mul(stack):
//     if len(stack) < 2:
//         return False
//     element1 = decode_num(stack.pop_back())
//     element2 = decode_num(stack.pop_back())
//     stack.append(encode_num(element2 * element1))
//     return True

  if (env.stack.size () < 2)
    return false; 

  bytearray element1 = env.stack.back();
  env.stack.pop_back ();
  
  bytearray element2 = env.stack.back ();
  env.stack.pop_back (); 

  FEConRef F = env.ffme.F; 

  ScopeDeleter dr (F); 
  FE_t lhs = dr(F->New_bin (&element1[0], element2.size (), false));
  FE_t rhs = dr(F->New_bin (&element2[0], element1.size (), false));
  FE_t out = dr(F->New ());

  //out = el2 * el1
  F->SMul (out, lhs, rhs); 

  bytearray tmp;
  env.stack.push_back (encode_num (tmp, out, F)); 

  return true;
 }


 bool bmx::proc_OP_BOOLAND           (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }  
 bool bmx::proc_OP_BOOLOR            (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }  
 bool bmx::proc_OP_NUMEQUAL          (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }  
 bool bmx::proc_OP_NUMEQUALVERIFY    (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }  
 bool bmx::proc_OP_NUMNOTEQUAL       (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }  
 bool bmx::proc_OP_LESSTHAN          (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }  
 bool bmx::proc_OP_GREATERTHAN       (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }  
 bool bmx::proc_OP_LESSTHANOREQUAL   (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }  
 bool bmx::proc_OP_GREATERTHANOREQUAL(script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; } 
 bool bmx::proc_OP_MIN               (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }  
 bool bmx::proc_OP_MAX               (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }  
 bool bmx::proc_OP_WITHIN            (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }  
 bool bmx::proc_OP_RIPEMD160         (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }  
 bool bmx::proc_OP_SHA1              (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }  
 bool bmx::proc_OP_SHA256            (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }  


//
//
bool bmx::proc_OP_HASH160 (script_env& env) {
  //printf ( "I AM  %s...restore plz\n" , __FUNCTION__); 
  if (env.stack.empty ())
    return false; 

  const af::bytearray top_el  = env.stack.back ();
  env.stack.pop_back ();
  
  af::digest20 dig20;
  af::hash160 (dig20, &top_el[0],  top_el.size ());
  
  env.stack.push_back (af::bytearray  (dig20.begin(), dig20.end()) ); 
  
  return true;
}


//
bool bmx::proc_OP_HASH256 (script_env& env) {
  printf ("I AM %s.\n", __FUNCTION__);

  if (env.stack.empty ()) {
    return false;
  }

  bytearray out; 
  digest32  dig32; 

  const bytearray input = env.stack.back ();

  hash256 (dig32, &input[0], input.size ()); 
  env.stack.pop_back ();

  env.stack.push_back (copy (out, dig32)); 
  
  return true;
} 

//
//
bool bmx::proc_OP_CHECKSIG (script_env& env) {

  printf ( "I AM  %s.\n" , __FUNCTION__); 

  script_stack&       alt  = env.alts;
  const command_list& cmds = env.cmds;
  const digest32&     z    = env.z;
  
  if (env.stack.size () < 2)
    return false; 

  //size_t ReadSignature_DER  (Signature& out, af::ReadStreamRef rs);
  std::string sigstr ; 

  const bytearray pubkey_bin = env.stack.back ();
  env.stack.pop_back ();
  
  const bytearray sig_bin    = env.stack.back (); 
  env.stack.pop_back ();

  // hex::encode (sigstr, &sig_bin[0], sig_bin.size()); 
  // printf ("B4%s\n", sigstr.c_str()); 

  //   hex::encode (sigstr, &sig_bin[0], sig_bin.size()); 
  //   printf ("4F%s\n", sigstr.c_str()); 

  PublicKey pubkey; 
  size_t readlen = ReadPoint (pubkey, CreateReadMemStream (&pubkey_bin[0], pubkey_bin.size()));
  assert (pubkey_bin.size() == readlen); 

  Signature sig;
  size_t sig_size = sig_bin.size () - 1; 
  size_t read_len_sig = ReadSignature_DER (sig, sig_size , CreateReadMemStream (&sig_bin[0], sig_size)); 
  assert (read_len_sig == sig_size); 

  bytearray tmp;
  FEConRef F (nullptr);
  Init_FE_context (F); 
  if ( SECP256k1_Verify (sig, pubkey, z)) {
    //printf ( "sig good ..[%s]\n" , __FUNCTION__); 
    env.stack.push_back (encode_num (tmp, F->New_si(1), F));
  }
  else {
    printf ( "sig bad ..[%s]\n" , __FUNCTION__); 
    env.stack.push_back(encode_num (tmp, F->New_si(0), F)); 
  }

  printf ( "Exiting..[%s]\n" , __FUNCTION__); 
  return true;

}


//
//
bool bmx::proc_OP_CHECKSIGVERIFY (script_env& env) {
  printf ( "I AM  %s.\n" , __FUNCTION__);
  return proc_OP_CHECKSIG (env) && proc_OP_VERIFY (env);
}


bool bmx::proc_OP_CHECKMULTISIG       (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }
bool bmx::proc_OP_CHECKMULTISIGVERIFY (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }

// bool bmx::proc_OP_NOP                 (script_env& env) { return 0; }
bool bmx::proc_OP_CHECKLOCKTIMEVERIFY (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }
bool bmx::proc_OP_CHECKSEQUENCEVERIFY (script_env& env) { printf ( "I AM  %s.\n" , __FUNCTION__); return false; }






// nothing to see








void bmx::test_encode_decode() {

  //pt::map  pm;
  //el::map  em;
  
  FEConRef F (nullptr) ;
  Init_FE_context (F);
  ScopeDeleter dr (F);
  
  std::array<char, 128> msg;
  //bmx::Init_secp256k1_Env (F, EC, em, pm);
  bytearray A, oA, dA, rawA; 
  bytearray B, oB, dB, rawB; 

  FE_t a = dr(F->New ("-8911237",  0));
  F->Print ("a", a); 
  //FE_t a = F->New ("0x1",  0);
  std::string hexs =  ""; 
  int sign_a = 0; 

  F->Raw (rawA, sign_a, a, false);
  hex::encode (hexs, &rawA[0], rawA.size());
  printf  ("(rawA, %i):%s\n", sign_a, hexs.c_str()); 
  

  encode_num (oA, a, F); 
  hex::encode (hexs, &oA[0], oA.size());
  printf  ("hex(oA,l:%zu):%s\n", oA.size(), hexs.c_str()); 


  FE_t decA = dr(F->New_ui(0));
  int sgn_dA = 0;
  decode_num (decA, F,  oA); 
  F->Print ("decA", decA);
  F->Raw (dA, sgn_dA, decA, false); 
  hex::encode (hexs, &dA[0], dA.size());
  printf  ("hex(dA s:%i):%s\n", sgn_dA, hexs.c_str()); 

  //F->SNeg (a, a); 
  //printf ("-hexadec: %s\n", F->Format (&msg[0], "%Zx", a)); 
  //printf ("-decinte: %s\n", F->Format (&msg[0], "%Zd", a)); 

  // encode x to put on stack
  // encode_num (oA,  A);
  // hex::encode (hexs, &oA[0], oA.size());  
  // printf  ("oA:%s\n", hexs.c_str()); 

  // decode_num (dA, oA);
  // hex::encode (hexs, &dA[0], dA.size()); 
  // printf  ("dA:%s\n", hexs.c_str()); 

  //FE_t b = F->New ("-21703", 10); 

  //bytearray& encode_num (bytearray& enc_out, FE_t num, FEConRef F) ;
  
  encode_num (oB, F->New_si(-1), F);

  //FE_t decode_num (FE_t num, FEConRef F, const af::bytearray& n_enc) ;
  FE_t decB = F->New_ui(0);
  decode_num (decB, F, oB); 

  F->Print ("decB", decB); 
  // decode_num (dB, oB);
  // decode to read thing from stack
  //af::bytearray& decode_num (af::bytearray& dec_out, const af::bytearray& n_enc) {
  
}

