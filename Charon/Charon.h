#ifndef CHARON_INCLUDE
#define CHARON_INCLUDE


// C RunTime Header Files
#include <cstdlib>
#include <memory>
#include <numeric>
#include <utility>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <list>
#include <map>
#include <tuple>

#include "GL/glew.h"
#include <glm/glm.hpp>  
#include <glm/ext.hpp>

#include "FreeImage.h"

#include <Dx/System.h>
#include <Dx/Utility.h>
#include <Dx/VecMath.h>
#include <Dx/Render.h>
#include <Dx/Resource.h>


#include <boost/lexical_cast.hpp>
#include <boost/shared_array.hpp>
#include <boost/assert.hpp>

#include "process_mars_terrain_for_runtime.h"
#include "quantize_height_tiles_to_u16.h"



typedef char                  u8;
typedef unsigned char         s8;
typedef short                 s16;
typedef unsigned short        u16;
typedef int                   s32;
typedef unsigned int          u32;
typedef float                 f32;
typedef double                f64;
typedef long long             s64;
typedef unsigned long long    u64;


#define MARS_DRIVE "E:/" 
#define SHADER_DRIVE "C:/"

void Validate_GL_call();
void wat();

template<typename Ty> 
   Ty* As(void* v) {
   return static_cast<Ty*>(v);
   }

template<typename Ty> 
struct sh_arr : public boost::shared_array<Ty> {
   sh_arr (Ty* p) : boost::shared_array<Ty> (p) { 
   }
   sh_arr () : boost::shared_array<Ty> () { 
   }
}; 

struct Renderable
{
   virtual glm::fvec3&  Pos() = 0;
   virtual glm::fvec3&  Rot() = 0;
   virtual glm::fvec3&  Scl() = 0;

   virtual GLuint       Bin_ID() = 0;
   virtual GLuint       ROp_ID() = 0;

   virtual void Setup_RS (
      const Rn::UniformMap&      uniformMap, 
      const Rn::UniformValueMap& unifvalue,
      const Rn::AttributeMap&    attribMap) = 0;

protected:
   Renderable() { }
};

//
// FUNC Update_view_transform 
void Update_view_transform (
   glm::fvec3&				      view_Pos,
   glm::fvec3&				      view_Rot,
   float                      move_Rate,
   const sy::Keyboard_state&  kb,
   const sy::Mouse_state&     ms
   );

GLenum   GL_TEXTURE_stage(int stage);

//
//
namespace mars_terr
{
   extern const char                kBase_path[]; 
   extern const char                kTile_subp[]; 
   extern const char                kShader_subp[]; 
   extern const char                kShader_name[];
   extern const char*               kShader_ext[];
   extern const GLuint              kShader_types[];
   extern const char*               kFiles[];
   extern const std::string         kType[];
   extern const FREE_IMAGE_FORMAT   kFIF_fmt[];
   extern const size_t              kNum_files; 
   extern const int                 kTexture_dim; 
   extern const int                 kNum_X_tiles;  
   extern const int                 kNum_Y_tiles;  
   extern const unsigned            kWd; 
   extern const unsigned            kHt; 
}


std::string index_2_str (int X, int Y);
bool        str_2_index (int &X, int& Y, const std::string& s);

void generate_height_tiles (
   std::string fname,
   std::string basename,
   int         in_width,
   int         in_height,
   std::string outpath,
   int         numxtiles,
   int         numytiles,
   double      scale
   );

void generate_color_tiles (
   std::string fname,
   std::string basename,
   int         in_width,
   int         in_height,
   std::string outpath,
   int         numxtiles,
   int         numytiles
   );

void generate_normal_tiles (
   std::string fname          ,
   std::string basename       ,
   int         in_width,
   int         in_height,
   std::string outpath,
   int         numxtiles,
   int         numytiles
   );


//
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
}


union ptru {
   void*             v; 
   char*             c; 
   unsigned char*    uc; 
   short*            s; 
   unsigned short*   us; 
   int*              i;
   unsigned int*     ui;  
   float*            f; 
   double*           d;
   }; 

//
// signed/unsigned 
template<typename Ty>
inline Ty str_to(const std::string& str) {
   return boost::lexical_cast<Ty>(str);
   }

template<typename Ty>
inline void DontCare(Ty& x) {
   (void) x;
   }


template<typename ATy_, typename BTy_>
struct Tuple {
   BTy_ a;
   ATy_ b; 
   };

typedef std::map<u32, std::string>                 IndexedMap;
typedef std::map<u32, std::string>                 PropMap;
typedef std::list<Tuple<std::string, std::string>> PropList;


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


   // 
   // a function to compute pos + rot
   void Update_view_transform (
	   Ma::Vec3f&				view_Pos, 
	   Ma::Vec3f&				view_Rot, 
	   float					   move_Rate,
	   const sy::Keyboard_state& kb,    
	   const sy::Mouse_state&    ms
      );




}

#endif