#include "Data.h"

#include <memory>



const char k_DataFile_ID[] = "DXDA"; 


//
//
unsigned int Db::File_version (
	const void* f_Data)
{
	const unsigned char* data_File = (const unsigned char*) f_Data; 
	return ((Data_file_header*) &data_File[0])->version; 
}

//
//
unsigned int Db::Num_entries (
	const void* f_Data)
{
   return static_cast<const Data_file_header*>(f_Data)->entry_Count; 
}

// 
//
const Db::Data_entry_header& Db::Entry_header (
	const void*	f_Data, 
	unsigned int		entry_Index)
{
	const unsigned char* data_File = (const unsigned char*) f_Data; 
	
	// file entry table begins immediate after entry_Count
	const unsigned int* entry_Table =  (const unsigned int* ) &data_File[sizeof (Db::Data_file_header)]; 

	// 2 * because offset + size
	return *( (const Db::Data_entry_header*) &data_File[ entry_Table[2 * entry_Index]] ); 
}

//
//
const void* Db::Entry_data (
	const void*    f_Data,     
	unsigned int   entry_Index)
{
	const unsigned char* data_File = (const unsigned char*) f_Data; 

	// file entry table begins immediate after entry_Count
	const unsigned int* entry_Table =  (const unsigned int* ) &data_File[sizeof (Db::Data_file_header)]; 

	// 2 * because offset + size
	return &data_File[ entry_Table[2 * entry_Index] + sizeof(Db::Data_entry_header) ] ; 
}

// 
//
unsigned int Db::Entry_data_size (
	const void*	   f_Data, 
	unsigned int   entry_Index) 
{
	const unsigned char* data_File = (const unsigned char*) f_Data; 

	// file entry table begins immediate after entry_Count
	const unsigned int* entry_Table =  (const unsigned int* ) &data_File[sizeof (Db::Data_file_header)]; 

	// 2 * because <offset + size>
	return  entry_Table[ (2 * entry_Index) + 1 ]; 
}
