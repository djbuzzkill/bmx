

#include "script.h"
#include "op_fns.h"
#include "script_types.h"
#include "aframe/utility.h"


using namespace af;

  //

namespace priveval {
  
  using namespace bmx; 

  typedef std::function<int(bmx::script_env&)>  op_proc;
  typedef std::map<bmx::OpCode, op_proc>        OpFnMap;
  //
  //
  static OpFnMap op_map = {

    { OP_0                   , proc_OP_0                   },  
    { OP_1NEGATE             , proc_OP_1NEGATE             },  
    { OP_1                   , proc_OP_1                   },  
    { OP_2                   , proc_OP_2                   },  
    { OP_3                   , proc_OP_3                   },  
    { OP_4                   , proc_OP_4                   },  
    { OP_5                   , proc_OP_5                   },  
    { OP_6                   , proc_OP_6                   },  
    { OP_7                   , proc_OP_7                   },  
    { OP_8                   , proc_OP_8                   },  
    { OP_9                   , proc_OP_9                   },  
    { OP_10                  , proc_OP_10                  },  
    { OP_11                  , proc_OP_11                  },  
    { OP_12                  , proc_OP_12                  },  
    { OP_13                  , proc_OP_13                  },  
    { OP_14                  , proc_OP_14                  },  
    { OP_15                  , proc_OP_15                  },  
    { OP_16                  , proc_OP_16                  },  
    { OP_NOP                 , proc_OP_NOP                 },  
    //
    { OP_IF                  , proc_OP_IF                  },  
    { OP_NOTIF               , proc_OP_NOTIF               },  
    //
    //
    //
    { OP_VERIFY              , proc_OP_VERIFY              },  
    { OP_RETURN              , proc_OP_RETURN              },  
    { OP_TOALTSTACK          , proc_OP_TOALTSTACK          },  
    { OP_FROMALTSTACK        , proc_OP_FROMALTSTACK        },  
    { OP_2DROP               , proc_OP_2DROP               },  
    { OP_2DUP                , proc_OP_2DUP                },  
    { OP_3DUP                , proc_OP_3DUP                },  
    { OP_2OVER               , proc_OP_2OVER               },  
    { OP_2ROT                , proc_OP_2ROT                },  
    { OP_2SWAP               , proc_OP_2SWAP               },  
    { OP_IFDUP               , proc_OP_IFDUP               },  
    { OP_DEPTH               , proc_OP_DEPTH               },  
    { OP_DROP                , proc_OP_DROP                },  
    { OP_DUP                 , proc_OP_DUP                 },  
    { OP_NIP                 , proc_OP_NIP                 },  
    { OP_OVER                , proc_OP_OVER                },  
    { OP_PICK                , proc_OP_PICK                },  
    { OP_ROLL                , proc_OP_ROLL                },  
    { OP_ROT                 , proc_OP_ROT                 },  
    { OP_SWAP                , proc_OP_SWAP                },  
    { OP_TUCK                , proc_OP_TUCK                },  
    //
    { OP_SIZE                , proc_OP_SIZE                },  
    //
    { OP_EQUAL               , proc_OP_EQUAL               },  
    { OP_EQUALVERIFY         , proc_OP_EQUALVERIFY         },  
    { OP_1ADD                , proc_OP_1ADD                },  
    { OP_1SUB                , proc_OP_1SUB                },  
    { OP_NEGATE              , proc_OP_NEGATE              },  
    { OP_ABS                 , proc_OP_ABS                 },  
    { OP_NOT                 , proc_OP_NOT                 },  
    { OP_0NOTEQUAL           , proc_OP_0NOTEQUAL           },  
    { OP_ADD                 , proc_OP_ADD                 },  
    { OP_SUB                 , proc_OP_SUB                 },  
    { OP_MUL                 , proc_OP_MUL                 },  
    // 
    { OP_BOOLAND             , proc_OP_BOOLAND             },  
    { OP_BOOLOR              , proc_OP_BOOLOR              },  
    { OP_NUMEQUAL            , proc_OP_NUMEQUAL            },  
    { OP_NUMEQUALVERIFY      , proc_OP_NUMEQUALVERIFY      },  
    { OP_NUMNOTEQUAL         , proc_OP_NUMNOTEQUAL         },  
    { OP_LESSTHAN            , proc_OP_LESSTHAN            },  
    { OP_GREATERTHAN         , proc_OP_GREATERTHAN         },  
    { OP_LESSTHANOREQUAL     , proc_OP_LESSTHANOREQUAL     },  
    { OP_GREATERTHANOREQUAL  , proc_OP_GREATERTHANOREQUAL  },  
    { OP_MIN                 , proc_OP_MIN                 },  
    { OP_MAX                 , proc_OP_MAX                 },  
    { OP_WITHIN              , proc_OP_WITHIN              },  
    { OP_RIPEMD160           , proc_OP_RIPEMD160           },  
    { OP_SHA1                , proc_OP_SHA1                },  
    { OP_SHA256              , proc_OP_SHA256              },  
    { OP_HASH160             , proc_OP_HASH160             },  
    { OP_HASH256             , proc_OP_HASH256             },  
    //
    { OP_CHECKSIG            , proc_OP_CHECKSIG            },  
    { OP_CHECKSIGVERIFY      , proc_OP_CHECKSIGVERIFY      },  
    { OP_CHECKMULTISIG       , proc_OP_CHECKMULTISIG       },  
    { OP_CHECKMULTISIGVERIFY , proc_OP_CHECKMULTISIGVERIFY },  
    { OP_NOP_0xb0            , proc_OP_NOP                 },  
    { OP_CHECKLOCKTIMEVERIFY , proc_OP_CHECKLOCKTIMEVERIFY },  
    { OP_CHECKSEQUENCEVERIFY , proc_OP_CHECKSEQUENCEVERIFY },  
    { OP_NOP_0xb3            , proc_OP_NOP                 },  
    { OP_NOP_0xb4            , proc_OP_NOP                 },  
    { OP_NOP_0xb5            , proc_OP_NOP                 },  
    { OP_NOP_0xb6            , proc_OP_NOP                 },  
    { OP_NOP_0xb7            , proc_OP_NOP                 },  
    { OP_NOP_0xb8            , proc_OP_NOP                 },  
    { OP_NOP_0xb9            , proc_OP_NOP                 },
  };  	                    


  //
  //
  typedef std::map<bmx::OpCode, std::string>  OpNameMap;
  //
  static OpNameMap op_name = {
    { OP_0                   , "OP_0"                   },  
    { OP_1NEGATE             , "OP_1NEGATE"             },  
    { OP_1                   , "OP_1"                   },  
    { OP_2                   , "OP_2"                   },  
    { OP_3                   , "OP_3"                   },  
    { OP_4                   , "OP_4"                   },  
    { OP_5                   , "OP_5"                   },  
    { OP_6                   , "OP_6"                   },  
    { OP_7                   , "OP_7"                   },  
    { OP_8                   , "OP_8"                   },  
    { OP_9                   , "OP_9"                   },  
    { OP_10                  , "OP_10"                  },  
    { OP_11                  , "OP_11"                  },  
    { OP_12                  , "OP_12"                  },  
    { OP_13                  , "OP_13"                  },  
    { OP_14                  , "OP_14"                  },  
    { OP_15                  , "OP_15"                  },  
    { OP_16                  , "OP_16"                  },  
    { OP_NOP                 , "OP_NOP"                 },  
    { OP_IF                  , "OP_IF"                  },  
    { OP_NOTIF               , "OP_NOTIF"               },  
    { OP_VERIFY              , "OP_VERIFY"              },  
    { OP_RETURN              , "OP_RETURN"              },  
    { OP_TOALTSTACK          , "OP_TOALTSTACK"          },  
    { OP_FROMALTSTACK        , "OP_FROMALTSTACK"        },  
    { OP_2DROP               , "OP_2DROP"               },  
    { OP_2DUP                , "OP_2DUP"                },  
    { OP_3DUP                , "OP_3DUP"                },  
    { OP_2OVER               , "OP_2OVER"               },  
    { OP_2ROT                , "OP_2ROT"                },  
    { OP_2SWAP               , "OP_2SWAP"               },  
    { OP_IFDUP               , "OP_IFDUP"               },  
    { OP_DEPTH               , "OP_DEPTH"               },  
    { OP_DROP                , "OP_DROP"                },  
    { OP_DUP                 , "OP_DUP"                 },  
    { OP_NIP                 , "OP_NIP"                 },  
    { OP_OVER                , "OP_OVER"                },  
    { OP_PICK                , "OP_PICK"                },  
    { OP_ROLL                , "OP_ROLL"                },  
    { OP_ROT                 , "OP_ROT"                 },  
    { OP_SWAP                , "OP_SWAP"                },  
    { OP_TUCK                , "OP_TUCK"                },  
    { OP_SIZE                , "OP_SIZE"                },  
    { OP_EQUAL               , "OP_EQUAL"               },  
    { OP_EQUALVERIFY         , "OP_EQUALVERIFY"         },  
    { OP_1ADD                , "OP_1ADD"                },  
    { OP_1SUB                , "OP_1SUB"                },  
    { OP_NEGATE              , "OP_NEGATE"              },  
    { OP_ABS                 , "OP_ABS"                 },  
    { OP_NOT                 , "OP_NOT"                 },  
    { OP_0NOTEQUAL           , "OP_0NOTEQUAL"           },  
    { OP_ADD                 , "OP_ADD"                 },  
    { OP_SUB                 , "OP_SUB"                 },  
    { OP_MUL                 , "OP_MUL"                 },  
    { OP_BOOLAND             , "OP_BOOLAND"             },  
    { OP_BOOLOR              , "OP_BOOLOR"              },  
    { OP_NUMEQUAL            , "OP_NUMEQUAL"            },  
    { OP_NUMEQUALVERIFY      , "OP_NUMEQUALVERIFY"      },  
    { OP_NUMNOTEQUAL         , "OP_NUMNOTEQUAL"         },  
    { OP_LESSTHAN            , "OP_LESSTHAN"            },  
    { OP_GREATERTHAN         , "OP_GREATERTHAN"         },  
    { OP_LESSTHANOREQUAL     , "OP_LESSTHANOREQUAL"     },  
    { OP_GREATERTHANOREQUAL  , "OP_GREATERTHANOREQUAL"  },  
    { OP_MIN                 , "OP_MIN"                 },  
    { OP_MAX                 , "OP_MAX"                 },  
    { OP_WITHIN              , "OP_WITHIN"              },  
    { OP_RIPEMD160           , "OP_RIPEMD160"           },  
    { OP_SHA1                , "OP_SHA1"                },  
    { OP_SHA256              , "OP_SHA256"              },  
    { OP_HASH160             , "OP_HASH160"             },  
    { OP_HASH256             , "OP_HASH256"             },  
    { OP_CHECKSIG            , "OP_CHECKSIG"            },  
    { OP_CHECKSIGVERIFY      , "OP_CHECKSIGVERIFY"      },  
    { OP_CHECKMULTISIG       , "OP_CHECKMULTISIG"       },  
    { OP_CHECKMULTISIGVERIFY , "OP_CHECKMULTISIGVERIFY" },  
    { OP_NOP_0xb0            , "OP_NOP"                 },  
    { OP_CHECKLOCKTIMEVERIFY , "OP_CHECKLOCKTIMEVERIFY" },  
    { OP_CHECKSEQUENCEVERIFY , "OP_CHECKSEQUENCEVERIFY" },  
    { OP_NOP_0xb3            , "OP_NOP"                 },  
    { OP_NOP_0xb4            , "OP_NOP"                 },  
    { OP_NOP_0xb5            , "OP_NOP"                 },  
    { OP_NOP_0xb6            , "OP_NOP"                 },  
    { OP_NOP_0xb7            , "OP_NOP"                 },  
    { OP_NOP_0xb8            , "OP_NOP"                 },  
    { OP_NOP_0xb9            , "OP_NOP"                 },
  };  
}


//
//
//bool bmx::EvalScript (const command_list& commands, const af::digest32& z) {
bool bmx::EvalScript (script_env& env) {
  //
  using namespace priveval;
  //printf ("Enter:%s|ln: %i", __FUNCTION__, __LINE__ + 1);
  //script_env env; // env (commands, z, F); 

  //std::copy (z.begin(), z.end(),  env.z.begin());
  //std::copy (commands.begin(), commands.end(), std::back_inserter(env.cmds)); 

  //printf ("env.cmds.size():%zu\n",  env.cmds.size ()); 
  while (env.cmds.size ()) {
    // printf ("loop: %zu commands left\n", env.cmds.size ()); 
    const script_command cmd = std::move (env.cmds.front ()); 

    env.cmds.pop_front ();

    switch (ty (cmd)) {
      // SC_operation 
    case command_type::SC_operation:
      
      if (op_map.count (op(cmd))) {
	//assert (cmd.bin.size () == 1); 
	//printf ("Op val : %x\n", op(cmd));
	//printf ("cmd.bin[0]:%i\n", cmd.bin[0] ); 
	//printf ("operation :%s\n", op_name[op(cmd)].c_str());
	if ( !op_map[op(cmd)] (env) ) {
	  // wtf
	  //printf ("op:%s return false\n", op_name[op(cmd)].c_str ()); 
	  //printf ("return false:%i", __LINE__ + 1);
	  return false;
	}
	else {
	  printf ("%s returned true\n", op_name[op(cmd)].c_str ());
	}
      }
      else {
	//printf ("op should not be missing:%i\n", __LINE__);
	//printf ("return false:%i", __LINE__ + 1);
	return false; //  __LINE__; 
      }
      break;
      
    // SC_element:
    case command_type::SC_element:
      //printf ("element: push [%zu] \n", arr(cmd).size());
      env.stack.push_back (arr(cmd));
      break;
      // SC_uninitialized
    default:
      // wtf
      //printf ("return false:%i", __LINE__ + 1);
      return false; ////  __LINE__;
      break;

    }

  } // while 

  if (env.stack.empty ()) {
    printf ("return false:%i", __LINE__ + 1);
    return false; //  __LINE__;
   // fail
  }

  if (env.stack.back().size () == 0) {
    printf ("return false:%i", __LINE__ + 1);
    return false; //  __LINE__; 
    // empty string
  }
  //printf ("Exit:%s|ln: %i", __FUNCTION__, __LINE__ + 1);
  return true; 
}

  



int print_verify_mapping () {

  using namespace priveval;

  script_env env; 
  printf ("%s()\n\n ", __FUNCTION__); 
	      
  for (OpNameMap::const_iterator it = op_name.begin (); it != op_name.end (); ++it) {


    if (0 == op_map.count (it->first)) {
      printf ("!![%s] not in map", it->second.c_str()); 
      assert (0); 
    }
    else {

    printf ("  %s[%x]in map[%s]..calling: ",
	    it->second.c_str(),
	    (unsigned char)(it->first),
	    op_map.count (it->first) ? "Yes" : "No" );
    op_map[it->first] (env); 
    }
    
    //OpFnMap::const_iterator 

  }


  return 0; 
}
