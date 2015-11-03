include "Files_system.h"


namespace Fs 
{
   
	enum Type {
		Invalid_FS_Type = -1, 
		RegFile, 
		DirPath , 
		}; 

	 Filesystem Node 
	struct Node {
		Type		   type; 
		std::string label;
		HANDLE		handl; 
		}; 

   
    Find_dir 
	bool Find_dir (Node& node, const std::string& cur_Path)
	{
		WIN32_FIND_DATA   findFile, 
                        nextFind; 

		std::string       list_Path   = cur_Path + "/*"; 
		HANDLE            handl       = ::FindFirstFile (list_Path.c_str (), &findFile); 

		if (handl != INVALID_HANDLE_VALUE)
		{
			node.handl = handl; 
			node.label = findFile.cFileName; 
			
			if (findFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				node.type =	DirPath;  
			}
			else 
			if (findFile.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)
			{
				node.type = RegFile;  
			}
			else
			{
				node.type = Invalid_FS_Type; 
			}

			return true; 
		}
		return false; 

	}

	bool Close_dir (Node& node)
	{
		return (TRUE == ::FindClose (node.handl));  
	}

	bool Next_entry (Node& next, const Node& cur_Node)
	{
		WIN32_FIND_DATA findFile; 

		if (::FindNextFile (cur_Node.handl, &findFile))
		{
			next.label = findFile.cFileName; 
			next.handl = cur_Node.handl; 

			if (findFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				next.type =	DirPath;  
			}
			else 
			if (findFile.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
			{
				 FILE_ATTRIBUTE_NORMAL is not correct 
				next.type = RegFile;  
			}
			else
			{
				next.type = Invalid_FS_Type; 
			}

			return true; 
		}

		DWORD error_Code = ::GetLastError (); 

		return false; 
	}

   
   
   bool Collect_exclusion_list (const std::string& txt)
   {
       nothing for now
      return false; 
   }

   bool File_exclusion_list (const std::string& txt)
   {
      return false; 
   }

   
    Build_Dir_tree - build tree from archive path
   Dir_node* Build_Dir_tree (
	   Dir_node*			   dir_Node, 
	   Dir_node::Map&		   dir_Map, 
	   const std::string&	accum_Path,	
	   const std::string&	disk_Path) 
   {
	   Node fs_Node, fs_Next; 

	   const std::string archive_Path =  disk_Path + "/" + accum_Path; 

	   if ( Find_dir (fs_Node, archive_Path)) 
	   {
		   if (!dir_Node)
		   {
             this better be the root
			   size_t node_Index = dir_Map.size () + 1; 
			   dir_Node		= &dir_Map[node_Index]; // node_Bucket.Get (); 
			   dir_Node->name	= accum_Path; 
			   dir_Node->index = node_Index; 
		   }

		   while (Next_entry (fs_Next, fs_Node)) //for (bfs::directory_iterator iter (cur_Path); iter != bfs::directory_iterator (); iter++)
		   {
			   if (fs_Next.label == "." || fs_Next.label == "..")
				   continue; 

			   if (fs_Next.type == DirPath) //if (bfs::is_directory (iter->status ()))
			   {
				   size_t dir_Index = dir_Map.size () + 1; 
				   Dir_node* new_Node = &dir_Map[dir_Index];// 
				   new_Node->index = dir_Index; 

				   Add_child (dir_Node, new_Node);			// dir_Node->children.push_back (new_Node); 
				   new_Node->name = fs_Next.label;			//new_Node->name		= iter->leaf (); 
				   Build_Dir_tree (new_Node, dir_Map, accum_Path + "/" + new_Node->name, disk_Path); 
			   }
		   }

		   Close_dir (fs_Node); 
	   }

	   return dir_Node;
   }


   
   
    IndexedMap& Build_Dir_table (
    	IndexedMap&			out, 
    	Dir_node::Map&		dir_Map, 
    	const std::string&	archive_ID,
    	const std::string&	disk_Path
    	)
    {
    	std::string archive_Path = disk_Path + "/" + archive_ID; 
    	
    	Dir_node::Bucket node_Bucket (1024); 
    	
    	Dir_node* dir_Root = Build_Dir_tree (
    		0, 
    		dir_Map, // node_Bucket, 
    		archive_ID,
    		disk_Path 
    		); 
    	
    	
    	 FQ names - relavtive path from base (ar_path). 
    	for (Dir_node::Map::iterator iter = dir_Map.begin (); iter != dir_Map.end(); iter++)
    		out[iter->first] = FQ_path_name (iter->second, "/");
    	
    	return out; 
    }


    File_entry::Map& Collect_files (
	   File_entry::Map&		out, 
	   Ar_file_entry::Bucket&	bucket, 
	   const IndexedMap&		fq_Paths, 
	   const std::string&		disk_Path, 
	   const size_t			disk_Index)
   {
      std::map<std::string, std::vector<int> > uniq_map;    
      for (File_entry::Map::const_iterator it = out.begin (); it != out.end (); it++)
         uniq_map[it->second.name].push_back (it->second.parent_Index); 

	   size_t file_Counter = out.size ();
	   for (IndexedMap::const_iterator it = fq_Paths.begin (); it != fq_Paths.end (); it++)
	   {
		   Node dirNode, curNode; 
		   if ( Find_dir (dirNode, disk_Path + "/" + it->second))
		   {
			   while ( Next_entry (curNode, dirNode))
			   {
                exclude 
               if (File_exclusion_list (curNode.label))
                  continue; 

                check the initial contents
               if (uniq_map.count (curNode.label) && std::count (uniq_map[curNode.label].begin (), uniq_map[curNode.label].end (), it->first))
                  continue; 

				   if (curNode.type == RegFile)
				   {
					   file_Counter++; // so we can be non-zero
					   out[file_Counter] = bucket.Get (); 
					   out[file_Counter].name			= curNode.label; 
					   out[file_Counter].parent_Index	= it->first ; 
					   out[file_Counter].disk_Index	= disk_Index;
				   }
			   }

			   Close_dir (dirNode); 
		   }
	   }
	   return out; 
   }

}
