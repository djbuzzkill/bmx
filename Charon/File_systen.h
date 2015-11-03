#ifndef FILESYSTEM_INCLUDE
#define FILESYSTEM_INCLUDE
namespace Fs
{
   // 
   // File_entry 
   struct File_entry {

      File_entry() : parent_Index(0), disk_Index(~0x0) {
      }

      typedef std::map<size_t, File_entry> Map;

      std::string    name;
      size_t		   parent_Index;
      size_t		   disk_Index;
   };

   //
   // 
   struct Dir_node {
      //
      Dir_node() : parent(0), next(0), children(0) {
      }
      std::string	name;
      u32    		index;
      Dir_node*	parent;
      Dir_node*	next;
      Dir_node*	children;
      //
      typedef std::map<u32, Dir_node> Map;
   };

   // 
   inline Dir_node* Add_child(Dir_node* node, Dir_node* new_Child)
   {
      new_Child->parent = node;

      if (Dir_node* curr_Child = node->children)
      {
         while (curr_Child)
         {
            if (curr_Child->next)
            {
               curr_Child = curr_Child->next;
            }
            else
            {
               curr_Child->next = new_Child;
               break;
            }
         }
      }
      else
      {
         node->children = new_Child;
      }
      return node;
   }


   //
   // Build_Dir_tree - build tree from archive path
   Dir_node* Build_Dir_tree(
      Dir_node*			dir_Node,
      Dir_node::Map&		dir_Map,
      const std::string&	accum_Path,
      const std::string&	disk_Path
      );

   //
   // Build_Dir_tree - build tree from archive path
   IndexedMap& Build_Dir_table(
      IndexedMap&			out,
      Dir_node::Map&		dir_Map,
      const std::string&	accum_Path,
      const std::string&	disk_Path
      );
   //
   //
   File_entry::Map& Collect_files(
      Fs::File_entry::Map& out,
      // SCC_NITF::Ar_file_entry::Bucket& bucket, 
      const IndexedMap&    fq_Paths,
      const std::string&   disk_Path,
      const size_t		   disk_Index
      );
}

#endif


