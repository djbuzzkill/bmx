
//
//
#include "op_fns.h"
#include "secp256k1.h"

#include "aframe/af.h"
#include "ffm/ffm.h"



namespace priv_op {

  using namespace ffm;
  using namespace af; 

  //
  //  encode x to put on stack 
  bytearray& encode_num (bytearray& enc_out,  const std::string& decinum) {

    //  printf ("[%s:decinum:%s]\n", __FUNCTION__, decinum.c_str()); 
    
    enc_out.clear ();

    // only valid strings
    assert ( !decinum.empty() ); 
    assert ( decinum != "-0"); 
    assert ( decinum != "+0"); 

    if (decinum.size() == 1 && decinum[0] == '0') { 
      return enc_out;
    }

    FEConRef F  (nullptr);

    if (!bmx::Init_FE_context (F)) { 
      // wtf some sH#$ happend
      return enc_out; 
    }


    FE_t num = F->New (decinum.c_str(), 10);

    const bool true_this_time = true; 
    bytearray n_raw; 
    int sign_n = 0;
    F->Raw (n_raw, sign_n, num , true_this_time);  // false == BE?

    enc_out = n_raw; 

    //printf ("[size(enc_out):%zu|back:%i]\n", enc_out.size(), enc_out.back ()); 
    if (enc_out.back () & 0x80 ) {
      if (sign_n < 0) {
	enc_out.push_back (0x80);
	//printf ("enc_out.push_back(0x80)\n"); 
      }
      else {
       enc_out.push_back (0x0);
       //printf ("enc_out.push_back(0x0)\n"); 
      }

    }
    else if (sign_n < 0) {
      enc_out.back () |= 0x80; 
      //printf ("enc_out |= 0x80\n"); 
    }

    return enc_out; 
      
    }

  
  //
  // decode to read thing from stack
  std::string& decode_num (std::string& dec_out, const af::bytearray& n_enc) {

    dec_out = "";
    
    if (n_enc.empty ()) {
      dec_out += '0'; 
      return dec_out;
    }
    // def decode_num(element):
    //     if element == b'':
    //         return 0
    FEConRef F  (nullptr);
    if (!bmx::Init_FE_context (F)) { 
      return dec_out; 
    }

    af::bytearray n_BE = n_enc;

    // end_byte knows
    auto end_byte = n_BE.back ();
    
    if (end_byte & 0x80) {
      n_BE.back () =  end_byte & 0x7f;
    }

    const bool true_here_too = true; 
    FE_t num = F->New_bin (&n_BE[0], n_BE.size(), true_here_too);
    //F->SAdd_ui (num, num,  (end_byte & 0x7f) ); 
    
    if (end_byte & 0x80) {
      F->SNeg (num, num); 
    }

    std::vector<char> msg (256,'\0'); 
    dec_out = F->Format (&msg[0],  "%Zd", num);
    //printf ("%s:dec_out:[%s]\n", __FUNCTION__, dec_out.c_str()); 
    
    return dec_out; 
    
    //return F->Raw (dec_out, num, false); 
  }

  bytearray& encode_num (bytearray& enc_out, FE_t num, FEConRef F) ;
  FE_t       decode_num (FE_t num, FEConRef F, const af::bytearray& n_enc) ;
  
  bytearray& encode_num (bytearray& enc_out, FE_t num, FEConRef F) {

    //  printf ("[%s:decinum:%s]\n", __FUNCTION__, decinum.c_str()); 
    enc_out.clear ();
    // only valid strings

    const bool true_here = true; 
    int        sign_n = 0;
    bytearray  n_raw; 

    F->Raw (n_raw, sign_n, num , true_here);  // false == BE?
    enc_out = n_raw; 
    //printf ("[size(enc_out):%zu|back:%i]\n", enc_out.size(), enc_out.back ()); 
    if (enc_out.back () & 0x80 ) {
      if (sign_n < 0) {
	enc_out.push_back (0x80); // printf ("enc_out.push_back(0x80)\n"); 
      }
      else {
       enc_out.push_back (0x0); // printf ("enc_out.push_back(0x0)\n"); 
      }
    }
    else if (sign_n < 0) {
      enc_out.back () |= 0x80; // printf ("enc_out |= 0x80\n"); 
    }
    return enc_out; 
  }

  //std::string& decode_num (std::string& dec_out, const af::bytearray& n_enc) {
  FE_t decode_num (FE_t num, FEConRef F, const af::bytearray& n_enc) {

    if (n_enc.empty ()) {
      F->Set_ui (num, 0); 
      return num;
    }
    
    af::bytearray n_BE = n_enc;

    // end_byte knows
    auto end_byte = n_BE.back ();
    
    if (end_byte & 0x80) {
      n_BE.back () = end_byte & 0x7f;
    }

    const bool true_here_too = true; 
    F->Set_bin (num, &n_BE[0], n_BE.size(), true_here_too);
    
    if (end_byte & 0x80) {
      F->SNeg (num, num); 
    }

    //std::vector<char> msg (256,'\0'); 
    //printf ("%s:dec_out:[%s]\n", __FUNCTION__, F->Format (&msg[0], "%Zd", num)); 
    return num; 
  }

}



using namespace priv_op;


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



  //  encode x to put on stack
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



//
//
int bmx::proc_OP_0 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  script_stack& s = env.stack; 
  bytearray tmp;

  FEConRef F (nullptr);
  Init_FE_context (F); 
  s.push (encode_num(tmp, F->New_ui (0), F)); 
  return 0;
}

//
//
int bmx::proc_OP_1NEGATE(script_env &env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  script_stack& s = env.stack; 
  bytearray tmp; 
  FEConRef F (nullptr);
  Init_FE_context (F); 
  s.push (encode_num(tmp, F->New_si (-1), F)); 
  return 0;
}

//
//
int bmx::proc_OP_1 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  script_stack& s = env.stack; 
  bytearray tmp; 
  FEConRef F (nullptr);
  Init_FE_context (F); 
  s.push (encode_num(tmp, F->New_ui(1), F));
  return 0;
  }

//
//
int bmx::proc_OP_2 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  script_stack& s = env.stack; 
  bytearray tmp; 
  s.push (encode_num(tmp, "2")); 
  return 0;
}

int bmx::proc_OP_3 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  script_stack& s = env.stack; 
  bytearray tmp; 
  s.push (encode_num(tmp, "3")); 
  return 0;
}

int bmx::proc_OP_4 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  script_stack& s = env.stack; 
  bytearray tmp; 
  s.push (encode_num(tmp, "4")); 
  return 0;
}

int bmx::proc_OP_5 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  script_stack& s = env.stack; 
  bytearray tmp;
  s.push (encode_num(tmp, "5")); 
  return 0;
}

int bmx::proc_OP_6 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  script_stack& s = env.stack; 
  bytearray tmp; 
  s.push (encode_num(tmp, "6")); 
  return 0;
}

int bmx::proc_OP_7 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  script_stack& s = env.stack; 
  bytearray tmp; 
  s.push (encode_num(tmp, "7")); 
  return 0;
}

int bmx::proc_OP_8 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  script_stack& s = env.stack; 
  bytearray tmp; 
  s.push (encode_num(tmp, "8")); 
  return 0;
}

int bmx::proc_OP_9 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  script_stack& s = env.stack; 
  bytearray tmp; 
  s.push (encode_num(tmp, "9")); 
  return 0;
}

int bmx::proc_OP_10 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  script_stack& s = env.stack; 
  bytearray tmp; 
  s.push (encode_num(tmp, "10")); 
  return 0;
}

int bmx::proc_OP_11 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  script_stack& s = env.stack; 
  bytearray tmp; 
  s.push (encode_num(tmp, "11")); 
  return 0;
}

int bmx::proc_OP_12 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  script_stack& s = env.stack; 
  bytearray tmp; 
  s.push (encode_num(tmp, "12")); 
  return 0;
}

int bmx::proc_OP_13 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  script_stack& s = env.stack; 
  bytearray tmp; 
  s.push (encode_num(tmp, "13")); 
  return 0;
}

int bmx::proc_OP_14 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  script_stack& s = env.stack; 
  bytearray tmp; 
  s.push (encode_num(tmp, "14")); 
  return 0;
}

int bmx::proc_OP_15 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__);
  script_stack& s = env.stack; 

  bytearray tmp; 
  s.push (encode_num(tmp, "15")); 
  return 0;
}

//
//
int bmx::proc_OP_16 (script_env& env) {
  //printf ( "I am  %s.\n" , __FUNCTION__); 
  script_stack& s = env.stack; 
  bytearray tmp; 
  s.push (encode_num(tmp, "16")); 
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
  const digest32&     z    = env.z;
  
  if (s.size () < 2)
    return false; 

  //size_t ReadSignature_DER  (Signature& out, af::ReadStreamRef rs);
  std::string sigstr ; 

  const bytearray  pubkey_bin = s.top ();
  s.pop ();
  
  const bytearray sig_bin    = s.top (); 

  // hex::encode (sigstr, &sig_bin[0], sig_bin.size()); 
  // printf ("B4%s\n", sigstr.c_str()); 

  //   hex::encode (sigstr, &sig_bin[0], sig_bin.size()); 
  //   printf ("4F%s\n", sigstr.c_str()); 
  s.pop ();

  
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
  if (bmx::VerifySignature  (sig, pubkey, z)) {
    printf ( "sig good ..[%s]\n" , __FUNCTION__); 

    s.push (encode_num (tmp, F->New_si(1), F));
  }
  else {
    printf ( "sig bad ..[%s]\n" , __FUNCTION__); 
    s.push(encode_num (tmp, F->New_si(0), F)); 
  }

  // def op_checksig(stack, z):
  //     if len(stack) < 2:
  //         return False
  //     sec_pubkey = stack.pop()
  //     der_signature = stack.pop()[:-1]
  //     try:
  //         point = S256Point.parse(sec_pubkey)
  //         sig = Signature.parse(der_signature)
  //     except (ValueError, SyntaxError) as e:
  //         return False
  //     if point.verify(z, sig):
  //         stack.append(encode_num(1))
  //     else:
  //         stack.append(encode_num(0))
  //     return True
  printf ( "Exiting..[%s]\n" , __FUNCTION__); 
  return true;

}

int bmx::proc_OP_CHECKSIGVERIFY      (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }
int bmx::proc_OP_CHECKMULTISIG       (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }
int bmx::proc_OP_CHECKMULTISIGVERIFY (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }

// int bmx::proc_OP_NOP                 (script_env& env) { return 0; }
int bmx::proc_OP_CHECKLOCKTIMEVERIFY (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }
int bmx::proc_OP_CHECKSEQUENCEVERIFY (script_env& env) { printf ( "I am  %s.\n" , __FUNCTION__); return 0; }

