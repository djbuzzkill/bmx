#include "Scene.h"


using namespace Ma;
  

// 

    
//
//
  

void Sc::Obj::update_xf () 
{

	if (!node)
	   return;

	h3dSetNodeTransform (
	   node, 
	   Ma::X (pos), Ma::Y (pos), Ma::Z (pos), 
	   Ma::X (rot), Ma::Y (rot), Ma::Z (rot), 
	   Ma::X (scl), Ma::Y (scl), Ma::Z (scl)
	   );
}
   //
void Sc::Obj::bind_ (Obj::ID n)
{
   node = n; 
   if (!node)
	   return;

   h3dGetNodeTransform (
      node, 
	   &Ma::X (pos), &Ma::Y (pos), &Ma::Z (pos), 
	   &Ma::X (rot), &Ma::Y (rot), &Ma::Z (rot), 
	   &Ma::X (scl), &Ma::Y (scl), &Ma::Z (scl));
}


Sc::Obj::Obj () : node (0)
{
}

Sc::Obj::Obj (Obj::ID  n) : node (n)
{
   if (node)
	   bind_ (node); 
}

Sc::Obj::~Obj () {
   }

//
void Sc::Obj::Set_pos (const Ma::Vec3f& p)
{
   Ma::Copy (pos, p); 
   update_xf();
}

void Sc::Obj::Set_rot (const Ma::Vec3f& r)
{
   Ma::Copy (rot, r); 
   update_xf();
}

void Sc::Obj::Set_scl (const Ma::Vec3f& s)
{
   Ma::Copy (scl, s); 
   update_xf();
}

Bn::AABBf& Sc::Obj::Get_AABB (Bn::AABBf& out) 
{
   if (node)
   {
      h3dGetNodeAABB (
         node, 
         out.min_ext.ptr () + 0, out.min_ext.ptr() + 1, out.min_ext.ptr() + 2,
         out.max_ext.ptr() + 0, out.max_ext.ptr() + 1, out.max_ext.ptr() + 2
         );
   }

   return out; 
}

//
//
Ma::Vec3f& Sc::Obj::Get_pos (Ma::Vec3f& p) 
{
   h3dGetNodeTransform (
      node, 
	   &Ma::X (pos), &Ma::Y (pos), &Ma::Z (pos), 
	   &Ma::X (rot), &Ma::Y (rot), &Ma::Z (rot), 
	   &Ma::X (scl), &Ma::Y (scl), &Ma::Z (scl)
      );
   return Ma::Copy (p, this->pos);
} 

//
Ma::Vec3f& Sc::Obj::Get_rot (Ma::Vec3f& r) {

   h3dGetNodeTransform (node, 
	   &Ma::X (pos), &Ma::Y (pos), &Ma::Z (pos), 
	   &Ma::X (rot), &Ma::Y (rot), &Ma::Z (rot), 
	   &Ma::X (scl), &Ma::Y (scl), &Ma::Z (scl));

   return Ma::Copy (r, this->rot);
   } 

//
//
Ma::Vec3f& Sc::Obj::Get_scl (Ma::Vec3f& s) {

   h3dGetNodeTransform (node, 
	   &Ma::X (pos), &Ma::Y (pos), &Ma::Z (pos), 
	   &Ma::X (rot), &Ma::Y (rot), &Ma::Z (rot), 
	   &Ma::X (scl), &Ma::Y (scl), &Ma::Z (scl));

   return Ma::Copy (s, this->scl);
   } 


bool Sc::Obj::Light_params (
      
      float radius, 
      float fov0, 
      
      int   shadowmap_count, 
      float shadowmap_bias, 


      const Ma::Col3f& col, 
      float col_mult)
{

   
   if (H3DNodeTypes::Light != h3dGetNodeType  (node)) 
      return false; 

   h3dSetNodeParamF (node, H3DLight::RadiusF, 0, radius);          
   h3dSetNodeParamF (node, H3DLight::FovF, 0, fov0);         
   h3dSetNodeParamI (node, H3DLight::ShadowMapCountI, shadowmap_count);        
   h3dSetNodeParamF (node, H3DLight::ShadowMapBiasF, 0, shadowmap_bias);  
   h3dSetNodeParamF (node, H3DLight::ColorF3, 0, Ma::X (col) );          
   h3dSetNodeParamF (node, H3DLight::ColorF3, 1, Ma::Z (col) );          
   h3dSetNodeParamF (node, H3DLight::ColorF3, 2, Ma::X (col) );          
   h3dSetNodeParamF (node, H3DLight::ColorMultiplierF, 0, col_mult); 
   return true; 
}

void        Sc::Set_pos  (Obj* o, const Ma::Vec3f& pos) {o->Set_pos (pos);} 
void        Sc::Set_rot  (Obj* o, const Ma::Vec3f& rot) {o->Set_rot (rot); } 
void        Sc::Set_scl  (Obj* o, const Ma::Vec3f& scl) { o->Set_scl (scl); }
Bn::AABBf&  Sc::Get_AABB (Bn::AABBf& out, Obj* o){ return o->Get_AABB(out); }
Ma::Vec3f&  Sc::Get_pos  (Ma::Vec3f& pos, Obj* o){ return o->Get_pos (pos); } 
Ma::Vec3f&  Sc::Get_rot  (Ma::Vec3f& rot, Obj* o){ return o->Get_rot(rot); }
Ma::Vec3f&  Sc::Get_scl  (Ma::Vec3f& scl, Obj* o){ return o->Get_scl (scl);}

void Sc::Bind_node (Obj* o, Obj::ID n) { o->bind_(n); } 
//
//
Ma::Mat44f& Sc::Get_transform  (Obj* o, Ma::Mat44f& mat)
{
   if (o && o->node)
   {
      const Ma::Mat44f* rel_mat = 0; 
      h3dGetNodeTransMats (o->node, (const float**) &rel_mat, 0); 
      return Ma::Copy (mat, *rel_mat); 
   }
   return mat;
}

//
//
void Sc::Set_transform (Obj* o, Ma::Mat44f& mat)
{
   if (o)
   {
      h3dSetNodeTransMat (o->node, mat.ptr()); 
   }
}

//
// 
Ma::Mat44f& Sc::Get_abs_transform (Ma::Mat44f& mat, Obj* o)
{
   if (o && o->node)
   {
      const Ma::Mat44f* abs_mat = 0; 
      h3dGetNodeTransMats (
         o->node, 
         0, 
         (const float**) &abs_mat
         ); 
      return Ma::Copy (mat, *abs_mat); 
   }
   return mat;
}

// lights
bool Sc::Light_params (             
   Obj*              ob,            
   float             radius,           
   float             fov,              
   int               shadowmap_count,  
   float             shadowmap_bias,   
   const Ma::Col3f&  col,                        
   float             col_mult)
{
   return ob->Light_params (radius, fov, shadowmap_count, shadowmap_bias, col, col_mult);
} 

//
Ma::Vec3f&  Sc::Abs_obj_pos (Ma::Vec3f& out, Obj* ob)
{
   const Ma::Vec3f*  abs_pos; 
   const Ma::Mat44f* mabs_; 
   const Ma::Mat44f* mrel_; 

   if (ob && ob->node)
   {
      h3dGetNodeTransMats (
         ob->node, 
         (const float**) &mrel_, 
         (const float**) &mabs_
         ); 

      abs_pos = (const Ma::Vec3f*) (mabs_->ptr() + 12);
      Ma::Copy (out, *abs_pos); 
   }

   return out; 

} 

// camera
bool Sc::Camera_params (
   Obj* cam,

   int vp_x0, int vp_y0, 
   int vp_wd, int vp_ht, 

   float fov   , 
   float z_near, float z_far)
{
   if (H3DNodeTypes::Camera != h3dGetNodeType  (cam->node)) 
      return false; 

	h3dSetNodeParamI  (cam->node, H3DCamera::ViewportXI, vp_x0);
	h3dSetNodeParamI  (cam->node, H3DCamera::ViewportYI, vp_y0);
	h3dSetNodeParamI  (cam->node, H3DCamera::ViewportWidthI,  vp_wd);
	h3dSetNodeParamI  (cam->node, H3DCamera::ViewportHeightI, vp_ht);
	h3dSetupCameraView(cam->node, fov, (float) vp_wd / (float) vp_ht, z_near, z_far);
   return true;
}

struct Scene
{
   //typedef std::map<std::string, H3DRes> ResTable;
	//ResTable res_tbl;
   //
   //typedef std::map<std::string, H3DNode> 
   //   NodeTable;		
	//NodeTable node_tbl;

   typedef std::map<std::string, Sc::Obj*> 
      ObjTable;	
	ObjTable obj_tbl;

   //

}; 



bool Sc::Joint_orie (Obj* o, Ma::Mat33f& orie)
{
   if (H3DNodeTypes::Joint != h3dGetNodeType  (o->node)) 
      return false; 

   Ma::Mat44f mat;

   for (int iY = 0; iY < 3; iY++)
      for (int iX = 0; iX < 3; iX++)
         mat[iY][iX] = orie[iY][iX]; 

   Ma::Set (mat[3], Ma::X(o->pos ), Ma::Y(o->pos), Ma::Z(o->pos), 1.0f);  
   Sc::Set_transform (o, mat); 

   return true; 
}

//
//
static Scene* sg_scene = 0; 




//
//
Sc::Res Sc::Add_resource (const std::string& src, Sc::ResType rt, unsigned int fl)
{
   
   if (sg_scene)
   {
      return h3dAddResource (rt, src.c_str(), fl); 
   }
   
   Assert (0, ""); 
   return 0; 
}

Sc::Res Sc::Get_resource (Sc::ResType restyp, const std::string& name)
{

   int ret = h3dFindResource (restyp, name.c_str()); 
   Assert (ret , "no res"); 
   return ret ; 
}

//
// 
Sc::Obj* Sc::Create_scene_obj (const std::string& name, const std::string& resname)
{
   if (sg_scene && sg_scene->obj_tbl.count(name))
   {
     return 0; 
   } 

   int res_id = h3dFindResource (H3DResTypes::SceneGraph, resname.c_str()); 
   Assert (res_id, "no such resname"); 
   if (sg_scene && res_id)
   {
      //sg_scene->node_tbl[lbl] = 
      Obj::ID n = h3dAddNodes (H3DRootNode, res_id); 
      sg_scene->obj_tbl[name]  = new Sc::Obj (n); 
      return sg_scene->obj_tbl[name];
   }

   Assert (0, "lolz u shud not pass lolz"); 
   return 0; 
}


//
//
Sc::Obj* Sc::Create_camera (const std::string& name, Res pipeline_res)
{
   if (!sg_scene)
      return 0;

   if (h3dFindNodes (H3DRootNode, name.c_str (), H3DNodeTypes::Camera))
      return 0;

   if (sg_scene->obj_tbl.count (name))
      return 0; 

   //return h3dGetNodeFindResult (0); 
   Obj::ID  n = h3dAddCameraNode (H3DRootNode, name.c_str(), pipeline_res);
   sg_scene->obj_tbl[name] = new Sc::Obj (n); 
   return sg_scene->obj_tbl[name] ;

}

//
//
Sc::Obj* Sc::Create_light (const std::string& name, Res lit_mtrl, const std::string& light_context, const std::string& shadow_context)
{
   if (!sg_scene)
      return 0;

   if (h3dFindNodes (H3DRootNode, name.c_str (), H3DNodeTypes::Camera))
      return 0;
 
   if (sg_scene->obj_tbl.count (name))
      return 0; 

   //if (sg_scene->node_tbl.count (name))
   //   return 0; 

   Obj::ID n = h3dAddLightNode (H3DRootNode, name.c_str(), 0, light_context.c_str(), shadow_context.c_str());
   sg_scene->obj_tbl[name] = new Sc::Obj (n); 

   return sg_scene->obj_tbl[name]; 
}

//
//
bool Sc::Destroy_scene_obj (Obj* obj)
{
   for (Scene::ObjTable::iterator it = sg_scene->obj_tbl.begin(); it != sg_scene->obj_tbl.end(); it++)
   {
      if (it->second == obj)
      {
         h3dRemoveNode (it->second->node); 
         delete it->second; 
         sg_scene->obj_tbl.erase (it); 
         return true; 
      }
   }

   Assert (0, "no res"); 
   return false;
}


//
//
bool Sc::Initialize_scene ()
{

   if (!sg_scene)
   {
      h3dInit (); 
      sg_scene = new Scene; 
      
      return true; 
   }

   Assert (0, "no res"); 
   return false; 
}

void Sc::Deinitialize_scene ()
{
   if (sg_scene)
   {
      h3dRelease (); 
      delete sg_scene ; 
      sg_scene = 0; 
   }

} 


int Sc::Joint_index (const Obj& joint)
{
   if (H3DNodeTypes::Joint == h3dGetNodeType  (joint.node)) 
      return h3dGetNodeParamI (joint.node, H3DJoint::JointIndexI); 

   return -1; 
}

const char* Sc::Joint_name  (const Obj& joint)
{
   if (H3DNodeTypes::Joint == h3dGetNodeType  (joint.node)) 
      return h3dGetNodeParamStr (joint.node, H3DNodeParams::NameStr); 

   return 0; 
}

void Sc::Destroy_skeleton_arr(Sc::Obj::Arr& skel_map)
{

   //for (Sc::ObjMap::iterator it = skel_map.begin(); it != skel_map.end(); it++)
   //   delete it->second; 

   skel_map.clear (); 
}

bool Sc::Get_skeleton_arr (Sc::Obj::Arr& joint_arr, Sc::IndMap& joint_ind, Sc::Obj* obj)
{
   joint_arr.clear(); 
   joint_ind.clear();

   if (H3DNodeTypes::Model != h3dGetNodeType  (obj->node)) 
      return false; 


   int num_Joints = h3dFindNodes (obj->node, 0, H3DNodeTypes::Joint); 

   joint_arr.resize (num_Joints); 

   for (int iJoint = 0; iJoint < num_Joints; iJoint++) {
      H3DNode     bone_node   = h3dGetNodeFindResult (iJoint);
      std::string bone_name   = h3dGetNodeParamStr (bone_node, H3DNodeParams::NameStr); 
      
      int jind = h3dGetNodeParamI (bone_node , H3DJoint::JointIndexI);

      Sc::Bind_node (&joint_arr[jind], bone_node); 
         // = new Sc::Obj (bone_node); 

      joint_ind[bone_name] = jind;
   	}
   
   return true;
}

void Sc::Render_scene (Sc::Obj* cam)
{
   h3dRender(cam->node);
   //h3dutShowFrameStats ( 
   //h3dutShowText( "Yo Yo Momma", 10, 10, 2.0, 1.0f, 1.0f, 1.0f, res_Table["generic.mtrl"] );
   // Finish rendering of frame
   h3dFinalizeFrame();
}

//
void Sc::Resize_render_target (Res pipe_res, int wd, int ht)
{
   if (sg_scene && pipe_res)
	   h3dResizePipelineBuffers (pipe_res, wd , ht);
}


   