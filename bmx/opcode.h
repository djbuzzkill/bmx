
#ifndef CURV_OPCODE_H
#define CURV_OPCODE_H

namespace bmx {

  //
  enum OpCode : unsigned char {

    OP_0                   =   0, // 0x00
    OP_PUSH1               =  76, // 0x4c
    OP_PUSH2               =  77, // 0x4d
    OP_PUSH4               =  78, // 0x4e
    OP_1NEGATE             =  79, // 0x4f
    //__unused_0x50        =  80, // 0x50
    OP_1                   =  81, // 0x51 
    OP_2                   =  82, // 0x52 
    OP_3                   =  83, // 0x53 
    OP_4                   =  84, // 0x54 
    OP_5                   =  85, // 0x55 
    OP_6                   =  86, // 0x56 
    OP_7                   =  87, // 0x57 
    OP_8                   =  88, // 0x58 
    OP_9                   =  89, // 0x59 
    OP_10                  =  90, // 0x5a 
    OP_11                  =  91, // 0x5b 
    OP_12                  =  92, // 0x5c 
    OP_13                  =  93, // 0x5d 
    OP_14                  =  94, // 0x5e 
    OP_15                  =  95, // 0x5f 
    OP_16                  =  96, // 0x60 
    OP_NOP                 =  97, // 0x61 
    //__unused_0x62        =  98, // 0x62
    OP_IF                  =  99, // 0x63 
    OP_NOTIF               = 100, // 0x64 
    //__unused_0x65        = 101, // 0x65
    //__unused_0x66        = 102, // 0x66
    //__unused_0x67        = 103, // 0x67
    //__unused_0x68        = 104, // 0x68
    OP_VERIFY              = 105, // 0x69 
    OP_RETURN              = 106, // 0x6a 
    OP_TOALTSTACK          = 107, // 0x6b 
    OP_FROMALTSTACK        = 108, // 0x6c 
    OP_2DROP               = 109, // 0x6d 
    OP_2DUP                = 110, // 0x6e 
    OP_3DUP                = 111, // 0x6f 
    OP_2OVER               = 112, // 0x70 
    OP_2ROT                = 113, // 0x71 
    OP_2SWAP               = 114, // 0x72 
    OP_IFDUP               = 115, // 0x73 
    OP_DEPTH               = 116, // 0x74 
    OP_DROP                = 117, // 0x75 
    OP_DUP                 = 118, // 0x76 
    OP_NIP                 = 119, // 0x77 
    OP_OVER                = 120, // 0x78 
    OP_PICK                = 121, // 0x79 
    OP_ROLL                = 122, // 0x7a 
    OP_ROT                 = 123, // 0x7b 
    OP_SWAP                = 124, // 0x7c 
    OP_TUCK                = 125, // 0x7d
    //__unused_0x7e        = 126, // 0x7e
    //__unused_0x7f        = 127, // 0x7f
    //__unused_0x80        = 128, // 0x80
    //__unused_2x81        = 129, // 0x81
    OP_SIZE                = 130, // 0x82
    //__unused_131         = 131, // 0x83
    //__unused_132         = 132, // 0x84
    //__unused_133         = 133, // 0x85
    //__unused_134         = 134, // 0x86
    OP_EQUAL               = 135, // 0x87 
    OP_EQUALVERIFY         = 136, // 0x88 
    OP_1ADD                = 139, // 0x8b 
    OP_1SUB                = 140, // 0x8c 
    OP_NEGATE              = 143, // 0x8f 
    OP_ABS                 = 144, // 0x90 
    OP_NOT                 = 145, // 0x91 
    OP_0NOTEQUAL           = 146, // 0x92 
    OP_ADD                 = 147, // 0x93 
    OP_SUB                 = 148, // 0x94 
    OP_MUL                 = 149, // 0x95

    OP_BOOLAND             = 154, // 0x9a 
    OP_BOOLOR              = 155, // 0x9b 
    OP_NUMEQUAL            = 156, // 0x9c 
    OP_NUMEQUALVERIFY      = 157, // 0x9d 
    OP_NUMNOTEQUAL         = 158, // 0x9e 
    OP_LESSTHAN            = 159, // 0x9f 
    OP_GREATERTHAN         = 160, // 0xa0 
    OP_LESSTHANOREQUAL     = 161, // 0xa1 
    OP_GREATERTHANOREQUAL  = 162, // 0xa2 
    OP_MIN                 = 163, // 0xa3 
    OP_MAX                 = 164, // 0xa4 
    OP_WITHIN              = 165, // 0xa5 
    OP_RIPEMD160           = 166, // 0xa6 
    OP_SHA1                = 167, // 0xa7 
    OP_SHA256              = 168, // 0xa8 
    OP_HASH160             = 169, // 0xa9 
    OP_HASH256             = 170, // 0xaa
    //__unused_0xab        = 
    OP_CHECKSIG            = 172, // 0xac 
    OP_CHECKSIGVERIFY      = 173, // 0xad 
    OP_CHECKMULTISIG       = 174, // 0xae 
    OP_CHECKMULTISIGVERIFY = 175, // 0xaf 
    OP_NOP_0xb0            = 176, // 0xb0 
    OP_CHECKLOCKTIMEVERIFY = 177, // 0xb1 
    OP_CHECKSEQUENCEVERIFY = 178, // 0xb2
    OP_NOP_0xb3            = 179, // 0xb3
    OP_NOP_0xb4            = 180, // 0xb4
    OP_NOP_0xb5            = 181, // 0xb5  
    OP_NOP_0xb6            = 182, // 0xb6  
    OP_NOP_0xb7            = 183, // 0xb7  
    OP_NOP_0xb8            = 184, // 0xb8  
    OP_NOP_0xb9            = 185, // 0xb9
  };				  

};
#endif
