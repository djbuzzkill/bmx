#ifndef hello_anim_biped
#define hello_anim_biped

#include <map>
#include <string>


#include "VecMath.h"
#include "Shapes.h"

//#include "Horde3D.h"
//#include "Horde3DUtils.h"

//#include "Scene.h"
//#include "Bounding.h"


//
//
namespace Co
   {
class Collision_space;  
   }




//
//
namespace An
{

   struct Joint_def 
   {
	   std::string name;
	   int			num_rots; 
	   Ma::Vec3f	axes[3]; 

      typedef std::map<std::string, const Joint_def*> Map;

   };


   //
   // bone articulation
   //
   namespace Joint_state
   {
	   struct Axes
	   {
         const Joint_def*  def; 
		   float		         rot[3];
	   };

      typedef std::map<std::string, Axes> Map; 
      typedef std::vector<Axes>           Arr; 

	   Axes& Initialize (Axes& jnt, const Joint_def* jd, float a, float b, float c);
   }

   //
   // Articulator - anim system interface
   struct Articulator 
      { 
      // Init  
      // ? only a scene object ?
      //
      virtual int Init  (Sc::Obj*, const Joint_def::Map& joint_map) = 0; 
      
      // Update 
      // ? what about just passing it data instead of collsp ?
      //
      virtual int Update (Sc::Obj*, Co::Collision_space* collsp, Ma::Vec3f const& curr_v, Ma::Vec3f const& desr_v) = 0; 

      // Deinit 
      // ...
      virtual int Deinit () = 0;
      }; 

   //
   //    deprecate
   
   // proto
   struct Leg_bind     ; 
   struct Leg_dyna     ; 
   struct Biped        ; 

   struct Dynamic_state; 
   struct Humanoid_articulator;

   //
   enum Motion_state {
   MS_null_state_ = 0, 
   MS_fall, 
   MS_land_vert, 
   MS_idle, 
   MS_walk, 
   MS_jump, 
   MS_run, 
   MS_stop_run, 
   MS_combat_ready, 
   MS_attack_0, 
   }; 

   //
   //
   struct Leg_bind 
   {
      //
      // hip, knee, heel, ball
      //
      // float     deltas[4]; // can we always start at 0?
      Ma::Vec3f points[4];    // binding joint positions
      float radius[4];    // of the joint
      float toe_dist; 
   };

   //
   // computed once, should not change, 
   // should be merged with Leg_bind?
   struct Stride_extents {

      // not axis, but its min/max
      struct { Ma::Vec2f 
         axis[3]; 
             } hip, knee, ankle, toe; 

      // height/radius
      Ma::Vec2f walk, run; 
      }; 

   
   inline float Min (const Ma::Vec2f& x) { return Ma::X (x); }
   inline float Max (const Ma::Vec2f& x) { return Ma::Y (x); }
   inline float Hgt (const Ma::Vec2f& x) { return Ma::X (x); }
   inline float Rad (const Ma::Vec2f& x) { return Ma::Y (x); }

   inline float& Min (Ma::Vec2f& x) { return Ma::X (x); }
   inline float& Max (Ma::Vec2f& x) { return Ma::Y (x); }
   inline float& Hgt (Ma::Vec2f& x) { return Ma::X (x); }
   inline float& Rad (Ma::Vec2f& x) { return Ma::Y (x); }

 



   //
   // 
   struct Rot_axis  {
      int   ind;
      float radians; 
      };

   //
   // 
   struct Leg_dyna 
   {
      enum State {
      // 
      LS_brace_land, 
      //                // concurrent oppositie state
      LS_fw_aim,        // LS_fw_push, 
      LS_fw_landcontact,// LS_fw_recoil,
      LS_fw_flatsupport,// LS_fw_aim,  
      LS_fw_push,       // LS_fw_landcontact,
      LS_fw_recoil,     // LS_fw_flatsupport,
      } state;




// LS_fw_aim,           // contact aim
// LS_fw_landcontact,   // contact.inital -> contact.final 
// LS_fw_flatsupport,   // flatsupport 
// LS_fw_push,          // push, Compute_next_step, 
// LS_fw_recoil,        // recoil contact aim


      // State - leg state independent of the other leg
      // preseve continuity by maintaining pos angle
      // when transitioning direct -> offset, offset -> direct
      // heel pos is calculated directly if contact on ground
      // else based on ball ground contact offs 
      // offset - LS_brace_land
      // direct - LS_fw_aim,           
      // direct - LS_fw_landcontact,
      // direct - LS_fw_flatsupport, 
      // offset - LS_fw_push, 
      // direct - LS_fw_recoil,



      // absolute angles
      // define pose when applied to bind state
      struct { 
      int         num_rots;   // <= 3
      Rot_axis    seq[3];     // may not be all used
      } angles[4]; 

      //float       flat_contact_angle;       

      // IK solution
      float       deltas[4];           // !! hip will always be considered fixed (at <0>), so this should always be at most 3
      Ma::Vec2f   chain[4];            // hip, knee, heel, ball in 2D IK space


      // flat_contact_angle would be 0 on flat ground
      //  +       <- leg upper 
      //  |       
      //  |       
      //  +       <- leg lower (knee)
      //  |
      //  |
      //  +--+
      //
      // neg on downward slope
      //  +
      //  |
      //  |
      //  |
      //  +
      //  |
      //  |
      //  +  <- neg theta 
      //   \
      //    +
      //            

      // boolean flags       
      struct { bool        
      ball_contact,     // 
      heel_contact,     // 
      ball_target_pos,  //
      toe_pointed_pos,  //
      foot_pointed_pos; //
      } is; 


      union {
         struct { // LS_brace_land,
            Ma::Vec3f target_ball; 
            };

         struct { // LS_fwd_aim, 
            Ma::Vec3f target_heel;
            }; 

         struct { // LS_fwd_landcontact,
         }; 

         struct { // LS_fwd_flatsupport, 
         }; 

         struct { // LS_fwd_push, 
         }; 

         struct { // LS_fwd_recoil,
         }; 
      }; 

   }; 


   //
   //
   // Compute_contact_projection  - 
   bool Find_contact_projection (
      Ma::Vec2f&  heel_contact, 
      float&      incline, 

      const Leg_dyna&   leg, 
      const Ma::Vec2f&  front, 
      const Ma::Vec2f&  rear
      ); 


   // 
   // 
   // and instantaneous grid with model at origin
   // do we even need a Nav_grid_struct? 

   struct Nav_grid {
      int   dim_Left,   dim_Fwd;
      float step_Left,  step_Fwd; 
      }; 

   // build a grid in local model space, 
   // disregard Y(vPos), only use X,Z
   Nav_grid& Build_grid (
      Nav_grid& grid, 
      const Ma::Vec3f& vPos, 
      const Ma::Vec3f& vFwd, 
      const Ma::Vec3f& vLeft); 

   // this will ray cast the ground and return the vertical (Y) height at 
   // vFwd + vLeft since we can calculate any point as we need, we dont 
   // need Build_grid  
   float Height_at_point (          
      Ma::Vec3f&        pnt,     
      const Ma::Vec3f&  vPos,     
      const Ma::Vec3f&  vFwd,     
      const Ma::Vec3f&  vLeft
      );  
   // we dont even need this, just raycast to find the height at point.  
   //
   // you will already have U,V of height if know which point to raycast
   
   //
   // Biped 
   struct Biped 
   { 
      static float kFull_extension;    // we'll say its 99% of bind height
      static float kMax_compression;   // 94% 
      static float kMin_compression;   // 98% 
      static float kTargetAim_rate;    // move into ready contact regardless of progress interp.

      static float kLight_compression;    // 99% ??
      static float kModerate_compression; // 90% ??
      static float kHeavy_compression;    // 80% ??
      static float kSevere_compression;   // 70% ??

      Motion_state req_state; 
      Motion_state state; 

      Ma::Vec3f 
         root_pos; 

      struct {
         Leg_bind    bind; 
         Leg_dyna    dyna; 
         } L, R; 
   }; 



   //
   //
   struct Dynamic_state 
   {
      // current motion state
      // acceleration, velocity,.. 
      Ma::Vec3f   prev_velo;
      Ma::Vec3f   dir; 
      int         power_leg; 
   }; 


   //
   // Humanoid_articulator 
   struct Humanoid_artic : public Articulator
   {
   public: 

      virtual int Init   (Sc::Obj* o, const Joint_def::Map& joint_map); 
      virtual int Update (Sc::Obj* model_obj, Co::Collision_space* collsp, Ma::Vec3f const& curr_dx, Ma::Vec3f const& desired_dx); 
      virtual int Deinit (); 

      Dynamic_state                 ds;
      Sc::Obj::Arr                  joint_arr;
      Sc::IndMap                    joint_ind;
		Joint_state::Arr              joint_state; 

      // Sc::IndMap     ind_Map;
      Biped          legs;
      Ma::Vec3f      min_compr; 
      Ma::Vec3f      max_compr;
   };


   //
   // Vec_ball_2_heel :: leg -> foot length in IK space
   Ma::Vec2f& Vec_ball_2_heel (
      Ma::Vec2f& out, 
      const Leg_bind& leg
      ); 


   //
   //
   Ma::Vec2f& Rot_foot (
      Ma::Vec2f& out ,  
      const Ma::Vec2f& vfoot, 
      const Ma::Vec2f& voffs, 
      float theta
      ); 


   //
   // use this when transitioning
   Ma::Vec2f& Calc_ball_pos (
      Ma::Vec2f& out, 
      const An::Leg_bind& 
      leg, float theta
      ); 

   //
   //
    Ma::Vec2f& Calc_heel_pos (
       Ma::Vec2f& out, 
       const Leg_bind& leg, 
       float theta
       ); 

   //
   //
   bool Compute_leg_params (
      Leg_bind& legL, 
      Leg_bind& legR, 
      Sc::Obj::Map& bmap
      ); 

   //
   Biped& Init (
      Biped& biped, 
      std::map<std::string, Ma::Vec3f>& abs_pos_map, 
      const Bn::AABBf&  
      ); 
 
   //
   Biped& Update (
      Biped&            biped, 
      Dynamic_state&    ds,
      Ma::Vec3f&        actual_dx, 

      const Ma::Vec3f&  desired_dx,
      const Sc::Obj*    model_obj
      );  

   //

   //
   // no more locomotion state, idle, walk, run, etc.. will be determined by 
   // desired motion, direction, speed, etc
   // class Locomotion_state {  virtual int Update () = 0; }; 
} // An

#endif