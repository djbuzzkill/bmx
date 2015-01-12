#ifndef Include_Xp
#define Include_Xp

#include "Horde3D.h"
#include "Horde3DUtils.h"

#include <memory>
#include <memory.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <tchar.h>

#include <sstream>
#include <iomanip>
#include <list>
#include <map>

#include <forward_list>
//#include <GL/glew.h>


 
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
//#include "OIS.h"

#include "btBulletCollisionCommon.h"

#include <Dx/VecMath.h>
#include <Dx/System.h>

// C++11
#include<iterator>


struct mass_armature; 
struct ik_armature; 
struct articulation_armature; 

namespace Humanoid
{
   enum ActionState
   {
      AS_Standing, 

      AS_ShiftStartL, 
      AS_PushL, 
      AS_RecoilL, 
      AC_ContactR,

      AS_ShiftStartR,
      AS_PushR, 
      AS_RecoilR, 
      AS_ContactR, 

      ShiftStopR, 
      ShiftStopL, 

   };
   

   // idle? state computed from import
   struct BindState
   {
   }; 

   // current vector state
   struct MotionState
   {
   }; 

   // where we want to be
   struct MotionGoal
   {
   }; 

   //
   MotionState& Update_MotionState (
      MotionState&      motion, 
      const MotionGoal& goal
      ); 

   // Update transform matracies of nodes
   void Update_Matrices (
      std::map<H3DNode, int>& out, 
      const MotionState&      motionState
      ); 
   
   // state of humanoid based on imported bones
   void Calculate_composure (
      BindState&                             bindState, 
      const std::map<std::string, H3DNode>&  joint_Map
      ); 

   //
}




template<typename _Ty>
	inline void DontCare (_Ty& x)
{
	(void) x; 
}

//
// Conv_2_longlong
typedef unsigned long long	ui_t;
typedef long long			   si_t;

template<typename T>
union PtrU {
   void*             v; 
   char*             c; 
   unsigned char*    uc; 
   short*            s; 
   unsigned short*   us; 
   int*              i;
   unsigned int*     ui;  
   float*            f; 
   double*           d;
   T*                t; 
   }; 

//
// signed/unsigned 
ui_t str_2_uint	(const std::string& s); 
si_t str_2_int		(const std::string& s);
            
#define DefObj(x) struct x {};  
#define DefGeneric(...)  
DefGeneric (ObjA, ObjB, int , float); 

typedef int          s32; 
typedef unsigned int u32; 

typedef std::map<u32, std::string> IndexedMap;


namespace Fs
{
	// 
	// File_entry 
	struct File_entry {
		
		File_entry () : parent_Index (0), disk_Index (~0x0) { 
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
		Dir_node () : parent (0), next (0), children (0) { 
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
	inline Dir_node* Add_child (Dir_node* node, Dir_node* new_Child)
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
	Dir_node* Build_Dir_tree (
		Dir_node*			dir_Node, 
		Dir_node::Map&		dir_Map,
		const std::string&	accum_Path,	
		const std::string&	disk_Path
		);

	//
	// Build_Dir_tree - build tree from archive path
	IndexedMap& Build_Dir_table (
		IndexedMap&			out, 
		Dir_node::Map&		dir_Map, 
		const std::string&	accum_Path,
		const std::string&	disk_Path
		);
   //
	//
	File_entry::Map& Collect_files (
		Fs::File_entry::Map& out, 
		// SCC_NITF::Ar_file_entry::Bucket& bucket, 
		const IndexedMap&    fq_Paths, 
		const std::string&   disk_Path, 
		const size_t		   disk_Index
		); 
}

namespace xp
{

   //
   struct Joint_params
   {
      std::string          name_; 
      int                  index_; 
      Ma::Vec3f            offs_; 
      std::list<Ma::Vec3f> axes_; 
      std::list<float>     angles_; 
   }; 


   //
   struct Bind_State
   {
      std::map<int, Joint_params> joint_params; 

      float optimal_StandElevation; 
   }; 


   //
   class Biped { public: 
      //
      //
       
      enum Extrema {
         Heel_contact, 
         Toe_push_min, 
         Toe_push_max, 
         }; 

      //
      //
      enum PedalState {
         PS_contact, 
         PS_push, 
         PS_recoil, 
         PS_target, 
         }; 

      //
      //         
      enum DynamicState {
         DS_stand_idle, 
         DS_walk_forward, 
         // DS_walk_forward_2 etc.., 
         // DS_walk_backward, 
         }; 

      // target contact of foot [0-1] => [heel-toe]
      // does this make sense anymore?
      // struct Leg_params { 
      //    float          contact_delta;
      //    PedalState     pedl_state; 
      //    Ma::Vec3f      curr_pos; 
      //    Ma::Vec3f      targ_pos; 
      //    }; 

      //
      // serves a different purpose than Leg_param's
      // the bind characteristics
      struct Leg_params {
         // the ball of the foot to the heel 
         // joint relative to a flat surface
         float ball_bind_angle; 

         // the bind angle subtended by 
         // the ball -> heel ->knee joints
         float foot_bind_angle; 
 
         // vectors along the length of the 'bone'
         // hip -> knee
         Ma::Vec3f hipknee_dir; 
         //
         // knee -> heel
         Ma::Vec3f kneeheel_dir; 
         //
         // ball -> heel (this one points back up the chain)
         Ma::Vec3f ballheel_dir; 
         };


      // 
      DynamicState   targ_DS;             //DS_stand_idle, DS_walk_forward, 
      DynamicState   curr_DS; 

      float          curr_speed_;
      Ma::Vec3f      curr_dir;
      Ma::Vec3f      curr_pos;

      float          targ_speed_;   // scalar 
      Ma::Vec3f      targ_dir;      // facing
      Ma::Vec3f      targ_pos;      // this should be computed

      Leg_params     L; 
      Leg_params     R;

      Bind_State     bind_state;


   }; 


   void     Update_stand_idle();

	//
	// H3D::Obj
	//
	// class H3D::Obj {
	// 	void update_xf();
	// 	H3DNode		node; 
	// 	Ma::Vec3f	pos, rot, scl; 
	// public: 
	// 	H3D::Obj (); 
	// 	H3D::Obj (H3DNode node); 
   // 
	// 	void        Bind     (H3DNode node); 
	// 	void        Set_pos  (const Ma::Vec3f& pos); 
	// 	void        Set_rot  (const Ma::Vec3f& rot); 
	// 	void        Set_scl  (const Ma::Vec3f& scl); 
   // 
	// 	Ma::Vec3f&  Get_pos  (Ma::Vec3f& pos); 
	// 	Ma::Vec3f&  Get_rot  (Ma::Vec3f& rot); 
	// 	Ma::Vec3f&  Get_scl  (Ma::Vec3f& scl); 
	// 	}; 

	//
	// 
//	struct OIS_Comp {
//		// IO			
//		OIS::InputManager*		myInputMgr;   // OIS input manager
//		OIS::Keyboard*			myKeyboard;   // keyboard device
//		OIS::Mouse*				myMouse;      // mouse device
//		}; 

	//
	// horde graphhics
	// struct Horde3D_Comp 
   // {
   //    typedef std::map<std::string, H3DRes>		ResourceTable;
   //    typedef std::map<std::string, H3DNode>    NodeTable;		
   //    typedef std::map<std::string, H3D::Obj*>	ObjectTable;	
   // 
	// 	ResourceTable  res_Table;
	// 	NodeTable      node_Table;
	// 	ObjectTable    obj_Table;
	// };

	//
	// bullet 
	//struct BlPhy_Comp 
   //{
	//	std::shared_ptr<btDefaultCollisionConfiguration>	mCollisionConfiguration;	
	//	std::shared_ptr<btCollisionDispatcher>				   mCollisionDispatcher;
	//	std::shared_ptr<btAxisSweep3>						      mBroadPhaseInterface;
	//	std::shared_ptr<btCollisionWorld>					   mCollisionWorld;
   //
	//}; 



	//
	// do not want
	//void Update_view_transform (
	//	Ma::Vec3f&				view_Pos, 
	//	Ma::Vec3f&				view_Rot, 
	//	float					move_Rate,
	//	const OIS::Keyboard*	kb,    
	//	const OIS::Mouse*		ms
	//	);

   // 
   // a function to compute pos + rot
   void Update_view_transform (
	   Ma::Vec3f&				view_Pos, 
	   Ma::Vec3f&				view_Rot, 
	   float					   move_Rate,
	   const sy::Keyboard_state& kb,    
	   const sy::Mouse_state&    ms
      );


	//
	//OIS_Comp& Init_input (
	//	OIS_Comp& comp, 
	//	sy::System_context* sys, 
	//	sy::Window* rw
	//	);


	//	
	//BlPhy_Comp& Init_Bullet (
	//	BlPhy_Comp& 
	//	); 

	//
	sy::System_context* Create_Win32_system (
		HINSTANCE inst
		); 
}

#endif