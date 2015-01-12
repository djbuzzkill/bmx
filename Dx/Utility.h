#ifndef include_Utility
#define include_Utility

#include <map>
#include <list>
#include <vector>

#include <memory>

#include <assert.h>
#include <stdio.h>




#ifdef _DEBUG

	#ifdef _WIN32

		void _debug_break_ ();
		#define Debug(...)   fprintf(stderr, __VA_ARGS__)
		#define Assert(x, msg, ...) if((x) == false){printf("\n\n*WTF* -------------> \""##msg##"\" ", __VA_ARGS__); assert(0);} 
		#define Break(msg, ...) do{printf("\n\n*BREAK* -------------> \""##msg##"\" ", __VA_ARGS__); _debug_break_ ();}while(0)
		#define BreakAssert(x, msg, ...) if((x) == false){printf("\n\n*BREAK* -------------> \""##msg##"\" ", __VA_ARGS__); _debug_break_ ();} 

	#else
		#include "NeedPlatformBreak.h"
	#endif

#else

	#define Debug(...)   
	#define Assert(x, msg, ...)
	#define Break(msg, ...) 
	#define BreakAssert(msg, ...) 

#endif


/** ****************************************************************
 *  
 ******************************************************************/
#define El_count(x)(sizeof(x)/sizeof(x[0]))

#define TriMax(a, b, c) (((a)>(b))?(((a)>(c))?(a):(c)):(((b)>(c))?(b):(c)))
#define TriMin(a, b, c) (((a)<(b))?(((a)<(c))?(a):(c)):(((b)<(c))?(b):(c)))




namespace Ut
{
	typedef std::vector<unsigned char> Byte_array; 

	//
	size_t SizeOf_file (
		const std::string& fname
		); 

	// 
	size_t Load_data_file (
		Ut::Byte_array&		data_File, 
		const std::string&	fname
		); 

	//
template<typename _Ty>
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
