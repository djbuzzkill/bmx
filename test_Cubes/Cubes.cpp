
#include "test_Deferred.h"
#include "Dx/VecMath.h"

const int kInitial_window_width  = 1024;
const int kInitial_window_height = 768;

static double  kNear_plane_dist    = 200.0;
static double  kFar_plane_dist     = 30000.0;
static double  kPerspective_FoV    = 60.0; 
const float    kMaskVal             = -2553.0f;

void Make_Mars_tiles       ();
void Make_Mars_normal_map  (float height_scale); 

template<typename Ty> 
struct sh_arr : public boost::shared_array<Ty> {
   sh_arr (Ty* p) : boost::shared_array<Ty> (p) { 
   }
   sh_arr () : boost::shared_array<Ty> () { 
   }

}; 

void wat ()
{
   int i = 0; i++; 
}

   struct EN_coord 
      { 
   double e; double n; 
      }; 

   template<typename Ty> struct minmax 
      { 
   Ty mn, mx; 
      };


   EN_coord& EN_incr (EN_coord& a, const EN_coord& b)
      {
   a.e += b.e;
   a.n += b.n;
   return a; 
      }

   EN_coord EN_add (const EN_coord& a, const EN_coord& b)
      {
   EN_coord coord = { a.e + b.e, a.n + b.n }; 
   return coord; 
      } 

   EN_coord EN_sub (const EN_coord& a, const EN_coord& b)
      {
   EN_coord EN = { a.e - b.e, a.n - b.n }; 
   return EN; 
      } 

   double EN_dot (const EN_coord& a, const EN_coord& b)
      {
   return a.e * b.e + a.n * b.n; 
      }

   double EN_dist (const EN_coord& a, const EN_coord& b)
      {
   EN_coord v = EN_sub (b, a);
   return sqrt (EN_dot (v, v)); 
      }

   EN_coord EN_dir (const EN_coord& a)
      {
   EN_coord v0 = {0.0, 0.0}; 
   double l = 1.0 / EN_dist (a, v0); 
   EN_coord v = { a.e*l, a.n*l }; 
   return v; 
      }


//
//
union ptru 
   {
void* v;  float* f;  double* d;  unsigned char* uc;  char* c;  unsigned short* us;  short* s; EN_coord* en;  
   }; 



 

//
//
struct Simple_obj : public Renderable 
   {

   Simple_obj () { 
      }

   virtual glm::dvec3&  Get_pos  () { return pos; } 
   virtual glm::dvec3&  Get_rot  () { return rot; } 
   virtual GLuint       Bin_ID() { return 0; } 
   virtual GLuint       ROp_ID() { return 0; } 

   virtual void         Setup_RS (const TerrainRenderer::UniformMap& uniformMap, const TerrainRenderer::AttributeMap& attribMap)
   {}

   glm::dvec3                 pos; 
   glm::dvec3                 rot; 
   glm::dvec3                 scl; 

   GLuint                     txrID; 
   }; 

//
//
struct Light_obj : public Renderable 
   {

   enum Type {
      _INVALID_LT_ = -1,
      LT_Directional, 
      LT_Point, 
      LT_Spot, 
      }; 

   Light_obj () { 
      }

   virtual glm::dvec3&  Get_pos  () { return pos; } 
   virtual glm::dvec3&  Get_rot  () { return rot; } 
   virtual GLuint       Bin_ID() { return 0; } 
   virtual GLuint       ROp_ID() { return 0; } 

   virtual void         Setup_RS (const TerrainRenderer::UniformMap& uniformMap, const TerrainRenderer::AttributeMap& attribMap)
   {}
      
   Type                       type; 
   glm::dvec3                 pos; 
   glm::dvec3                 rot; 
   glm::dvec3                 dir; 
   double                     radius; 

   std::vector<glm::fvec3>    geom; 

   GLuint                     col_ID; 
   GLuint                     hgt_ID; 
   }; 

// 
//
// FUNC Update_view_transform 
void Update_view_transform (
	glm::dvec3&				view_Pos, 
	glm::dvec3&				view_Rot, 
	double 					   move_Rate,
	const sy::Keyboard_state& kb,    
	const sy::Mouse_state&    ms)
{

   const double kDeg2Pi = Ma::Pi / 180.0f; 
   const double fHalfPi = Ma::HalfPi;


   static float dYdx = 0.75f; 
   static float dXdy = 0.75f; 
   
   Ma::Vec3d dir_Fwd, dir_Right, dir_Up; 

   Ma::Set (dir_Up, 0.0, 1.0, 0.0); 

   {
      if (ms.yRel || ms.yRel)
      {
         wat (); 
      }

      view_Rot[0] -= kDeg2Pi * (ms.yRel * dYdx);
      view_Rot[1] -= kDeg2Pi * (ms.xRel * dXdy);
      view_Rot.z = 0.0; 

      Ma::Vec3f v_t;
      // (\ spherical.theta(0).phi(0)) => <1, 0, 0>
      Ma::Spherical (v_t, 0.0f, 0.0f);
      Ma::Spherical (dir_Fwd,    kDeg2Pi * view_Rot[1] - Ma::HalfPi, 0.0);
      Ma::Spherical (dir_Right,  kDeg2Pi * view_Rot[1], 0.0); 
      Ma::X (dir_Right);
   }
   
   
   
   // view movement
   {
      const glm::dvec3 kX_axis (1.0, 0.0, 0.0); 
      const glm::dvec3 kY_axis (0.0, 1.0, 0.0); 
      const glm::dvec3 kZ_axis (0.0, 0.0, 1.0); 


      glm::dvec3 dirForward  = glm::rotateY (kZ_axis, view_Rot[1] ); 
      glm::dvec3 dirRight    = glm::rotateY (kX_axis, view_Rot[1] ); 
          
      Ma::Vec3d v  ;

      //
      if (sy::Is_keydown (sy::SC_F, kb)) 
         {
         view_Pos -= move_Rate * dirForward; 
   	   }
      else 
      if (sy::Is_keydown (sy::SC_V, kb)) 
         {
         view_Pos += move_Rate * dirForward; 
   	   }
   
      //
      if (sy::Is_keydown (sy::SC_D, kb)) {
         view_Pos -= move_Rate * dirRight; 
   	   }
      else 
      if (sy::Is_keydown (sy::SC_G, kb)) {
         view_Pos += move_Rate * dirRight; 
   	   }

      //
      if (sy::Is_keydown (sy::SC_A, kb)) {
         view_Pos.y += move_Rate; 
   	   }
      else
      if (sy::Is_keydown (sy::SC_Z, kb)) {
         view_Pos.y -= move_Rate; 
   	   }


   }   

}

static glm::fvec3 cube_norm[] = {
   // front
   glm::fvec3 (0.0, 0.0, 1.0), 
   glm::fvec3 (0.0, 0.0, 1.0), 
   glm::fvec3 (0.0, 0.0, 1.0), 
   glm::fvec3 (0.0, 0.0, 1.0), 
   glm::fvec3 (0.0, 0.0, 1.0), 
   glm::fvec3 (0.0, 0.0, 1.0), 
      // right
   glm::fvec3 (1.0, 0.0, 0.0), 
   glm::fvec3 (1.0, 0.0, 0.0), 
   glm::fvec3 (1.0, 0.0, 0.0), 
   glm::fvec3 (1.0, 0.0, 0.0), 
   glm::fvec3 (1.0, 0.0, 0.0), 
   glm::fvec3 (1.0, 0.0, 0.0), 
      // left
   glm::fvec3 (-1.0, 0.0, 0.0), 
   glm::fvec3 (-1.0, 0.0, 0.0), 
   glm::fvec3 (-1.0, 0.0, 0.0), 
   glm::fvec3 (-1.0, 0.0, 0.0), 
   glm::fvec3 (-1.0, 0.0, 0.0), 
   glm::fvec3 (-1.0, 0.0, 0.0), 
      // bottom
   glm::fvec3 (0.0, -1.0, 0.0), 
   glm::fvec3 (0.0, -1.0, 0.0), 
   glm::fvec3 (0.0, -1.0, 0.0), 
   glm::fvec3 (0.0, -1.0, 0.0), 
   glm::fvec3 (0.0, -1.0, 0.0), 
   glm::fvec3 (0.0, -1.0, 0.0), 
      // top
   glm::fvec3 (0.0, 1.0, 0.0), 
   glm::fvec3 (0.0, 1.0, 0.0), 
   glm::fvec3 (0.0, 1.0, 0.0), 
   glm::fvec3 (0.0, 1.0, 0.0), 
   glm::fvec3 (0.0, 1.0, 0.0), 
   glm::fvec3 (0.0, 1.0, 0.0), 
      // back
   glm::fvec3 (0.0, 0.0, -1.0), 
   glm::fvec3 (0.0, 0.0, -1.0), 
   glm::fvec3 (0.0, 0.0, -1.0), 
   glm::fvec3 (0.0, 0.0, -1.0), 
   glm::fvec3 (0.0, 0.0, -1.0), 
   glm::fvec3 (0.0, 0.0, -1.0), 
   };

static glm::fvec3 cube_geom[] = {
   // front
   glm::fvec3 (1.0, 1.0, 1.0), 
   glm::fvec3 (-1.0, 1.0, 1.0), 
   glm::fvec3 (-1.0, -1.0, 1.0), 
   glm::fvec3 (-1.0, -1.0, 1.0), 
   glm::fvec3 (1.0, -1.0, 1.0), 
   glm::fvec3 (1.0, 1.0, 1.0), 
   // right
   glm::fvec3 (1.0, 1.0, -1.0), 
   glm::fvec3 (1.0, 1.0, 1.0), 
   glm::fvec3 (1.0, -1.0, 1.0), 
   glm::fvec3 (1.0, -1.0, 1.0), 
   glm::fvec3 (1.0, -1.0, -1.0), 
   glm::fvec3 (1.0, 1.0, -1.0), 
   // left
   glm::fvec3 (-1.0, 1.0, 1.0), 
   glm::fvec3 (-1.0, 1.0, -1.0), 
   glm::fvec3 (-1.0, -1.0, -1.0), 
   glm::fvec3 (-1.0, -1.0, -1.0), 
   glm::fvec3 (-1.0, -1.0, 1.0), 
   glm::fvec3 (-1.0, 1.0, 1.0), 
   // bottom
   glm::fvec3 (1.0, -1.0, 1.0), 
   glm::fvec3 (-1.0, -1.0, 1.0), 
   glm::fvec3 (-1.0, -1.0, -1.0), 
   glm::fvec3 (-1.0, -1.0, -1.0), 
   glm::fvec3 (1.0, -1.0, -1.0), 
   glm::fvec3 (1.0, -1.0, 1.0), 
   // top
   glm::fvec3 (1.0, 1.0, -1.0), 
   glm::fvec3 (-1.0, 1.0, -1.0), 
   glm::fvec3 (-1.0, 1.0, 1.0), 
   glm::fvec3 (-1.0, 1.0, 1.0), 
   glm::fvec3 (1.0, 1.0, 1.0), 
   glm::fvec3 (1.0, 1.0, -1.0), 
   // back
   glm::fvec3 (-1.0, 1.0, -1.0), 
   glm::fvec3 (1.0, 1.0, -1.0), 
   glm::fvec3 (1.0, -1.0, -1.0), 
   glm::fvec3 (1.0, -1.0, -1.0), 
   glm::fvec3 (-1.0, -1.0, -1.0), 
   glm::fvec3 (-1.0, 1.0, -1.0), 
}; 


static glm::fvec2 cube_txc0[] = {
   glm::fvec2 (1.0, 1.0), 
   glm::fvec2 (0.0, 1.0), 
   glm::fvec2 (0.0, 0.0), 
   glm::fvec2 (0.0, 0.0), 
   glm::fvec2 (1.0, 0.0), 
   glm::fvec2 (1.0, 1.0), 

   glm::fvec2 (1.0, 1.0), 
   glm::fvec2 (0.0, 1.0), 
   glm::fvec2 (0.0, 0.0), 
   glm::fvec2 (0.0, 0.0), 
   glm::fvec2 (1.0, 0.0), 
   glm::fvec2 (1.0, 1.0), 

   glm::fvec2 (1.0, 1.0), 
   glm::fvec2 (0.0, 1.0), 
   glm::fvec2 (0.0, 0.0), 
   glm::fvec2 (0.0, 0.0), 
   glm::fvec2 (1.0, 0.0), 
   glm::fvec2 (1.0, 1.0), 

   glm::fvec2 (1.0, 1.0), 
   glm::fvec2 (0.0, 1.0), 
   glm::fvec2 (0.0, 0.0), 
   glm::fvec2 (0.0, 0.0), 
   glm::fvec2 (1.0, 0.0), 
   glm::fvec2 (1.0, 1.0), 

   glm::fvec2 (1.0, 1.0), 
   glm::fvec2 (0.0, 1.0), 
   glm::fvec2 (0.0, 0.0), 
   glm::fvec2 (0.0, 0.0), 
   glm::fvec2 (1.0, 0.0), 
   glm::fvec2 (1.0, 1.0), 

   glm::fvec2 (1.0, 1.0), 
   glm::fvec2 (0.0, 1.0), 
   glm::fvec2 (0.0, 0.0), 
   glm::fvec2 (0.0, 0.0), 
   glm::fvec2 (1.0, 0.0), 
   glm::fvec2 (1.0, 1.0), 
}; 

//

//
struct MRT_frame_buffer                  
{

   enum Type {
      RT_Color = 0, 
      RT_Depth, 
      RT_Norm, 
      Num_RT_types
   }; 

   GLuint         ID;
   int            width; 
   int            height; 
   GLuint         rt_IDs[Num_RT_types]; 
   // color, depth, normal
};

//
void MRT_Initialize (MRT_frame_buffer& mrt, int wd, int ht)
{
   glGenFramebuffers (1, &mrt.ID); 

   glBindFramebuffer (GL_FRAMEBUFFER, mrt.ID); 

   glGenTextures (MRT_frame_buffer::Num_RT_types, mrt.rt_IDs); 


   for (int i = 0; i < MRT_frame_buffer::Num_RT_types; i++) 
   {
      glBindTexture (GL_TEXTURE_3D, mrt.rt_IDs[i]); 
      switch (i)
      {
      case MRT_frame_buffer::RT_Color:
         glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB32F, mrt.width, mrt.height, 0, GL_RGB32F, GL_RGB32F, 0);  
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
         glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mrt.rt_IDs[i], 0); 
         break; 

      case MRT_frame_buffer::RT_Depth:
         glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB32F, mrt.width, mrt.height, 0, GL_RGB32F, GL_RGB32F, 0);  
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
         glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mrt.rt_IDs[i], 0); 
         break; 
      case MRT_frame_buffer::RT_Norm:
         glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB32F, mrt.width, mrt.height, 0, GL_RGB32F, GL_RGB32F, 0);  
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
         glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mrt.rt_IDs[i], 0); 
         break; 
      }



   }

   // reset to default
   glBindFramebuffer (GL_FRAMEBUFFER, 0); 

}

void MRT_Deinitialize (MRT_frame_buffer& mrt)
{
}

//
void MRT_BindForRendering (MRT_frame_buffer& mrt)
{

}


//
// CLASS Defer_test 
class Defer_test : public sy::RT_Task, public sy::Window_listener, public cx::Destructor
{
private: 

   void initialize_graphics_objects (); 
   void initialize_scene_objects    (); 
   
   


   // 
   std::shared_ptr<sy::Graphics_window>   windo; 
   bool                                   init_; 
   glm::ivec2                             view_dim; 
   

   MRT_frame_buffer              framebuffer;
   View_params                   view_Params;

   struct DeferDat {
      std::vector<Light_obj>     lights; 
      std::vector<Simple_obj>    objs; 
      std::vector<Renderable*>   rend_lit; 
      std::vector<Renderable*>   rend_obj; 
      } dat; 


   void init_graphics   (sy::System_context* sys);
   void update_input    (sy::System_context* sys); 
   void render          (sy::System_context* sys);

public: 

   Defer_test (sy::System_context* sys);
   virtual ~Defer_test ();

   // app loop 
	virtual int Initialize	      (sy::System_context*);  
	virtual int Deinitialize      (sy::System_context*);  
	virtual int Update	         (sy::System_context*);  
   
   // window listener
	virtual void OnWindowResize	(int wd, int ht);   
	virtual void OnWindowClose		();
	virtual void OnWindowActivate	(bool activate);

   static const std::string kOut_path   ; // = "J:/Quarantine/Mars"; 
   static const std::string kBase_fname ; // = "/ESP_018065_1975_RED_ESP_019133_1975_RED"; 

};

//
Defer_test::Defer_test (sy::System_context* sys)
   : windo     ()
   , init_     (false)
   , view_dim  (kInitial_window_width, kInitial_window_height)
   , dat      () 
{  

}

//
Defer_test::~Defer_test ()
{
   Deinitialize (0); 
}



void Defer_test::init_graphics (sy::System_context* sys)
{
   glewInit (); 

   ilInit (); 

}

void Defer_test::initialize_graphics_objects () 
{
   
}

void Defer_test::initialize_scene_objects () 
{



   MRT_Initialize (framebuffer, kInitial_window_width, kInitial_window_height); 



}


//
int Defer_test::Initialize (sy::System_context* sys)
{
   // Make_Mars_tiles (); 

   windo.reset (sys->Create_GraphicsWindow (this, "hello Mars", kInitial_window_width, kInitial_window_height, false)); 

   init_graphics (sys); 

   
   view_Params.pos[0] = 0.0; 
   view_Params.pos[1] = 9.0;
   view_Params.pos[2] = 0.0; 

   view_Params.rot[0] = glm::pi<double>(); 
   view_Params.rot[1] = glm::pi<double>(); 

   //
   initialize_graphics_objects (); 

   initialize_scene_objects (); 

   //





   return 0;
}
  
//
int Defer_test::Deinitialize (sy::System_context*)
{

   // std::vector<GLuint> v ; 
   // v.reserve (num_tx_ids);
   // glDeleteTextures (num_tx_ids, v.data()); 

   return 0;
}
  
//
int Defer_test::Update (sy::System_context* sys)
{
   //   
   update_input (sys); 
   
   // 
   render (sys); 

   return 0;
}
  
void Defer_test::update_input (sy::System_context* sys)
{
   sy::Keyboard_state   kb; 
   sy::Mouse_state      ms;
   sys->Poll_input (ms, kb); 

//   if (sy::Is_keydown (sy::SC_LEFTBRACKET, kb))
//      tessFactor -= 0.321f;
//
//   if (sy::Is_keydown (sy::SC_RIGHTBRACKET, kb))
//      tessFactor += 0.321f;
//    // clamp
//   tessFactor = glm::clamp (tessFactor , 1.0f, 200.0f); 


   //
   //
   if (sy::Is_keydown (sy::SC_Q, kb) || sy::Is_keydown (sy::SC_ESCAPE, kb)) 
   {
      sys->SysReq (sy::SR_Exit); 
   }
   else
   {
//      if (sy::Is_keydown (sy::SC_H, kb))
//         mars.patchScale.y += 0.2; 
//
//      if (sy::Is_keydown (sy::SC_N, kb))
//         mars.patchScale.y -= 0.2; 
//      bg_renderer->Patch_scale (mars.patchScale.x, mars.patchScale.y, mars.patchScale.z); 

      double camera_y_asp = double (view_dim[0]) / double(view_dim[1]); 
      view_Params.FoV = Ma::Pi * kPerspective_FoV / 180.0;
      view_Params.Asp_ratio = camera_y_asp ; 
      view_Params.dist_Near = kNear_plane_dist; 
      view_Params.dist_Far =  kFar_plane_dist; 
      static double Dt = 20.0;
      // -> is a quat mars.view_Rot, not euler
      Update_view_transform (view_Params.pos, view_Params.rot, Dt , kb, ms); 
   } 
     
}


//
// 
void Defer_test::render (sy::System_context* sys)
{   
   //
   glViewport (0, 0, view_dim[0] , view_dim[1]); 

   //
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 



   glClearColor (0.7f, 0.8f, 0.9f, 1.0f); 

   //


   // draw stuff 

   //
   windo->Swap_buffers (); 
}



//
// window listener
void Defer_test::OnWindowResize (int wd, int ht) 
{
   view_dim[0] = wd;
   view_dim[1] = ht;
}
   
//
void Defer_test::OnWindowClose ()  
{
   int wat = 0; wat++; 
}

//
void Defer_test::OnWindowActivate	(bool activate)
{
   int wat = 0; wat++; 
}

//
//
Defer_test* Create_Defer_test (sy::System_context* sys)
{
   return new Defer_test (sys); 
}





//
// main entry
int main (int argv, char** argc)
   {
std::shared_ptr<sy::System_context>  sys   (sy::Create_context ()); 
std::shared_ptr<Defer_test>          test  (Create_Defer_test(sys.get())); 
return sy::Run_realtime_task (sys.get(), test.get()); 
   }


