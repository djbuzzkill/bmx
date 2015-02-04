	#include "Utility.h"



#if WIN32

   #include "windows.h"
#else
   #include "_Need_Platform_Break_"
#endif





/** ****************************************************************
* thomasinesi.com 
******************************************************************/
void _debug_break_ ()
{
	DebugBreak ();
}



size_t ut::SizeOf_file (const std::string& fname)
{
	std::shared_ptr<FILE> f ( fopen (fname.c_str (), "rb"), fclose );  

	fseek (f.get(), 0, SEEK_END);  
	long sizeOf_file = ftell (f.get()); 

	return sizeOf_file; 
}




size_t ut::Load_text_file (
	std::vector<char>&   txtfile, 
	const std::string&	fname)
{
   size_t sizeOf_file = ut::SizeOf_file (fname);

	txtfile.resize (1 + sizeOf_file , 0); 
	std::shared_ptr<FILE> f (fopen (fname.c_str (), "rb"), fclose); 
	return fread (&txtfile[0], sizeof (char), sizeOf_file, f.get()); 
}


// 
size_t ut::Load_data_file (
	ut::Byte_array&		data_File, 
	const std::string&	fname)
{

	data_File.resize ( ut::SizeOf_file (fname) ); 

	std::shared_ptr<FILE> f (fopen (fname.c_str (), "rb"), fclose); 

	return fread (&data_File[0], sizeof (unsigned char), data_File.size (), f.get()); 
}


