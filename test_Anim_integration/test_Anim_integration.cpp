// test_Anim_integration.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <map>
#include <list>
#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include <algorithm>

#include <SDL.h>

#include <eh.h>

#include <glm/glm.hpp>

#include <Dx/Dx.h>
#include <Xp/Xp.h> 
#include <Dx/Horde3D_platform.h>

#include <GL/glew.h>
#include <GL/wglew.h>


using namespace Ma;
using namespace Sc;

class character_collision;
class test_terrain_collision;

//
// sg_Default_PixelFormatDescriptor
static PIXELFORMATDESCRIPTOR sg_Default_PIXELFORMATDESCRIPTOR = {

	sizeof (PIXELFORMATDESCRIPTOR), // WORD  nSize;
	0x0001,  // WORD  nVersion;
	PFD_DRAW_TO_WINDOW|PFD_DOUBLEBUFFER|PFD_SUPPORT_OPENGL, // DWORD dwFlags;
	PFD_TYPE_RGBA,                                              // BYTE  iPixelType;
	32,      // BYTE  cColorBits;

	0,			// BYTE  cRedBits;
	0,			// BYTE  cRedShift;
	0,			// BYTE  cGreenBits;
	0,			// BYTE  cGreenShift;
	0,			// BYTE  cBlueBits;
	0,			// BYTE  cBlueShift;
				   
	0,			// BYTE  cAlphaBits;
	0,			// BYTE  cAlphaShift;
	0,			// BYTE  cAccumBits;
	0,			// BYTE  cAccumRedBits;
	0,			// BYTE  cAccumGreenBits;
	0,			// BYTE  cAccumBlueBits;
	0,			// BYTE  cAccumAlphaBits;
				   
	24,			// BYTE  cDepthBits;
	8,			// BYTE  cStencilBits;
	0,			// BYTE  cAuxBuffers;
				   
	0,			// BYTE  iLayerType;
	0,			// BYTE  bReserved;
	0,			// DWORD dwLayerMask;
	0,			// DWORD dwVisibleMask;
	0,			// DWORD dwDamageMask;	
	}; 



//
//
static char kContent_root[]      = "C:/Quarantine/awsum/humanoid_test"; 
static char kForward_pipeline[]  = "pipelines/forward.pipeline.xml"; 
static char kDeferred_pipeline[] = "pipelines/deferred.pipeline.xml"; 
static char kWhatsMyName[]       = "models/watsmyname.geo.scene"; 
static char kTrenchcoatGuy[]     = "models/man/man.scene.xml"; 
static char kWalkAnim[]          = "animations/man.anim";
static char kStage[]             = "models/hills.geo.scene";
static char kStageGeo[]          = "models/hills.geo";
static char kRockPath[]          = "models/high_path.geo.scene";
static char kIsland[]            = "models/island.geo.scene";

static H3D::Obj                  sg_GfxObj[256]; 
static int                       sg_GfxObj_counter = 0; 




namespace BG_terrain {

   typedef glm::ivec2 Key; 

   struct Less_than 
   {
      bool operator() (const Key& lhs, const Key& rhs)  
      {
         if (lhs[0] < rhs[0])
            return true; 
         else 
         if (lhs[0] == rhs[0])
            return (lhs[1] < rhs[1]);
         else
            return false;
      }


   }; 

   typedef std::map<Key, glm::dvec3, Less_than> Map; 

}



const An::Joint_def kUndef_joint = { 
   "undef", 0, { V3f(0.0f, 1.0f, 0.0f), V3f(), V3f() }
   };


const An::Joint_def kHumanoidArmatureDef[] = 
{
	{ "root",				0, { V3f(0.0f, 1.0f, 0.0f), V3f(), V3f() } },

	{ "spine.001",			1, { V3f(1.0f, 0.0f, 0.0f), V3f(), V3f() } },
	{ "spine.002",			1, { V3f(1.0f, 0.0f, 0.0f), V3f(), V3f() } },
	{ "spine.003",			1, { V3f(1.0f, 0.0f, 0.0f), V3f(), V3f() } },
	{ "neck",				1, { V3f(1.0f, 0.0f, 0.0f), V3f(), V3f() } },
	{ "head",				1, { V3f(1.0f, 0.0f, 0.0f), V3f(), V3f() } },

   // todo - these branch off the spine, minmal movement
	{ "shoulder.left",	0, { V3f(1.0f, 0.0f, 0.0f), V3f(1.0f, 0.0f, 0.0f), V3f() } },
	{ "shoulder.right",	0, { V3f(1.0f, 0.0f, 0.0f), V3f(1.0f, 0.0f, 0.0f), V3f() } },
	
   // upper arm (shoulders), appear to be correct
	{ "arm.upper.left",	3,	{ V3f(0.0f, 1.0f, 0.0f), V3f(1.0f, 0.0f, 0.0f), V3f(0.0f, 0.0f, 1.0f) } },
	{ "arm.upper.right", 3, { V3f(0.0f, 1.0f, 0.0f), V3f(1.0f, 0.0f, 0.0f), V3f(0.0f, 0.0f, 1.0f) } },

   // todo
	{ "arm.lower.left",	1, { V3f(0.0f, 1.0f, 0.0f), V3f(), V3f() } },
	{ "arm.lower.right", 1, { V3f(0.0f, 1.0f, 0.0f), V3f(), V3f() } },

   // todo
	{ "hand.left",			0, { V3f(0.0f, 1.0f, 0.0f), V3f(), V3f() } },
	{ "hand.right",		0, { V3f(0.0f, 1.0f, 0.0f), V3f(), V3f() } },

   // the pelvis should be (are) fixed
	{ "pelvis.left",		0, { V3f(0.0f, 1.0f, 0.0f), V3f(), V3f() } },
	{ "pelvis.right",		0, { V3f(0.0f, 1.0f, 0.0f), V3f(), V3f() } },
	{ "hip.left",			0, { V3f(0.0f, 1.0f, 0.0f), V3f(), V3f() } },
	{ "hip.right",			0, { V3f(0.0f, 1.0f, 0.0f), V3f(), V3f() } },

   // upper legs appear to be correct 
	{ "leg.upper.left",	3, { V3f(0.0f, 1.0f, 0.0f), V3f(0.0f, 0.0f, 1.0f), V3f(1.0f, 0.0f, 0.0f) } },
	{ "leg.upper.right", 3, { V3f(0.0f, 1.0f, 0.0f), V3f(0.0f, 0.0f, 1.0f), V3f(1.0f, 0.0f, 0.0f) } },

   // lower legs (knees) only bend one way
	{ "leg.lower.left",	1, { V3f(1.0f, 0.0f, 0.0f), V3f(), V3f() } },
	{ "leg.lower.right", 1, { V3f(1.0f, 0.0f, 0.0f), V3f(), V3f() } },

   // feet not done yet
	{ "foot.left",			2, { V3f(1.0f, 0.0f, 0.0f), V3f(0.0f, 1.0f, 0.0f), V3f() } },
	{ "foot.right",		2, { V3f(1.0f, 0.0f, 0.0f), V3f(0.0f, 1.0f, 0.0f), V3f() } },
   // toes not done yet
	{ "toe.left",			1, { V3f(1.0f, 0.0f, 0.0f), V3f(), V3f() } },
	{ "toe.right",			1, { V3f(1.0f, 0.0f, 0.0f), V3f(), V3f() } },
}; 

const unsigned k_Humanoid_joint_count =
		El_count(kHumanoidArmatureDef);

An::Joint_def::Map sg_HumanoidJointMap;



//
// Colliders ---------------------------------
//
class character_collision : public Co::Collider 
{ 
   Sc::Obj*   obj;  
   std::string    label;
   //H3DNode horde3d_node;

public: 

   //
   //
   character_collision (Sc::Obj* o, const std::string& lbl) : obj (o), label(lbl) {
      }

   //
   // 
   virtual int On_contact (Collider* other, const Vec3f& pos, const Vec3f& nrm, float dist) {

      Vec3f my_pos;
      // 
      // Assert (0, "Get_position gets the visual position, what about the collidable position"); 
      Get_position (my_pos) ;
      return 0;

      }

   //
   virtual Vec3f& Get_position (Vec3f& pos) {
      return Sc::Get_pos (pos, obj);
      } 

   //
   virtual const char* Get_label () {
      return label.c_str();
      }

}; 


//
// test_terrain_collision ---------------------------------
class test_terrain_collision : public Co::Collider 
{ 

	Sc::Obj* obj;
   std::string label;

public: 

   test_terrain_collision (Sc::Obj* o, const std::string& lbl) 
      : obj (o), label (lbl) 
      {
      }

   virtual int On_contact (Collider* other, const Vec3f& pos, const Vec3f& nrm, float dist) 
      {
      return 0;
      }

   virtual Vec3f& Get_position (Vec3f& pos) 
      {
      return Sc::Get_pos (pos, obj);
      } 

   virtual const char* Get_label () {
      return label.c_str();
      }

}; 

struct Thread_context 
{
   int someshit; 
}; 

int Work_Thread (void* user)
{
   int hello = 0; 

   WNDCLASS wndclass = 
   {
   }; 


   DWORD err_ = 0;   
   BOOL res_ = 0;

   //RegisterClass (
   HDC hdc = ::GetDC (0);
   int pxl_format = ::ChoosePixelFormat (hdc, &sg_Default_PIXELFORMATDESCRIPTOR); 

   //int pxl_format = ::GetPixelFormat (hdc); 
   res_ = ::SetPixelFormat (hdc, pxl_format , &sg_Default_PIXELFORMATDESCRIPTOR); 

   HGLRC hglrc = ::wglCreateContext (hdc); 
err_ =    ::GetLastError (); 

   HGLRC cur_hglrc = wglGetCurrentContext (); 
err_ =    ::GetLastError (); 


   BOOL res = ::wglMakeCurrent (hdc, hglrc ); 
   err_ =    ::GetLastError (); 

   GLuint         glerr_ = 0;
   const GLubyte* glerr_s_ = 0;
   //GLenum  glew_res = glewInit (); 
   GLuint tex_ID; 

   glewInit (); 

   glGenTextures(1, &tex_ID);

   glerr_ = glGetError (); 
   if (glerr_ != GL_NO_ERROR)
      {
      glerr_s_= glewGetErrorString (glerr_); 
      }

   GLuint fb_ID = 0;
   glGenFramebuffers (1, &fb_ID); 
   
   glerr_ = glGetError (); 
   if (glerr_ != GL_NO_ERROR)
      {
      glerr_s_= glewGetErrorString (glerr_); 
      }
   



   hello++;

   return 0; 
}

//
// APP ---------------------------------
class test_anim 
   : public sy::RT_window_listener
   // , public H3D::Comp
{
   bool                                   init_; 
   std::shared_ptr<sy::Graphics_window>    windo; 
   Vec3f                                  view_Rot, view_Pos; 
   int                                    viewport_wd;   
   int                                    viewport_ht;      
   double                                 Dt;         
   sy::Time_tracker                       tracker;    
   std::shared_ptr<Co::Collision_space>   coll_space; 
   Sc::Res                                fwd_pipe;   
   // Sc::ObjMap                             obj_Table;  
   Dx::GobyMap                            gobys; 

   //
   ////


   struct : public An::Humanoid_artic {
      //H3DNodeMap		                     bone_Node; 
      //std::map<std::string, int>		      bone_Index; 
      //ObjectTable                         bone_Objs; 
      Vec3f                         fwd_pos_R, rear_pos_R,
                                    fwd_pos_L, rear_pos_L;
      // JointState::Axes should be moved into An::Humanoid_artic 
      // do we need the JointIndex from H3D?
      // kHumanoidArmatureDef should be moved into An::Humanoid_artic 

      std::shared_ptr<Co::Collider> coll;
      //
      } wat;

   //
   // trenchcoat guy
   struct { // 
      std::shared_ptr<Co::Collider>       coll;
      } trenchy; 

   //
   // hill thing
   struct { 
      std::vector<float>              heights;
      Vec2i                           dims;
      Vec2f                           size;
      Vec3f                           vmin, vmax;
      std::shared_ptr<Co::Collider>       coll;
      } hills; 
   
   //
   // rock path
   struct { 
      std::shared_ptr<Co::Collider>       coll;
      } rock_path; 

   // island
   struct { 
      std::shared_ptr<Co::Collider>       coll;
      } island; 

   bool enable_trenchguy ; 
   bool enable_whatsmyname ; 
   bool enable_rock_path ;
   bool enable_hill ;
   bool enable_island ;
   // wat
   //
   // test ik, 
   // biped
   // test collision
   // test animation
   //
   
   //
   // init_scene
   void init_scene (sy::System_context* sys)
   {
      //char revise_this_for_new_application_we_need_fixed_bind_points_for_leg_articulation[0];

      // test opengl in a separate thread
      // Thread_context work_context; 
      // SDL_CreateThread (Work_Thread, "",  &work_context);  

		// initialize Humanoid joint definition
      sg_HumanoidJointMap["undef"] = &kUndef_joint; 
		for (size_t i = 0; i < k_Humanoid_joint_count; i++)
			sg_HumanoidJointMap[kHumanoidArmatureDef[i].name] = kHumanoidArmatureDef+i; 

      enable_trenchguy = true; 
      enable_whatsmyname = true; 
      enable_rock_path = false;
      enable_hill = false;
      enable_island = true;
      // create the horde scene
      
      if (Sc::Initialize_scene ())
      {	// Set options
			h3dSetOption (H3DOptions::LoadTextures,   1);
			h3dSetOption (H3DOptions::TexCompression,	0);
			h3dSetOption (H3DOptions::FastAnimation,	1);
			h3dSetOption (H3DOptions::MaxAnisotropy,	4);
			h3dSetOption (H3DOptions::ShadowMapSize,  2048);

			// setup pipeline/resources
         {
            // a forward renderer
            Sc::Add_resource (kForward_pipeline, Sc::Pipeline, 0); 
            //res_Table["forward_pipe"]  = h3dAddResource (H3DResTypes::Pipeline, kForward_pipeline , 0);

            if (enable_trenchguy)
            {
            // trench coat man
            Sc::Add_resource (kTrenchcoatGuy, Sc::SceneGraph , 0); 
            Sc::Add_resource (kWalkAnim, Sc::Animation, 0); 
            }

            if (enable_whatsmyname)
            {
            // test humanoid
            Sc::Add_resource (kWhatsMyName, Sc::SceneGraph, 0); 
            }

            // environment model  
            if (enable_hill)
            {
               Sc::Add_resource (kStage, Sc::SceneGraph, 0); 
            }


            if (enable_rock_path)
            {
               Sc::Add_resource (kRockPath, Sc::SceneGraph, 0); 
            }

            if (enable_island) 
            {
               Sc::Add_resource (kIsland, Sc::SceneGraph, 0); 
               //res_Table["island"] = h3dAddResource (H3DResTypes::SceneGraph, kIsland, 0);
            }

			   // Load resources
			   h3dutLoadResourcesFromDisk (kContent_root);
         }

			// build scene ( build scene graph
         {
            Vec3f v_0, v_1; 

            if (enable_rock_path) 
            {
               Dx::So(gobys["rock_path"]) = Sc::Create_scene_obj ("rock_path", kRockPath); 
            }

            if (enable_island) 
            {
               Dx::So(gobys["island"]) = Sc::Create_scene_obj ("island", kIsland); 
               //obj_Table["island"] = Sc::Create_scene_obj ("island", kIsland); 
            }

            if (enable_hill)
            {  // the setting/stage whatever
               Dx::So (gobys["stage"]) = Sc::Create_scene_obj ("stage", "stage"); 
               //obj_Table["stage"]     = Sc::Create_scene_obj ("stage", "stage"); 
            }

            if (enable_trenchguy)
            {  // get this working first

               float scale_ = 36.0f;
               //obj_Table[ "trench_guy" ]  = Sc::Create_scene_obj ("trench_guy", kTrenchcoatGuy); 
               Dx::So(gobys[ "trench_guy" ]) = Sc::Create_scene_obj ("trench_guy", kTrenchcoatGuy); 

               Sc::Set_pos (Dx::So(gobys["trench_guy"]), Set (v_0, 200.0f, 140.0f, -180.0f)); 
               Sc::Set_scl (Dx::So(gobys["trench_guy"]), Set (v_0, scale_, scale_, scale_) ); 
            }
            
            if (enable_whatsmyname)
            {  // our test model
               const float scale_ = 6.0f; 
      
               Dx::So(gobys["wat"]) = Sc::Create_scene_obj ("wat", kWhatsMyName); 
               Sc::Set_pos (Dx::So(gobys["wat"]), Set (v_0, -100.0f, 180.0f, 100.0f)); 
               Sc::Set_scl (Dx::So(gobys["wat"]), Set (v_0, scale_, scale_, scale_) ); 
            }

			   {  // camera

               Dx::So(gobys["camera"]) = Sc::Create_camera ("camera", Sc::Get_resource(Sc::Pipeline, kForward_pipeline)); 
               Sc::Set_pos (Dx::So(gobys["camera"]), Set (v_0, 0.0f, 400.0f, -10.0f)); 
            }
            
            {  //
               // Add light source

               Vec3f _; 

               Dx::So(gobys["light"]) = Sc::Create_light ("light", 0, "LIGHTING", "SHADOWMAP"); 
               Sc::Set_pos (Dx::So(gobys["light"]), Set (_, 0.0f, 1200.0f, 0.0f) ); 
               Sc::Set_rot (Dx::So(gobys["light"]), Set (_, -90.0f, 0.0f, 0.0f)  ); 
               Sc::Set_scl (Dx::So(gobys["light"]), Set (_, 1.0f, 1.0f, 1.0f)    ); 
               
               Sc::Light_params (   
                  Dx::So(gobys["light"]),
                  2000.0f, 90.0, 
                  1, 
                  0.01f,
                  Set(_, 1.0f, 0.8f, 1.0f),
                  1.0f
                  ); 
   
            }
         }

      //
      // do skeletal stuff with wat
		if (enable_whatsmyname)
		{  
         // scob
         wat.Init (Dx::So(gobys["wat"]),  sg_HumanoidJointMap); 

         Vec3f hip_pos, knee_pos, heel_pos, leg_len;  
         Vec3f fwd_dist, bak_dist;

		   Sc::Get_pos (hip_pos , &wat.joint_arr[wat.joint_ind["leg.upper.left"]]); 
         Sc::Get_pos (knee_pos, &wat.joint_arr[wat.joint_ind["leg.lower.left"]]); 
         Sc::Get_pos (heel_pos, &wat.joint_arr[wat.joint_ind["foot.left"]]); 

         Add (leg_len, knee_pos, heel_pos); 
                       
         Scale (wat.min_compr, leg_len, An::Biped::kMin_compression); 
         Scale (wat.max_compr, leg_len, An::Biped::kMax_compression); 

         wat.coll.get();
      }

      resize_render_targets (800, 600); 
      }
   }
 
   //
   // init_collision 
   void init_collision (sy::System_context* sys)
   {
      Vec3f  world_Min ;
      Vec3f  world_Max ;

      Set (world_Min, -1200.0f,-1000.0f,-1200.0f);
      Set (world_Max,  1200.0f, 1000.0f, 1200.0f);

      coll_space.reset (Co::Create_collision_space (world_Min, world_Max), Co::Destroy_collision_space); 
      X(world_Max);


      if (enable_island)
      {
         H3DRes geo = h3dFindResource  (H3DResTypes::Geometry, "models/island.geo"); 
         Assert (geo, "no island");
         island.coll.reset (new test_terrain_collision (Dx::So(gobys["island"]), "island" ));

         int               num_inds    = h3dGetResParamI (geo, H3DGeoRes::GeometryElem, 0, H3DGeoRes::GeoIndexCountI);  
         int               num_verts   = h3dGetResParamI (geo, H3DGeoRes::GeometryElem, 0, H3DGeoRes::GeoVertexCountI); 

         Vec3f*            verts       = (Vec3f*)           h3dMapResStream (geo, H3DGeoRes::GeometryElem, 0, H3DGeoRes::GeoVertPosStream, true, false); 
         unsigned short*   inds        = (unsigned short*)  h3dMapResStream (geo, H3DGeoRes::GeometryElem, 0, H3DGeoRes::GeoIndexStream, true, false); 

         Co::TriMesh_params params = {
            num_inds, 
            inds, 
            verts
            };

         Co::Add_collider_tri_mesh (coll_space.get(), island.coll.get(), params); 

         h3dUnmapResStream (geo); 
      }


      if (enable_rock_path)
      {

         H3DRes geo = h3dFindResource  (H3DResTypes::Geometry, "models/rock_path.geo"); 
         
         rock_path.coll.reset (new test_terrain_collision (Dx::So(gobys["rock_path"]), "rock_path" ));

         int               num_inds    = h3dGetResParamI (geo, H3DGeoRes::GeometryElem, 0, H3DGeoRes::GeoIndexCountI);  
         int               num_verts   = h3dGetResParamI (geo, H3DGeoRes::GeometryElem, 0, H3DGeoRes::GeoVertexCountI); 

         Vec3f*            verts       = (Vec3f*)           h3dMapResStream (geo, H3DGeoRes::GeometryElem, 0, H3DGeoRes::GeoVertPosStream, true, false); 
         unsigned short*   inds        = (unsigned short*)  h3dMapResStream (geo, H3DGeoRes::GeometryElem, 0, H3DGeoRes::GeoIndexStream, true, false); 

         Co::TriMesh_params params = {
            num_inds, 
            inds, 
            verts
            };

         Co::Add_collider_tri_mesh (coll_space.get(), rock_path.coll.get(), params); 

         h3dUnmapResStream (geo); 
      }


      // setup terrain     
   
      if (enable_hill)
      { 
         H3D::Geo geo;
         H3D::Load (geo, std::string(kContent_root) + "/" + std::string(kStageGeo));  

         hills.coll.reset(new test_terrain_collision (Dx::So(gobys["hilly"]), "hilly")); 

         int h = sqrt (geo.vPos.size());
         Set (hills.dims, h, h); 
         hills.heights.resize (geo.vPos.size()); 

         Set(hills.vmin, X(geo.vPos[0]), Y(geo.vPos[0]), Z(geo.vPos[0]));
         Set(hills.vmax, X(geo.vPos[0]), Y(geo.vPos[0]), Z(geo.vPos[0]));

         for (size_t iPt  = 0; iPt < geo.vPos.size(); iPt++)
         {
         
            // THIS IS RONG!!!
            Vec3f& v = geo.vPos[iPt];
         
            hills.heights[iPt] = Y(v);
         
            X(hills.vmin) = (std::min) (X(hills.vmin), X(v)); 
            Y(hills.vmin) = (std::min) (Y(hills.vmin), Y(v));
            Z(hills.vmin) = (std::min) (Z(hills.vmin), Z(v));
         
            X(hills.vmax) = (std::max) (X(hills.vmax), X(v)); 
            Y(hills.vmax) = (std::max) (Y(hills.vmax), Y(v));
            Z(hills.vmax) = (std::max) (Z(hills.vmax), Z(v));
         
         }

         Set (
            hills.size, 
            X(hills.vmax) - X(hills.vmin), 
            Z(hills.vmax) - Z(hills.vmin));

         Co::Heightfield_params params = {
            hills.vmin, 
            hills.vmax, 
            X(hills.size), Y(hills.size), 
            X(hills.dims), Y(hills.dims), 
            hills.heights.data()
            };

         //
         Co::Add_collider_height_field (coll_space.get(), hills.coll.get(), params);       
      }


      // add character 
      if (enable_whatsmyname)
      {
         wat.coll.reset (new character_collision (Dx::So(gobys["wat"]), "wat"));
      
         Vec3f vmin, vmax;

         Bn::AABBf aabb; 
         Sc::Get_AABB (aabb, Dx::So(gobys["wat"])); 

         float depth_size = Z(aabb.max_ext) - Z(aabb.min_ext);
         float width_size = X(aabb.max_ext) - X(aabb.min_ext);

         float char_rad    = 0.25f * (depth_size + width_size) ;
         float char_height = Y(vmax) - Y(vmin) ;
         // char_height *= 0.5f;
         Co::Add_collider_capsule (coll_space.get(), wat.coll.get(), 1, char_rad, char_height);       


         Sc::Res wat_geo = Sc::Get_resource (Sc::Geometry, "models/watsmyname.geo" ); 
         
         //= h3dFindResource (H3DResTypes::Geometry, "models/watsmyname.geo" ); 
         
         //Ut::SizeOf_file ( 
         //h3dLoadResource ( "", 69 ); 
           
         Ma::Vec3f*             vpos        = (Ma::Vec3f*)                h3dMapResStream (wat_geo, H3DGeoRes::GeometryElem, 0, H3DGeoRes::GeoVertPosStream     , true, false); 
         H3D::StaticStreamElem* vstatic     = (H3D::StaticStreamElem*)h3dMapResStream (wat_geo, H3DGeoRes::GeometryElem, 0, H3DGeoRes::GeoVertStaticStream  , true, false); 
         int                    num_verts   =                             h3dGetResParamI (wat_geo, H3DGeoRes::GeometryElem, 0, H3DGeoRes::GeoVertexCountI);

         std::vector<H3D::StaticStreamElem> DBG_static_stream (vstatic, vstatic + num_verts); 
         h3dUnmapResStream (wat_geo); 
      }
      // creat terrain collision (hills.geo)
      // create char collision (wat)
      // 
   } 


   void resize_render_targets (int wd, int ht)
   {

      // resize render targets
      Sc::Camera_params ( 
         Dx::So(gobys["camera"]), 
         //
			0, 0,
			wd, ht, 
         60.0f, 
         1.0f, 5000.0f 
         ); 

      //
      Sc::Resize_render_target (
         Sc::Get_resource  (Sc::Pipeline, kForward_pipeline), 
         wd, ht
         ); 
   }

   //
   //
   void update_input (sy::System_context* sys)
   {
      // 
      sy::Mouse_state      ms; 
      sy::Keyboard_state   ks;    
      sys->Poll_input (ms, ks); 

      if (sy::Is_keydown (sy::SC_Q, ks))
      {
         sys->SysReq (sy::System_context::Req_Exit); 
         return; 
      }

		{	
         // update view movement
			Vec3f vPos, vRot; 
			Sc::Get_pos (vPos, Dx::So(gobys["camera"]));
			Sc::Get_rot (vRot, Dx::So(gobys["camera"])); 

         static float move_rate = 0.5f;

			xp::Update_view_transform (vPos, vRot, move_rate, ks,  ms); 

			Sc::Set_pos (Dx::So(gobys["camera"]), vPos); 
			Sc::Set_rot (Dx::So(gobys["camera"]), vRot); 
		}


   }

   //
   //
   void update_collision (sy::System_context* sys)
   {
      Co::DispatchCollisionEvents (coll_space.get(), 0.0);  
   }




   //
   //
   double _dticker; 
   //
	void update_anim (sy::System_context* sys)
   {
      _dticker += 0.02; 
      Vec3f v_0;

      {  // trench_guy
         Sc::Get_pos (v_0, Dx::So(gobys["trench_guy"])); 
         Sc::Set_pos (Dx::So(gobys["trench_guy"]), v_0); 

         Sc::Get_rot (v_0, Dx::So(gobys["trench_guy"])); 
         Y (v_0) += 4.0f; 
         Sc::Set_rot (Dx::So(gobys["trench_guy"]), v_0); 
      }

      {  // wat
			int ijnt;
         Vec3f actual_vel, desired_vel, v;

         // get the actual_vel

//        wat.Update (
//           Dx::So(gobys["wat"]),
//           coll_space.get(), 
//           actual_vel, 
//           desired_vel
//           ); 
         

			// ijnt = 0;
			// std::map<std::string, int> jointIndMap;
			// for (JointMap::const_iterator it = sg_HumanoidJointMap.begin(); it != sg_HumanoidJointMap.end(); it++)
         // {
			// 	jointIndMap[it->first] = ijnt;
         //    ijnt++; 
         // }


         // ?? build high level controls for spine, legs, arms head+neck ?? 

         Ma::Mat44f old_abs_transf_l, old_abs_transf_r; 

         Ma::Vec3f old_rot_l, old_rot_r, old_pos_l, old_pos_r;

         Sc::Get_abs_transform (old_abs_transf_l, &wat.joint_arr[wat.joint_ind["foot.left" ]]); 
         Sc::Get_abs_transform (old_abs_transf_r, &wat.joint_arr[wat.joint_ind["foot.right"]]); 

         Sc::Get_rot(old_rot_l, &wat.joint_arr[wat.joint_ind["foot.left"]] ); 
         Sc::Get_rot(old_rot_r, &wat.joint_arr[wat.joint_ind["foot.right"]]); 

         Sc::Get_pos(old_pos_l, &wat.joint_arr[wat.joint_ind["foot.left"]] ); 
         Sc::Get_pos(old_pos_r, &wat.joint_arr[wat.joint_ind["foot.right"]]); 


			wat.joint_state[wat.joint_ind["neck"]].rot[0] = 0.1f * std::cos (0.5f * _dticker); 
			wat.joint_state[wat.joint_ind["spine.001"]].rot[0] = 0.1f * std::cos (0.5f * _dticker); 
			wat.joint_state[wat.joint_ind["spine.002"]].rot[0] = 0.1f * std::cos (0.5f * _dticker); 
			wat.joint_state[wat.joint_ind["spine.003"]].rot[0] = 0.1f * std::cos (0.5f * _dticker); 

			wat.joint_state[wat.joint_ind["arm.upper.left" ]].rot[0]  = 0.5f * std::cos (0.5f * _dticker); 
			wat.joint_state[wat.joint_ind["arm.upper.right"]].rot[0] = -0.5f * std::cos (0.5f * _dticker); 
			wat.joint_state[wat.joint_ind["leg.upper.left" ]].rot[2]  = 1.5f ; // * std::cos (0.5f * _dticker + Ma::Pi ) + 0.5f; 
			wat.joint_state[wat.joint_ind["leg.upper.right"]].rot[2] = 1.5f ; // * std::cos (0.5f * _dticker  )         + 0.5f; 
			wat.joint_state[wat.joint_ind["leg.upper.left" ]].rot[0]  = -0.4f ; //* std::cos (1.0f * _dticker); 
			wat.joint_state[wat.joint_ind["leg.upper.left" ]].rot[0]  = -0.4f ; // * std::cos (2.0f * _dticker); 

         wat.joint_state[wat.joint_ind["leg.upper.left" ]].rot[0]  = -0.4f ; //* std::cos (1.0f * _dticker); 
			wat.joint_state[wat.joint_ind["leg.upper.left" ]].rot[0]  = -0.4f ; // * std::cos (2.0f * _dticker); 


         Ma::Mat44f new_abs_transf_l, new_abs_transf_r; 
         Ma::Vec3f new_rot_l, new_rot_r, new_pos_l, new_pos_r; 
         Sc::Get_rot(new_rot_l, &wat.joint_arr[wat.joint_ind["foot.left" ]]); 
         Sc::Get_rot(new_rot_r, &wat.joint_arr[wat.joint_ind["foot.right"]]); 

         Sc::Get_pos (new_pos_l, &wat.joint_arr[wat.joint_ind["foot.left" ]]); 
         Sc::Get_pos (new_pos_r, &wat.joint_arr[wat.joint_ind["foot.right"]]); 

         Sc::Get_abs_transform (new_abs_transf_l, &wat.joint_arr[wat.joint_ind["foot.left" ]]); 
         Sc::Get_abs_transform (new_abs_transf_r, &wat.joint_arr[wat.joint_ind["foot.right"]]); 

			//wat.joint_state["leg.upper.right"].rot[0]  = 0.3f * std::cos (1.0f * _dticker); 
			//wat.joint_state["leg.upper.right"].rot[0] =  0.7f ; // * std::cos (2.0f * _dticker); 


			//wat.joint_state["leg.lower.left"].rot[0]  = 0.5f * std::cos (0.5f * _dticker); 
			//wat.joint_state["leg.lower.right"].rot[0] = -0.5f * std::cos (0.5f * _dticker); 
			wat.joint_state[wat.joint_ind["leg.lower.left"  ]].rot[0]  = -1.5f ; //* std::cos (0.5f * _dticker); 
			wat.joint_state[wat.joint_ind["leg.lower.right" ]].rot[0] = -1.5f; // * std::cos (0.5f * _dticker); 
			wat.joint_state[wat.joint_ind["foot.left"       ]].rot[0]  = 0.3f * std::cos (2.5f * _dticker); 
			wat.joint_state[wat.joint_ind["foot.right"      ]].rot[0] = -0.3f * std::cos(2.5f * _dticker); 
			wat.joint_state[wat.joint_ind["toe.left"        ]].rot[0]  = 0.3f * std::cos (2.5f * _dticker); 
			wat.joint_state[wat.joint_ind["toe.right"       ]].rot[0] = -0.3f * std::cos(2.5f * _dticker); 

         
         Ma::Mat33f m; 
			ijnt = 0;


         // this should be part of the articulator, not in here
			//for (An::Joint_state::Map::const_iterator  it = wat.joint_state.begin(); it != wat.joint_state.end(); it++)
         for (int ij = 0; ij < wat.joint_arr.size(); ij++)
			{
				Ma::Quatf q_, qt; 
				Ma::SetIdent(q_); 		
            
				for (size_t iax = 0; iax < wat.joint_state[ij].def->num_rots; iax++)
				{
               
					Ma::AxisAngle (qt, wat.joint_state[ij].def->axes[iax], wat.joint_state[ij].rot[iax]); 

					Ma::Mult (q_, qt, q_);
				}


				Ma::SetMat33Quat(m, q_); 

				Sc::Joint_orie (&wat.joint_arr[ij], m);
				ijnt++; 
			}

			// wat.joint_state[]
         //
         // Ma::SetMatRotateX (mat, 0.3f * cosf(0.5f * _dticker)); 
         // Ma::Transpose (mat); 
         // Sc::Joint_orie (wat.joint_Map["leg.upper.left"], mat); 
			// 
         // //
         // Ma::SetMatRotateX (mat, 0.3f * -cosf(0.5f * _dticker)); 
         // Ma::Transpose (mat); 
         // Sc::Joint_orie (wat.joint_Map["leg.lower.left"], mat); 
		
      }
   }

   //
   //
   void render (sy::System_context* sys)
   {
	   // h3dSetOption( H3DOptions::DebugViewMode, 1.0f );
	   // h3dSetOption( H3DOptions::WireframeMode, 0.5f );

      Sc::Render_scene (Dx::So(gobys["camera"])); 

		windo->Swap_buffers ();    
   }

public: 

   test_anim (sy::System_context* sys) : init_ (false), _dticker(0.0) {
   }

   virtual ~test_anim () {
   }

	virtual void OnWindowResize (int wd, int ht) 
   {
      resize_render_targets (wd, ht); 
   }

	virtual void OnWindowClose (){}
	virtual void OnWindowActivate	(bool activate) {} 
	
   //
   //
   virtual int Initialize (sy::System_context* sys) { 
      if (init_) 
         return 0; 

      tracker.Reset (sys);

      windo.reset (sys->Create_GraphicsWindow (this, "test SDL2", 800, 600, false)); 
      init_scene (sys); 
      init_collision (sys);
      windo->Show (true); 
      init_ = true; 
      return 0; 
      } 

   //
   //
	virtual int Deinitialize (sy::System_context*) { 
      init_ = false; 
      Sc::Deinitialize_scene (); 
      return 0; 
      } 

   //
   //
	virtual int Update (sy::System_context* sys) { 
      Dt = tracker.GetDt (sys); 
      update_input (sys); 
      update_collision (sys); 
      update_anim (sys); 
      render (sys); 
      return 0; 
      } 

}; 


//
// Main --------------------------
//
int _tmain (int argv, char** argc)
	{
std::shared_ptr<sy::System_context>     sys (sy::Create_context ()); 
std::shared_ptr<sy::RT_window_listener>  rtt (new test_anim(sys.get()));
sy::Run_realtime_task (sys.get(), rtt.get()); 
return 0; 
	}




