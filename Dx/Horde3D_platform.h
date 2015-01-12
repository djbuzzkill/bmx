#ifndef introducing_Horde3D_platform
#define introducing_Horde3D_platform


#include <vector>
#include "VecMath.h"

#include <Horde3D.h>
#include <Horde3DUtils.h>



namespace H3D
{
   //
   class Obj {
	   void update_xf();
	   H3DNode		node; 
	   Ma::Vec3f	pos, rot, scl; 
   public: 
	   Obj (); 
	   Obj (H3DNode node); 

	   void        Bind     (H3DNode node); 
	   void        Set_pos  (const Ma::Vec3f& pos); 
	   void        Set_rot  (const Ma::Vec3f& rot); 
	   void        Set_scl  (const Ma::Vec3f& scl); 

	   Ma::Vec3f&  Get_pos  (Ma::Vec3f& pos); 
	   Ma::Vec3f&  Get_rot  (Ma::Vec3f& rot); 
	   Ma::Vec3f&  Get_scl  (Ma::Vec3f& scl); 

	   }; 



   // StaticStreamElem - cast a mapped buffer to this 
   struct StaticStreamElem {
      Ma::Vec2f uv0; 
      Ma::Vec4f jointInds; 
      Ma::Vec4f jointWgts; 
      Ma::Vec2f uv1; 
      }; 

   //
   // Geo - not used in game.  
   struct Geo {
	   char magic[4];
	   int version; 
	   int num_Vert_Streams; 
	   int num_Verts;
	   int num_Inds;
	   int num_Joints; 
	   int num_Morph_Targets;

	   std::vector<Ma::Mat44f>	inv_Bind_Mats; 

	   std::vector<int>		vInds; 
	   std::vector<Ma::Vec3f>	vPos; 
	   std::vector<Ma::Vec3s>	vNrm; 
	   std::vector<Ma::Vec3s>	vTan;

	   std::vector<Ma::Vec4ub>	vJointInds;
	   std::vector<Ma::Vec4ub>	vJointWgts;

	   std::vector<Ma::Vec3s>	vBiTan; 
	   std::vector<Ma::Vec2f>	vTxc0; 
	   std::vector<Ma::Vec2f>	vTxc1; 
      }; 


   struct Transform
   {
      Ma::Quatf rot; 
      Ma::Vec3f transl; 
      Ma::Vec3f scale; 
   }; 

   struct AnimNode {
      char name[256]; 
      char compressed; 
      std::vector<Transform> transform; 
      }; 




   struct Anim { 

      char magic[4]; 
      int version; 
      int n_Anims; 
      int n_Frames; 

      std::vector<AnimNode> nodes; 

      }; 

   //
   // horde graphhics
   struct Comp 
   {
      //typedef std::map<std::string, H3DRes>   ResourceTable;
	   //ResourceTable  res_Table;

      typedef std::map<std::string, H3DNode> NodeTable;		
      NodeTable                              node_Table;

      typedef std::map<std::string, Obj*>    ObjectTable;	
	   ObjectTable                            obj_Table;
   };

   // geometry
   Geo& Load (
      Geo&                 geo, 
      const std::string&   fname
      ); 

   // animation
   Anim& Load (
      Anim& anim, 
      const std::string& fname
      );  
} 

#endif


