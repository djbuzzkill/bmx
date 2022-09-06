
//
//
#ifndef CURV_OP_FNS_H
#define CURV_OP_FNS_H

#include "common.h"
#include "opcode.h"
#include "script_types.h"
#include "aframe/af.h"

namespace bmx {
  
  bool proc_OP_0                   (script_env& env);           
  bool proc_OP_1NEGATE             (script_env& env);          
  bool proc_OP_1                   (script_env& env);          
  bool proc_OP_2                   (script_env& env);          
  bool proc_OP_3                   (script_env& env);          
  bool proc_OP_4                   (script_env& env);          
  bool proc_OP_5                   (script_env& env);          
  bool proc_OP_6                   (script_env& env);          
  bool proc_OP_7                   (script_env& env);          
  bool proc_OP_8                   (script_env& env);          
  bool proc_OP_9                   (script_env& env);          
  bool proc_OP_10                  (script_env& env);          
  bool proc_OP_11                  (script_env& env);          
  bool proc_OP_12                  (script_env& env);          
  bool proc_OP_13                  (script_env& env);          
  bool proc_OP_14                  (script_env& env);          
  bool proc_OP_15                  (script_env& env);          
  bool proc_OP_16                  (script_env& env);          
  bool proc_OP_NOP                 (script_env& env);          
  bool proc_OP_IF                  (script_env& env);          
  bool proc_OP_NOTIF               (script_env& env);          
  bool proc_OP_VERIFY              (script_env& env);      
  bool proc_OP_RETURN              (script_env& env);      
  bool proc_OP_TOALTSTACK          (script_env& env);      
  bool proc_OP_FROMALTSTACK        (script_env& env);      
  bool proc_OP_2DROP               (script_env& env);      
  bool proc_OP_2DUP                (script_env& env);      
  bool proc_OP_3DUP                (script_env& env);      
  bool proc_OP_2OVER               (script_env& env);      
  bool proc_OP_2ROT                (script_env& env);      
  bool proc_OP_2SWAP               (script_env& env);      
  bool proc_OP_IFDUP               (script_env& env);      
  bool proc_OP_DEPTH               (script_env& env);      
  bool proc_OP_DROP                (script_env& env);      
  bool proc_OP_DUP                 (script_env& env);      
  bool proc_OP_NIP                 (script_env& env);      
  bool proc_OP_OVER                (script_env& env);      
  bool proc_OP_PICK                (script_env& env);      
  bool proc_OP_ROLL                (script_env& env);      
  bool proc_OP_ROT                 (script_env& env);      
  bool proc_OP_SWAP                (script_env& env);      
  bool proc_OP_TUCK                (script_env& env);      
  bool proc_OP_SIZE                (script_env& env);      
  bool proc_OP_EQUAL               (script_env& env);      
  bool proc_OP_EQUALVERIFY         (script_env& env);      
  bool proc_OP_1ADD                (script_env& env);      
  bool proc_OP_1SUB                (script_env& env);      
  bool proc_OP_NEGATE              (script_env& env);      
  bool proc_OP_ABS                 (script_env& env);      
  bool proc_OP_NOT                 (script_env& env);  
  bool proc_OP_0NOTEQUAL           (script_env& env);  
  bool proc_OP_ADD                 (script_env& env);  
  bool proc_OP_SUB                 (script_env& env);  
  bool proc_OP_MUL                 (script_env& env);  
  bool proc_OP_BOOLAND             (script_env& env);  
  bool proc_OP_BOOLOR              (script_env& env);  
  bool proc_OP_NUMEQUAL            (script_env& env);  
  bool proc_OP_NUMEQUALVERIFY      (script_env& env);  
  bool proc_OP_NUMNOTEQUAL         (script_env& env);  
  bool proc_OP_LESSTHAN            (script_env& env);  
  bool proc_OP_GREATERTHAN         (script_env& env);  
  bool proc_OP_LESSTHANOREQUAL     (script_env& env);  
  bool proc_OP_GREATERTHANOREQUAL  (script_env& env); 

  bool proc_OP_MIN                 (script_env& env);  
  bool proc_OP_MAX                 (script_env& env);  
  bool proc_OP_WITHIN              (script_env& env);  
  bool proc_OP_RIPEMD160           (script_env& env);  
  bool proc_OP_SHA1                (script_env& env);  
  bool proc_OP_SHA256              (script_env& env);  
  bool proc_OP_HASH160             (script_env& env);  
  bool proc_OP_HASH256             (script_env& env); 
  bool proc_OP_CHECKSIG            (script_env& env);
  bool proc_OP_CHECKSIGVERIFY      (script_env& env);
  bool proc_OP_CHECKMULTISIG       (script_env& env);
  bool proc_OP_CHECKMULTISIGVERIFY (script_env& env);
  bool proc_OP_NOP_0xb0            (script_env& env);
  bool proc_OP_CHECKLOCKTIMEVERIFY (script_env& env);
  bool proc_OP_CHECKSEQUENCEVERIFY (script_env& env);
  bool proc_OP_NOP_0xb3            (script_env& env);
  bool proc_OP_NOP_0xb4            (script_env& env);
  bool proc_OP_NOP_0xb5            (script_env& env);
  bool proc_OP_NOP_0xb6            (script_env& env);
  bool proc_OP_NOP_0xb7            (script_env& env);
  bool proc_OP_NOP_0xb8            (script_env& env);
  bool proc_OP_NOP_0xb9            (script_env& env);

  void test_encode_decode(); 

}

#endif
