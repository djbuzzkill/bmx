#ifndef hello_Scene
#define hello_Scene


#include "VecMath.h"
#include "Bounding.h"

#include "Horde3D.h"
#include <string>


namespace Sc 
{


   // scan terrain, predict contacts
   // 3-axis hips, 
   // realtime decision , update goal
   //

   struct Obj  
      {
      typedef std::map<std::string, Obj*> Map;

      typedef std::vector<Obj>            Arr;
      typedef int                         ID;

      void update_xf ();
      void bind_ (Obj::ID id);

	   ID             node; 
	   Ma::Vec3f pos, rot, scl; 

      Obj ();
      Obj (H3DNode n);
      ~Obj ();   //
      void Bind (Obj::ID n);
     void Set_pos (const Ma::Vec3f& p);
      void Set_rot (const Ma::Vec3f& r);
      void Set_scl (const Ma::Vec3f& s);
      Bn::AABBf& Get_AABB (Bn::AABBf& out);    //
      Ma::Vec3f& Get_pos (Ma::Vec3f& p) ; 
      Ma::Vec3f& Get_rot (Ma::Vec3f& r) ;
      Ma::Vec3f& Get_scl (Ma::Vec3f& s) ;

      bool Light_params (
         
         float radius, 
         float fov0, 
         
         int   shadowmap_count, 
         float shadowmap_bias, 

         const Ma::Col3f& col, 
         float col_mult);


   }; 

   typedef int                         Res;
   //typedef std::map<std::string, Obj*> ObjMap;
   typedef std::map<std::string, int>  IndMap;

   // overload our enum (for everything?)
	enum ResType {
      // but the beginning order should match horde res types?
		Undefined = 0,
		SceneGraph,
		Geometry,
		Animation,
		Material,
		Code,
		Shader,
		Texture,
		ParticleEffect,
		Pipeline, 

      // 
      Camera, 
      Light, 
      Joint, 
      Emitter, 
	   };

   void        Bind_node(Obj* o, H3DNode n);
	void        Set_pos  (Obj*, const Ma::Vec3f& pos); 
	void        Set_rot  (Obj*, const Ma::Vec3f& rot); 
	void        Set_scl  (Obj*, const Ma::Vec3f& scl); 

	Ma::Mat44f& Get_transform  (Obj*, Ma::Mat44f& mat); 
	void        Set_transform  (Obj*, Ma::Mat44f& mat); 

	Ma::Mat44f& Get_abs_transform  (Ma::Mat44f& mat, Obj*); 


   Bn::AABBf&  Get_AABB    (Bn::AABBf& out, Obj*);
	Ma::Vec3f&  Get_pos     (Ma::Vec3f& pos, Obj*); 
	Ma::Vec3f&  Get_rot     (Ma::Vec3f& rot, Obj*); 
	Ma::Vec3f&  Get_scl     (Ma::Vec3f& scl, Obj*); 
   Ma::Vec3f&  Abs_obj_pos (Ma::Vec3f& out, Obj* ob); 

   // lights
   bool Light_params (
      Obj*,
      float radius, float fov, 
      int shadowmap_count, 
      float shadowmap_bias, 
      const Ma::Col3f& col3f, 
      float col_mult
      ); 
   
   // camera
   bool Camera_params (
      Obj* cam,
      // viewport
      int vp_x0, int vp_y0, 
      int vp_wd, int vp_ht, 
      // frustum
      float fov   , 
      float z_near, float z_far
      ); 

   // Joints        
   bool Joint_orie (
      Obj* o, 
      Ma::Mat33f& orie
      ); 
 
    //
   bool Get_skeleton_arr (
      Obj::Arr& skel_map, 
      IndMap& joint_ind, 
      Sc::Obj* model_obj);

   void Destroy_skeleton_arr (
      Obj::Arr& skel_arr);

   int         Joint_index (const Obj& joint); 
   const char* Joint_name  (const Obj& joint); 

   //
   

   bool  Initialize_scene  (); 
   void  Deinitialize_scene(); 
   Res   Add_resource      (const std::string& src, ResType restype, unsigned int); 
   Res   Get_resource      (ResType restyp, const std::string& src); 

   Obj*  Create_scene_obj  (const std::string& name, const std::string& res_name); 
   Obj*  Create_camera     (const std::string& name, Res pipeline_res); 
   Obj*  Create_light      (const std::string& name, Res lit_mtrl, const std::string& light_context, const std::string& shadow_context); 

   //Obj* Create_joint
   //Obj* Create_emitter
   void Resize_render_target (Res pipeline, int wd, int ht); 
   void Render_scene       (Sc::Obj* cam); 

   bool  Destroy_scene_obj (Obj* obj); 

}

#endif
