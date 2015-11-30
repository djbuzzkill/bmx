#ifndef include_Utility
#define include_Utility

#include <map>
#include <list>
#include <string>
#include <vector>

#include <memory>

#include <assert.h>
#include <stdio.h>




#ifdef _DEBUG

	#ifdef _WIN32

		void _debug_break_ ();
		#define DX_DEBUG(...)   fprintf(stderr, __VA_ARGS__)
      #define DX_ASSERT(x, msg, ...) if((x) == false){fprintf(stderr,"\n\n*WTF* ------> "##msg##" ", __VA_ARGS__); assert(0);} 
		#define DX_BREAK(msg, ...) do{printf("\n\n*YO* ------> "##msg##" ", __VA_ARGS__); _debug_break_ ();}while(0)
		#define DX_BREAKASSERT(x, msg, ...) if((x) == false){printf("\n\n*BREAK* -------------> \""##msg##"\" ", __VA_ARGS__); _debug_break_ ();} 

	#else
		#include "NeedPlatformBreak.h"
	#endif

#else

	#define DX_DEBUG(...)   
	#define DX_ASSERT(x, msg, ...)
	#define DX_BREAK(msg, ...) 
	#define DX_BREAKASSERT(msg, ...) 

#endif


/** ****************************************************************
 *  
 ******************************************************************/
#define El_count(x)(sizeof(x)/sizeof(x[0]))

#define TriMax(a, b, c) (((a)>(b))?(((a)>(c))?(a):(c)):(((b)>(c))?(b):(c)))
#define TriMin(a, b, c) (((a)<(b))?(((a)<(c))?(a):(c)):(((b)<(c))?(b):(c)))

   



namespace ut
{
	typedef std::vector<unsigned char> Byte_array; 

   union ptru 
   {
      void*             v; 
      char*             c;
      unsigned char*    uc;
      short*            s;
      unsigned short*   us;
      int*              i;
      unsigned*         ui;
      float*            f;
      double*           d;
   }; 


	//
	size_t SizeOf_file (
		const std::string& fname
		); 

	// 
	size_t Load_data_file (
		ut::Byte_array&		data_File, 
		const std::string&	fname
		); 

   size_t Load_text_file (
	   std::vector<char>&   txtfile, 
	   const std::string&	fname
      ); 

	//
   template<
      typename _Ty>
   struct Bukkit : public std::vector<_Ty> {

	Bukkit (size_t c) : std::vector<_Ty> (c), counter (0) {
		}

	_Ty* Get () {
		return &(*this)[counter++]; 
		} 

	void Reset () {
		counter = 0; 
		}

	size_t counter; 
	};
}

#endif
