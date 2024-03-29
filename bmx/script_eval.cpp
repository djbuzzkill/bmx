

#include "opcode.h"
#include "script.h"
#include "op_fns.h"
#include "script_types.h"
#include <aframe/utility.h>


#define ENABLE_EVAL_BIP0016 1


using namespace af;

//
namespace priveval {
  
  using namespace bmx; 
  //
  typedef std::function<int(bmx::script_env&)>  op_proc;
  ///
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
  //FN_SCOPE (); 
  static bool diag_print_stack = false; 
  
  using namespace priveval;

  auto pass_counter  = 0; // this is for display
  
  while (env.cmds.size ()) {
    //printf ("%s [op no.%zu]-> current stack size : %zu\n", __FUNCTION__, op_num, env.stack.size()) ;
    script_command cmd = std::move (env.cmds.front ()); 
    env.cmds.pop_front ();

    switch (ty (cmd)) {
    // -------------------------------------------------------- 
    // OPERATION
    // -------------------------------------------------------- 
    case command_type::SC_operation:
      //printf ("[[ operation(0x%x) ]] ", op(cmd)); 
      if (op_map.count (op(cmd))) {
	//printf ("Op val : %x\n", op(cmd));
	//printf ("operation :%s\n", op_name[op(cmd)].c_str());
	if ( !op_map[op(cmd)] (env) ) {
	  // wtf
	  printf (" %s -> FALSE\n", op_name[op(cmd)].c_str ()); 
	  return false;
	}
	else {
	  //printf (" %s -> TRUE\n", op_name[op(cmd)].c_str ());
	}
      }
      else {
	//printf ("op should not be missing:%i\n", __LINE__);
	//printf ("return false:%i", __LINE__ + 1);
	return false; //  __LINE__; 
      }
      break;
      
    // -------------------------------------------------------- 
    // ELEMENT
    // -------------------------------------------------------- 
    case command_type::SC_element:
      
      if (diag_print_stack)
	printf ("[[ push element[%zu] ]]\n", arr(cmd).size()); 

      env.stack.push_back (std::move (arr(cmd)));

#ifdef ENABLE_EVAL_BIP0016
      // BIP0016 check
      if(env.cmds.size () == 3) {

	command_list::const_iterator it = env.cmds.begin ();
	if (ty(*it) != command_type::SC_operation || op(*it) != OP_HASH160)
	  continue; 

	it++; 
        if (ty(*it) != command_type::SC_element || arr(*it).size () != 20)
	  continue; 

	it++; 
	if (ty(*it) != command_type::SC_operation || op(*it) != OP_EQUAL)
	  continue;

	//printf (" ************* THIS LOOKS LIKE BIP0016\n"); 
	// make a copy of top right now
	bytearray maybe_redeem = env.stack.back (); 
	
        env.cmds.pop_back ();  // OP_HASH160
	bytearray h160 = std::move(arr(env.cmds.back())); 
	env.cmds.pop_back (); 
	env.cmds.pop_back (); 

	if (!op_map[OP_HASH160](env)) 
	  return false;
	
        env.stack.push_back (std::move (h160)); // hash160
	if (!op_map[OP_EQUAL](env))
	  return false;

	if (!op_map[OP_VERIFY](env))
	  return false;
	
	uint64         writelen_redeem = 0;
        bytearray      redeembuf (maybe_redeem.size () + util::SizeOf_varint (maybe_redeem.size ()), byte(0));
	WriteStreamRef redeem_ws  = CreateWriteMemStream (&redeembuf[0], redeembuf.size());

	writelen_redeem += util::write_varint(redeem_ws, maybe_redeem.size ()); 
	writelen_redeem += redeem_ws->Write (&maybe_redeem[0], maybe_redeem.size ()); 

        command_list redeem_script; 
	uint64 readlen_redeem = ReadScript (redeem_script, CreateReadMemStream (&redeembuf[0], writelen_redeem)); 
	append (env.cmds, redeem_script); 
	
      }
#endif //  ENABLE_EVAL_BIP0016

      
      break;

    default:
      // how
      // wtf
      printf ("return false from default :%i", __LINE__ + 1);

      return false; ////  __LINE__;
      break;

    } // switch 

    if (diag_print_stack) {
      printf ("stack[%zu]\n", env.stack.size ()); 
      for (size_t ist = 0; ist < env.stack.size (); ++ist) {
	auto stack_ind = env.stack.size () - 1 - ist;
	bytearray& cur_el = env.stack[stack_ind];
	
	std::string hexs;
	hex::encode (hexs, &cur_el[0], cur_el.size()); 
	printf ("  (%zu) [%zu]%s\n",  stack_ind, cur_el.size(), hexs.c_str()); 
	
      }
    }

    pass_counter++; 
  } // eval loop
  
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




const std::string nada = ""; 
const std::string& OpName (bmx::OpCode o) {
  
  if (priveval::op_name.count (o) == 0) 
    return nada;
  
  return    priveval::op_name[o]; 

}
