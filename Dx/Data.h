#ifndef Data_h
#define Data_h

#include "Utility.h"


#include <vector>

namespace Db { 
	//
	// data types in 
	//

	 
	struct SizeOf {
		enum { 
			Label = 64, 
			}; 
		}; 

	enum Type {
		// binary
		B8,

		// unsigned 
		U8, 
		U16, 
		U32, 
		U64, 
	
		// signed
		S8, 
		S16, 
		S32, 
		S64, 

		// IEEE floats
		F32,
		F64,
		}; 

	// a file consists of a
	struct Data_file_header {
		unsigned int   ID;			// file marker ID
		unsigned int   version;	   // version
		unsigned int   entry_Count;// number of data entries 
		}; 

	// this is the beginning of each entry 
	struct Data_entry_header {
		char	label[SizeOf::Label]; 
		size_t	elem_Count; 
		Type	type; 
		}; 

	// File Format: 
	//	entry size = sizeof (Data_entry_header) + sizeof (entry data)

	// mem: 
	// IDID : Data_file_header::ID
	// vvvv : Data_file_header::version
	// 0x04 : Data_file_header::entry_Count
	// < offset | size > 
	// < offset | size > 
	// < offset | size > 
	// < offset | size >
	// entries begin...
	//

	//
	// functions 
	// 
	unsigned int File_version (
		const void* data_File
		);  

	//
	unsigned int Num_entries (
		const void* data_File
		);  

	// 
	const Data_entry_header& Entry_header (
		const void*	   data_File, 
		unsigned int   entry_Index
		); 

   //
	const void* Entry_data (
		const void*    data_File, 
		unsigned int   entry_Index
		); 

   //
	size_t Entry_data_size (
		const void*    data_File, 
		unsigned int   entry_Index
		); 

	//
	// Resource Manager ..
	//


} // Db

#endif 