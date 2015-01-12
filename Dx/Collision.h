#ifndef introducing_Collision
#define introducing_Collision

#include "VecMath.h"
#include "Bounding.h"

namespace Co
{
   struct Ray_query;
   class Collider;
   class Collision_space;


   //
   // 
   struct Raycast_result
   {
	   Raycast_result (const Ma::Vec3f& ray_from, const Ma::Vec3f& ray_to)
	   :	rayFrom (ray_from), rayTo (ray_to)
	   {
      }

	   // :::::::::::::::::::::::::
	   float hitTime;
	   const Ma::Vec3f& rayFrom;  
	   const Ma::Vec3f& rayTo;    
	   Collider* who;
	   bool hasHit;
   };

   //
   bool Raycast (Raycast_result& query, Collision_space* cs, const Ma::Vec3f& ray_from, const Ma::Vec3f& ray_to);

   //
   //
   class Collider 
      { 
   protected:

      Collider () {}
   public: 
      virtual int          On_contact     (Collider* other, const Ma::Vec3f& pos, const Ma::Vec3f& nrm, float dist) = 0; 

      virtual Ma::Vec3f&   Get_position   (Ma::Vec3f& pos)  = 0; 
      virtual const char*  Get_label ()  = 0; 
      }; 


   //
   // collider shapes
   struct Heightfield_params
   {
      Ma::Vec3f   vmin, vmax;
      float       U_size, V_size;
      int         U_dim, V_dim;
      const float* data;
   }; 

   struct TriMesh_params
   {
      int                     num_inds;
      const unsigned short*   inds;
      const Ma::Vec3f*        verts;

   };

   //
   // 
   Collision_space*  Create_collision_space     (const Ma::Vec3f& vmin, const Ma::Vec3f& vmax); 
   void              Destroy_collision_space    (Collision_space*); 
   int               Add_collider_capsule       (Co::Collision_space*,  Collider* me, int maxjor_axis, float radius, float length); 
   int               Add_collider_height_field  (Co::Collision_space*,  Collider* me, const Heightfield_params& params); 
   int               Add_collider_tri_mesh      (Co::Collision_space*,  Collider* me, const TriMesh_params& params); 
   int               Remove_collider            (Collider* , Co::Collision_space*); 
   int               DispatchCollisionEvents    (Co::Collision_space* , double Dt);



}
 
#endif
