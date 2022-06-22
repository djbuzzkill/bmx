#ifndef SPUX_INCLUDE
#define SPUX_INCLUDE

#include <Shared.h>
#include <System.h>
#include <Runnable.h>



namespace spux
{
//
// Count number of elements in a fixed-size array.
template<typename T, size_t N>
   inline size_t ARRAY_COUNT(const T(&)[N]) { return N; }

template<typename Ty>
   struct tuplet { Ty x, y; 
   };

template<typename Ty> 
   struct triplet : public tuplet<Ty> { Ty z; 
   };

template<typename Ty>
   struct quadruplet : public triplet<Ty> { Ty w; 
   };

template<typename Ty>
   struct range  { Ty begin, end;
   };

//
//
typedef tuplet<size_t> pixpos; 

}

#endif
