
#ifndef CX_UTILITY_INCLUDE
#define CX_UTILITY_INCLUDE



#include "Common.h"




inline bool checkres (const char* str, bool res)
{
  printf ("%s:%s\n", str, res ? "TRUE" : "FALSE"); 
  return res; 
}

#endif
