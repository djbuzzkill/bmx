#include "Anim_biped.h"
#include "IKSolver2d.h"

#include <glm/glm.hpp>

using namespace Ma;

//
float An::Biped::kFull_extension  = 0.9999f; 
float An::Biped::kMax_compression = 0.9445f;
float An::Biped::kMin_compression = 0.9889f; 
float An::Biped::kTargetAim_rate  = 0.1000f; 

float An::Biped::kLight_compression    = 0.99f;
float An::Biped::kModerate_compression = 0.90f;
float An::Biped::kHeavy_compression    = 0.80f;
float An::Biped::kSevere_compression   = 0.70f;

//
// 
enum PowerLeg {
   PL_left = -1, 
   PL_right, 
   PL_none, 
   PL_both,
   }; 



//
//
An::Joint_state::Axes& An::Joint_state::Initialize (
   An::Joint_state::Axes& jnt, 
   const An::Joint_def* jd, 
   float a, 
   float b, 
   float c)
{
   jnt.def = jd;
   jnt.rot[0] = a;
   jnt.rot[1] = b;
   jnt.rot[2] = c;

   return jnt;
}


//
//
Ma::Vec2f& An::Calc_ball_pos (Ma::Vec2f& out, const An::Leg_bind& leg, float theta)
{
   Assert (0, "lolz"); 
   return out; 
}
   
//
//
Ma::Vec2f& An::Calc_heel_pos (Ma::Vec2f& out, const An::Leg_bind& leg, float theta)
{
   Ma::Vec2f v_foot; 
   Ma::Mat22f M;

   Ma::Set(v_foot, Ma::Z(leg.points[3]) - Ma::Z(leg.points[2]), Ma::Y(leg.points[3]) - Ma::Y(leg.points[2])); 
   Ma::SetMatRotateZ (M, theta);

      
   Ma::Mult (out, M, v_foot); 

   float len_foot = Ma::Length (v_foot); 
   float len_out =  Ma::Length (out); 

   return out;
}

//
//
Ma::Vec2f& An::Vec_ball_2_heel (
   Ma::Vec2f& out, 
   const Leg_bind& leg)
{
   return Ma::Set(out, Ma::Z(leg.points[2]) - Ma::Z(leg.points[3]), Ma::Y(leg.points[2]) - Ma::Y(leg.points[3])); 
} 

//
//
Ma::Vec2f& An::Rot_foot (Ma::Vec2f& out ,  const Ma::Vec2f& vfoot, const Ma::Vec2f& voffs, float theta) 
{
   Ma::Mat22f M;
   Ma::Mult (out, Ma::SetMatRotateZ (M, theta), vfoot); 
   return Ma::Incr (out, voffs); 
}


//
//Ma::Vec3f& An::Abs_node_pos (Ma::Vec3f& out, const std::string& name, H3DNodeMap& bmap)
//{
//   const Ma::Vec3f*  rel_pos; 
//   const Ma::Mat44f* mabs_; 
//   const Ma::Mat44f* mrel_; 
//
//
//   if (bmap.count (name))
//   {
//      h3dGetNodeTransMats (
//         bmap[name], 
//         (const float**) &mrel_, 
//         (const float**) &mabs_
//         ); 
//
//      rel_pos = (const Ma::Vec3f*) (mabs_->ptr() + 12);
//      Ma::Copy (out, *rel_pos); 
//   }
//
//   return out; 
//}


//
// Ma::Vec3f& An::Abs_node_pos (Ma::Vec3f& out, Sc::Node nd)
// {
//    const Ma::Vec3f*  rel_pos; 
//    const Ma::Mat44f* mabs_; 
//    const Ma::Mat44f* mrel_; 
// 
// 
//    if (nd)
//    {
//       h3dGetNodeTransMats (
//          nd, 
//          (const float**) &mrel_, 
//          (const float**) &mabs_
//          ); 
// 
//       rel_pos = (const Ma::Vec3f*) (mabs_->ptr() + 12);
//       Ma::Copy (out, *rel_pos); 
//    }
// 
//    return out; 
// }



bool Init_leg_params (An::Leg_bind& legL, An::Leg_bind& legR, std::map<std::string, Vec3f>& abs_pos_map)
{
   Ma::Vec3f leg_points[4]; 

   Copy (leg_points[0], abs_pos_map["leg.upper.left"]);
   Copy (leg_points[1], abs_pos_map["leg.lower.left"]);
   Copy (leg_points[2], abs_pos_map["foot.left"]);
   Copy (leg_points[3], abs_pos_map["toe.left"] );

   for (int i = 0; i < 4; i++)
   {
      Sub (legL.points[i], leg_points[i], leg_points[0]);
   }

   Copy (leg_points[0], abs_pos_map["leg.upper.right"]); 
   Copy (leg_points[1], abs_pos_map["leg.lower.right"]); 
   Copy (leg_points[2], abs_pos_map["foot.right"]); 
   Copy (leg_points[3], abs_pos_map["toe.right"] ); 

   for (int i = 0; i < 4; i++)
   {
      Sub (legR.points[i], leg_points[i], leg_points[0]);
   }

   return true; 
}
//
// deprecated


bool An::Compute_leg_params (Leg_bind& legL, Leg_bind& legR, Sc::Obj::Map& bmap)
{


   Ma::Vec3f leg_points[4]; 
   const Ma::Vec3f*  rel_pos; 
   const Ma::Mat44f* mabs_; 
   const Ma::Mat44f* mrel_; 

   h3dGetNodeTransMats (
      bmap["root"]->node, 
      (const float**) &mrel_, 
      (const float**) &mabs_
      ); 
   
   //         
   // left side
   h3dGetNodeTransMats (
      bmap["leg.upper.left"]->node,
      (const float**) &mrel_, 
      (const float**) &mabs_
      ); 

   rel_pos = (const Ma::Vec3f*) (mabs_->ptr() + 12);
   Ma::Copy (leg_points[0], *rel_pos); 


   h3dGetNodeTransMats (
      bmap["leg.lower.left"]->node,
      (const float**) &mrel_, 
      (const float**) &mabs_
      ); 

   rel_pos = (const Ma::Vec3f*) (mabs_->ptr() + 12);
   Ma::Copy (leg_points[1], *rel_pos); 


   h3dGetNodeTransMats (
      bmap["foot.left"]->node,
      (const float**) &mrel_, 
      (const float**) &mabs_
      ); 

   rel_pos = (const Ma::Vec3f*) (mabs_->ptr() + 12);
   Ma::Copy (leg_points[2], *rel_pos); 

   h3dGetNodeTransMats (
      bmap["toe.left"]->node,
      (const float**) &mrel_, 
      (const float**) &mabs_
      ); 

   rel_pos = (const Ma::Vec3f*) (mabs_->ptr() + 12);
   Ma::Copy (leg_points[3], *rel_pos); 


   // relative to upper leg 
   for (int i = 0; i < 4; i++)
      Ma::Sub ( legL.points[i], leg_points[i], leg_points[0]); 
   // vector points at the heel
   // Ma::Sub (legL.v_ball2heel, leg_points[2], leg_points[3]); 

   // ankle_bend. so we can subtract it out
   {  
      Ma::Vec3f v_tibia, v_foot;
      Ma::Vec2f a, b;
      Ma::Sub (v_tibia, leg_points[1], leg_points[2]); 
      Ma::Sub (v_foot, leg_points[3], leg_points[2]); 

      Ma::Set (a, Ma::Z(v_foot), Ma::Y(v_foot));
      Ma::Set (b, Ma::Z(v_tibia), Ma::Y(v_tibia));
      // legL.ankle_bend = Ik::Angle_subtend (a,b); 
   }


   //
   // right side
   h3dGetNodeTransMats (
      bmap["leg.upper.right"]->node,
      (const float**) &mrel_, 
      (const float**) &mabs_
      ); 

   rel_pos = (const Ma::Vec3f*) (mabs_->ptr() + 12);
   Ma::Copy (leg_points[0], *rel_pos); 


   h3dGetNodeTransMats (
      bmap["leg.lower.right"]->node,
      (const float**) &mrel_, 
      (const float**) &mabs_
      ); 

   rel_pos = (const Ma::Vec3f*) (mabs_->ptr() + 12);
   Ma::Copy (leg_points[1], *rel_pos); 


   h3dGetNodeTransMats (
      bmap["foot.right"]->node,
      (const float**) &mrel_, 
      (const float**) &mabs_
      ); 

   rel_pos = (const Ma::Vec3f*) (mabs_->ptr() + 12);
   Ma::Copy (leg_points[2], *rel_pos); 

   h3dGetNodeTransMats (
      bmap["toe.right"]->node,
      (const float**) &mrel_, 
      (const float**) &mabs_
      ); 

   rel_pos = (const Ma::Vec3f*) (mabs_->ptr() + 12);
   Ma::Copy (leg_points[3], *rel_pos); 

   // 
   for (int i = 0; i < 4; i++)
      Ma::Sub (legR.points[i], leg_points[i], leg_points[0]); 

   // vector points at the heel
   // Ma::Sub (legR.v_ball2heel, leg_points[2], leg_points[3]); 

   // ankle_bend. so we can subtract it out  
   {  
      Ma::Vec3f v_tibia, v_foot;
      Ma::Vec2f a, b;
      Ma::Sub (v_tibia, leg_points[1], leg_points[2]); 
      Ma::Sub (v_foot, leg_points[3], leg_points[2]); 

      Ma::Set (a, Ma::Z(v_foot), Ma::Y(v_foot));
      Ma::Set (b, Ma::Z(v_tibia), Ma::Y(v_tibia));
      //legL.ankle_bend = Ik::Angle_subtend (a,b); 
   }

   //
   // leg{R|L}.points computed, not compute remaining params
   return true; 
               
}




//
An::Biped& An::Init (An::Biped& biped, std::map<std::string, Vec3f>& abs_pos, const Bn::AABBf& aabb)
{
   // this assumes based skel position eq pos-z

   Ma::Vec3f leg_points[4]; 
   // upper hip is the fixed IK origin
   // subtract everything wrt/it
   Copy (leg_points[0], abs_pos["leg.upper.left"]);
   Copy (leg_points[1], abs_pos["leg.lower.left"]);
   Copy (leg_points[2], abs_pos["foot.left"]);
   Copy (leg_points[3], abs_pos["toe.left"] );
   for (int i = 0; i < 4; i++)
      Sub (biped.L.bind.points[i], 
            leg_points[i], leg_points[0]);

   Copy (leg_points[0], abs_pos["leg.upper.right"]); 
   Copy (leg_points[1], abs_pos["leg.lower.right"]); 
   Copy (leg_points[2], abs_pos["foot.right"]); 
   Copy (leg_points[3], abs_pos["toe.right"] ); 

   // R'bind'pos = curr'joint - pelvis'joint
   for (int i = 0; i < 4; i++)
      Sub (biped.R.bind.points[i], 
            leg_points[i], leg_points[0]);



   Vec3f head_pos, a, b,
         left_heel_pos, right_heel_pos, 
         left_ball_pos, right_ball_pos;


   Vec3f& head = abs_pos["head"]; 
   Vec3f& root_bone = abs_pos["root"]; 

   Sub (a, aabb.min_ext , root_bone); 
   Sub (head_pos, head, root_bone); 

   Vec3f& foot_left        = abs_pos["foot.left"]; 
   Vec3f& toe_left        = abs_pos["toe.left"]; 
   Sub (left_heel_pos, foot_left, root_bone); 
   Sub (left_ball_pos, toe_left, root_bone); 

   biped.L.bind.radius[0] = 3.0f;
   biped.L.bind.radius[1] = 2.0f;
   biped.L.bind.radius[2] = Y(left_heel_pos) - Y(a);
   biped.L.bind.radius[3] = Y(left_ball_pos) - Y(a);

   Vec3f& foot_right = abs_pos["foot.right"]; 
   Vec3f& toe_right  = abs_pos["toe.right"];  
   Sub (right_heel_pos, foot_right, root_bone); 
   Sub (right_ball_pos, toe_right, root_bone); 

   biped.R.bind.radius[0] = 3.0f;  // dont know yet
   biped.R.bind.radius[1] = 2.0f;  // just use these for now
   biped.R.bind.radius[2] = Y(right_heel_pos) - Y(a);
   biped.R.bind.radius[3] = Y(right_ball_pos) - Y(a);

   // dynamic params
   for (int i = 0; i < 4; i++)
   {
      // copy into ik space
      Set (biped.L.dyna.chain[i] , Z(biped.L.bind.points[i]), Y(biped.L.bind.points[i]) );
      Set (biped.R.dyna.chain[i] , Z(biped.R.bind.points[i]), Y(biped.R.bind.points[i]) );
      biped.L.dyna.deltas[i] = 0.0f;
      biped.R.dyna.deltas[i] = 0.0f;
   }

   //Set (biped.L.dyna.ball, 0.0f, 0.0f, 0.0f); 
   //Set (biped.R.dyna.pos_heel, 0.0f, 0.0f, 0.0f); 

   biped.L.dyna.is.ball_contact        = false; 
   biped.L.dyna.is.heel_contact        = false; 

   biped.L.dyna.is.ball_target_pos  = false;
   biped.L.dyna.is.toe_pointed_pos  = false;
   biped.L.dyna.is.foot_pointed_pos = false;
   biped.L.dyna.state = Leg_dyna::LS_brace_land; 
   
   biped.R.dyna.is.ball_contact = false; 
   biped.R.dyna.is.heel_contact = false; 

   biped.R.dyna.is.ball_target_pos  = false;
   biped.R.dyna.is.toe_pointed_pos  = false;
   biped.R.dyna.is.foot_pointed_pos = false;
   biped.R.dyna.state = Leg_dyna::LS_brace_land; 

   biped.state = MS_fall; 

   return biped;
}
 
//
//
// pri_node    - this must be parent of bones
// bone_Map    - these nodes will change 
// biped       - maintain state to modify nodes
// actual_dx   - how much we just moved, pri_node is changed with this value
// desired_dx  - how much we want to move
bool Is_idle (An::Biped& bp) 
{
   return false; 
} 


//
// 
bool Is_falling (An::Biped& bp)
{
   return false;
}


// 
// 
// front/rear - raycast contacts vertical intersections. 
bool An::Find_contact_projection (
   // contact of heel to ground
   Ma::Vec2f&  heel_contact, 
   // angle for flat contact 
   float&      incline, 

   // input          
   const Leg_dyna&   leg, 
   const Ma::Vec2f&  front, 
   const Ma::Vec2f&  rear)
{

   return false; 
}

//
void update_leg (An::Leg_dyna& l, int is_right, const An::Leg_dyna& al, An::Motion_state ms) 
{
   const An::Leg_dyna* le[2] = {
      (is_right ? &al : &l),
      (is_right ? &l : &al) 
      };     

   // transition dependant on current state
   //switch (ms)
   //{
   //
   //   ////
   //   //
   //   case An::MS_run: 
   //      // transition dependant on current state
   //      if (ms != An::MS_stop_run)
   //      {
   //         // should we do this? 
   //         b.req_state = An::MS_null_state_; 
   //      }
   //   break;
   //
   //
   //
   //   ////
   //   //.
   //   case An::MS_jump: 
   //   break;
   //}
   //
   switch (ms)
   {
   case An::MS_fall: 
      if (l.is.ball_contact)
      {}
      if (al.is.ball_contact)
      {
         // setup land params
         // b.state = An::MS_land_vert; 
      }
      else
      {
         switch (l.state)
         {
         //
         //// 
         case An::Leg_dyna::LS_brace_land: 
            if (!l.is.ball_contact)
            {
               // compute target pos
               if (!l.is.ball_target_pos) 
               {
                  // 
               }
            }
            else
            {

            }
            break;

         //
         ////
         case An::Leg_dyna::LS_fw_landcontact: 
            break;

         //
         ////
         default: 
            Assert (0, "how?"); 
            break;
         }
      }
   break; // Biped::BPS_fall: 

   //
   //// MS_land_vert
   case An::MS_land_vert: 
      break; // MS_land_vert

   //
   //// MS_idle  
   case An::MS_idle: 
      break; // MS_idle: 

   case An::MS_walk: break; 
   case An::MS_jump: break; 
   case An::MS_run: 
      {
         // b.req_state = An::MS_null_state_ ;
      }
      break; 
   case An::MS_stop_run: break; 
   case An::MS_combat_ready: break; 
   case An::MS_attack_0: break; 
   }    

}

//
// Update :: (Biped b) => b -> b 
An::Biped& An::Update (

      An::Biped&        b, 
      Dynamic_state&    ds,
      Ma::Vec3f&        actual_dx, 

      const Ma::Vec3f&  desired_dx,
      const Sc::Obj*    model_ob)
{
   

   //PL_left = -1, 
   //PL_right, 
   //PL_none, 
   //PL_both,

   update_leg (b.L.dyna, 0, b.R.dyna, b.state);
   update_leg (b.R.dyna, 1, b.L.dyna, b.state);

   Is_idle (b); 
   Is_falling (b);

   //desired_dx,
   //ds.prev_velo
   // PL_left:
   // PL_right:
   // PL_none:
   // PL_both:
   // biped.L.dyna;
   // biped.R.dyna; 
   // actual_dx; 

   {  // IK solved in yz-space 
      // theta negative because coords applied in xy-space
      Vec3f v;

      //Ik::Solve_2D (biped.L.dyna.chain + 1, biped.L.dyna.deltas, 2, heel_L); 
      //
      //Ik::Solve_2D (biped.R.dyna.chain + 1, biped.R.dyna.deltas, 2, heel_R); 
      
      //float hip_angl = Rads2Degr (wat.legs.L.dyna.deltas[0]); 
      //wat.joint_Map["leg.upper.left"]->Set_rot (Set (v, 15.0, 0.0f, 0.0f)); 
   
      //float knee_angl = Rads2Degr (wat.legs.L.dyna.deltas[1]);  
      //wat.joint_Map["leg.lower.left"]->Set_rot (Set (v, -15.0f, 0.0f, 0.0f));
   
      //float foot_ang= // Rads2Degr (wat.biped.L.dyna.deltas[1]);  
      
      //float ankle_ang = hip_angl + knee_angl + Rads2Degr (foot_ang_);
      //wat.joint_Map["foot.left"]->Set_rot (Set (v_0, ankle_ang , 0.0f, 0.0f));
   
      //float ball_ang = - Rads2Degr (foot_ang_);
      //   //float knee_angl = Rads2Degr (wat.biped.L.dyna.deltas[1]);  
      //wat.joint_Map["toe.left"]->Set_rot (Set (v_0, ball_ang , 0.0f, 0.0f));
   
   }

   return b;
}


//
// Init :: (Humanoid_articulator t) -> Humanoid_articulator
int An::Humanoid_artic::Init (Sc::Obj* obj, const An::Joint_def::Map& joint_def_map)
{
   //
   Vec3f model_pos ;
   Sc::Abs_obj_pos (model_pos , obj); 
   //
   //h3dGetNodeTransform (scene_node, 
   //   &X(ob_pos), &Y(ob_pos), &Z(ob_pos), 
   //   &X(ob_rot), &Y(ob_rot), &Z(ob_rot), 
   //   &X(ob_scale), &Y(ob_scale), &Z(ob_scale)
   //   ); 

	size_t cnt = 0;

   
   Bn::AABBf aabb; 
   Sc::Get_AABB (aabb, obj); 

   Sc::Get_skeleton_arr (this->joint_arr, this->joint_ind, obj); 
   
   //int num_joints = this->joint_Map.size ();
   joint_state.resize (this->joint_arr.size ());

   //for (Sc::ObjMap::const_iterator it = this->joint_Map.begin(); it != this->joint_Map.end(); it++)
   for (int ijt = 0; ijt < this->joint_arr.size (); ijt++)
   {
      const std::string& name = Sc::Joint_name (this->joint_arr[ijt]); 

      An::Joint_def::Map::const_iterator find_it = joint_def_map.find (name);

      if (find_it == joint_def_map.end ())
      {
         An::Joint_def::Map::const_iterator undef_it = joint_def_map.find ("undef"); 
         An::Joint_state::Initialize (joint_state[ijt], undef_it->second, 0.0f, 0.0f, 0.0f); 
      }
      else
      {
         An::Joint_state::Initialize (
            joint_state[ijt], //this->joint_state[find_it->first], 
            find_it->second, 0.0f, 0.0f, 0.0f); 
      }
   }



   std::map<std::string, Vec3f> abs_bind_bone_pos_map; 

   //for (Sc::ObjMap::iterator it = this->joint_Map.begin(); it != this->joint_Map.end(); it++)
   for (int ijt = 0; ijt < this->joint_arr.size (); ijt++)
   {
      const std::string& name = Sc::Joint_name (this->joint_arr[ijt]); 
      Sc::Abs_obj_pos (abs_bind_bone_pos_map[name], &this->joint_arr[ijt]); 
   }

   // + 1, because we do not pass in the fixed origin, assumed to be at <0>
   // target same heel position to compute bind angles, 
   // Ik::Solve_2D (wat.biped.L.dyna.chain + 1, wat.biped.L.dyna.deltas, 2, wat.biped.L.dyna.chain[2]); 
   // Ik::Solve_2D (wat.biped.R.dyna.chain + 1, wat.biped.R.dyna.deltas, 2, wat.biped.R.dyna.chain[2]); 

   // -1, 0, 1, 2 => left, none, right, both
   this->ds.power_leg = PL_none;
   // it should not be moving yet
   Set (this->ds.prev_velo, 0.0f, 0.0f, 0.0f);
   // assuming model is facing -z
   Set (this->ds.dir, 0.0f, 0.0f, 0.0f);

   //PL_left = -1, 
   //PL_right, 
   //PL_none, 
   //PL_both,

   An::Init(this->legs, abs_bind_bone_pos_map, aabb); 

   // wat?
   Y(this->min_compr); 
   Y(this->max_compr); 

   return 0; 
}

int An::Humanoid_artic::Deinit ()
{
   Sc::Destroy_skeleton_arr (this->joint_arr); 
   return 0; 

} 

//
int An::Humanoid_artic::Update (
   Sc::Obj*             model_obj,
   Co::Collision_space* collsp, 
   Ma::Vec3f const&     curr_dx,
   Ma::Vec3f const&     desired_dx)
{

   Vec3f v_, dir_;
   Vec2f ball_L, heel_L, v_foot, targ_R; 


   Ma::Mat44f X; 
   Ma::Transpose (Sc::Get_abs_transform (X, model_obj)); 

   Assert (0, "work on this"); 

   // animate legs
   Ma::Vec3f 
      actual_dx, 
      v_from, 
      v_to;

   Co::Raycast_result rayres (v_from, v_to); 
   
   //
   // find terrain points
   Assert (0, "work on this"); 
   //Co::Raycast (rayres, collsp, v_from, v_to); 
   //Co::Raycast (rayres, collsp, v_from, v_to); 
   //Co::Raycast (rayres, collsp, v_from, v_to); 
   //Co::Raycast (rayres, collsp, v_from, v_to); 
   //Co::Raycast (rayres, collsp, v_from, v_to); 
   //Co::Raycast (rayres, collsp, v_from, v_to); 


   Assert (0, "work on this"); 
   // this needs terrain points to 
   // legs dont yet know how to offset legs relative to X 
   An::Update (
      this->legs, this->ds, actual_dx, 
      desired_dx, model_obj); 

   // something (model_obj , actual_dx ); 

   



   // An::Biped& An::Update (
   //       An::Biped&        biped, 
   //       Sc::Node          pri_node, 
   //       Ma::Vec3f&        actual_dx, 
   //       const Ma::Vec3f&  desired_dx)


   // c
   // cpp
   // python
   // lua
   // commonlisp
   // scheme
   // clojure
   // go
   // haskell
   // ML
   // Basic
   // java


   //Copy (v_from, wat_pos); 
   //Copy (v_to, wat_pos); 
   //Y(v_from) += 1000.0f;
   //Y(v_to) -= 100.0f;
   //
   //Co::Raycast_result res(v_from, v_to);
   //
   //Sub(dir_, v_to, v_from);
   //            
   //if (Co::Raycast (res, coll_space.get(), v_from, v_to))
   //{
   //   Vec3f cont_, path_pos;
   //   // cont_ is the contact point on terrain
   //   Scale (cont_, dir_, res.hitTime); 
   //   Incr (cont_, v_from);
   //   Y (cont_);
   //
   //   obj_Table["island"]->Get_pos (path_pos); 
   //   //obj_Table["rock_path"]->Get_pos (path_pos); 
   //
   //   // keep rising if wat is not above terrain
   //   float y_pos = Y(wat_pos) + Y(wat.min_compr);
   //   if (y_pos < Y(cont_))
   //   {
   //      Y(wat_pos) += 0.2f;
   //      obj_Table["watsmyname"]->Set_pos (wat_pos); 
   //   }
   //
   //}
   //   
   //float t_ = 0.5 * cosf (2.0f * _dticker + Pi) + 0.5f;
   ////float foot_ang_ = 0.5 * cosf (1.0f * _dticker ) + 0.5;       
   ////float t_ = 0.1 ; // * cosf (1.0f * _dticker + Pi) + 0.5f;   
   //float foot_ang_      = 0.5f * cosf (5.0f * _dticker ) + 0.5;
   //float foot_ang_degr_ = Rads2Degr (foot_ang_);                 
   //
   //t_          = 0.0f;
   //foot_ang_   = 0.0f;
   //
   //Sub (dir_, wat.fwd_pos_L, wat.rear_pos_L);  
   //Scale (v_, dir_, t_);                       
   //Incr(v_, wat.rear_pos_L);                   
   //
   ////Y (v_)+= Y (wat_pos);
   //
   //Set (ball_L, Z(v_), Y(v_)); 
   //
   //An::Calc_heel_pos (v_foot, wat.legs.L.bind, foot_ang_); 
   //
   //Add (heel_L, ball_L, v_foot); 
   //
   //{  // IK solved in yz-space 
   //   // theta negative because coords applied in xy-space
   //   Ik::Solve_2D (wat.legs.L.dyna.chain + 1, wat.legs.L.dyna.deltas, 2, heel_L); 
   //   
   //   float hip_angl = Rads2Degr (wat.legs.L.dyna.deltas[0]); 
   //   wat.joint_Map["leg.upper.left"]->Set_rot (Set (v_0, -hip_angl, 0.0f, 0.0f)); 
   //
   //   float knee_angl = Rads2Degr (wat.legs.L.dyna.deltas[1]);  
   //   wat.joint_Map["leg.lower.left"]->Set_rot (Set (v_0, -knee_angl, 0.0f, 0.0f));
   //
   //   //float foot_ang= // Rads2Degr (wat.biped.L.dyna.deltas[1]);  
   //   
   //   float ankle_ang = hip_angl + knee_angl + Rads2Degr (foot_ang_);
   //   wat.joint_Map["foot.left"]->Set_rot (Set (v_0, ankle_ang , 0.0f, 0.0f));
   //
   //   float ball_ang = - Rads2Degr (foot_ang_);
   //      //float knee_angl = Rads2Degr (wat.biped.L.dyna.deltas[1]);  
   //   wat.joint_Map["toe.left"]->Set_rot (Set (v_0, ball_ang , 0.0f, 0.0f));
   //
   //}



      //{  // watsmyname 
      //   Vec3f wat_pos, wat_rot;
      //
      //   obj_Table["watsmyname"]->Get_pos (wat_pos); 
      //   obj_Table["watsmyname"]->Get_rot (wat_rot); 
      //
      //   // Y (v_0) += 0.2f; 
      //   // obj_Table["watsmyname"]->Set_rot (v_0); 
      //
      //   float _rotza  = 40.0f * cos (3.0f * _dticker) ; 
      //   float _rotzb = 40.0f * sin (3.0f * _dticker + Pi) ; 
      //   
      //   wat.joint_Map["neck"]->Set_rot (Set (v_0, 0.01f * _rotza, 0.0f, 0.0f)); 
      //   wat.joint_Map["head"]->Set_rot (Set (v_0, 0.2f * _rotza, 0.0f, 0.0f)); 
      //   // arms
      //   // fwd_pos_R, rear_pos_R,
      //   // fwd_pos_L, rear_pos_L;
      //   
      //   {  
      //      Vec3f v_, dir_;
      //      Vec2f ball_L, heel_L, v_foot, targ_R; 
      //
      //
      //      Vec3f v_from, v_to;
      //
      //      Copy (v_from, wat_pos); 
      //      Copy (v_to, wat_pos); 
      //      Y(v_from) += 1000.0f;
      //      Y(v_to) -= 100.0f;
      //
      //      Co::Raycast_result res(v_from, v_to);
      //
      //      Sub(dir_, v_to, v_from);
      //                  
      //      if (Co::Raycast (res, coll_space.get(), v_from, v_to))
      //      {
      //         Vec3f cont_, path_pos;
      //         // cont_ is the contact point on terrain
      //         Scale (cont_, dir_, res.hitTime); 
      //         Incr (cont_, v_from);
      //         Y (cont_);
      //
      //         obj_Table["island"]->Get_pos (path_pos); 
      //         //obj_Table["rock_path"]->Get_pos (path_pos); 
      //
      //         // keep rising if wat is not above terrain
      //         float y_pos = Y(wat_pos) + Y(wat.min_compr);
      //         if (y_pos < Y(cont_))
      //         {
      //            Y(wat_pos) += 0.2f;
      //            obj_Table["watsmyname"]->Set_pos (wat_pos); 
      //         }
      //
      //      }
      //         
      //      float t_ = 0.5 * cosf (2.0f * _dticker + Pi) + 0.5f;
      //      //float foot_ang_ = 0.5 * cosf (1.0f * _dticker ) + 0.5;       
      //      //float t_ = 0.1 ; // * cosf (1.0f * _dticker + Pi) + 0.5f;   
      //      float foot_ang_      = 0.5f * cosf (5.0f * _dticker ) + 0.5;
      //      float foot_ang_degr_ = Rads2Degr (foot_ang_);                 
      //
      //      t_          = 0.0f;
      //      foot_ang_   = 0.0f;
      //
      //      Sub (dir_, wat.fwd_pos_L, wat.rear_pos_L);  
      //      Scale (v_, dir_, t_);                       
      //      Incr(v_, wat.rear_pos_L);                   
      //
      //      //Y (v_)+= Y (wat_pos);
      //
      //      Set (ball_L, Z(v_), Y(v_)); 
      //
      //      An::Calc_heel_pos (v_foot, wat.legs.L.bind, foot_ang_); 
      //      
      //      Add (heel_L, ball_L, v_foot);     
      //
      //      {  // IK solved in yz-space 
      //         // theta negative because coords applied in xy-space
      //         Ik::Solve_2D (wat.legs.L.dyna.chain + 1, wat.legs.L.dyna.deltas, 2, heel_L); 
      //         
      //         float hip_angl = Rads2Degr (wat.legs.L.dyna.deltas[0]); 
      //         wat.joint_Map["leg.upper.left"]->Set_rot (Set (v_0, -hip_angl, 0.0f, 0.0f)); 
      //
      //         float knee_angl = Rads2Degr (wat.legs.L.dyna.deltas[1]);  
      //         wat.joint_Map["leg.lower.left"]->Set_rot (Set (v_0, -knee_angl, 0.0f, 0.0f));
      //
      //         //float foot_ang= // Rads2Degr (wat.biped.L.dyna.deltas[1]);  
      //         
      //         float ankle_ang = hip_angl + knee_angl + Rads2Degr (foot_ang_);
      //         wat.joint_Map["foot.left"]->Set_rot (Set (v_0, ankle_ang , 0.0f, 0.0f));
      //
      //         float ball_ang = - Rads2Degr (foot_ang_);
      //            //float knee_angl = Rads2Degr (wat.biped.L.dyna.deltas[1]);  
      //         wat.joint_Map["toe.left"]->Set_rot (Set (v_0, ball_ang , 0.0f, 0.0f));
      //      }
      //   }
      //
      //}      


   return 0;
}

/****************************************************************
 * Humanoid LocoMotion     
 ***************************************************************/	
enum LocoMotionStateEnum {
	LocoMotionStand = 0, 
	LocoMotionWalk, 
	LocoMotionRun, 
	//
	LocoMotionMaxEnum_
	};

enum BipedActionState {
	BipedStandIdle, 
	BipedFwdPower, 
	BipedFwdContact, 
	BipedFwdRecoil, 
	BipedFwdRelease, 
	//
	BipedActionMaxEnum_
};

