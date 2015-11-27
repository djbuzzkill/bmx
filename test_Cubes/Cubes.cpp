
#include "Cubes.h"



const int kInitial_window_width     = 1024;
const int kInitial_window_height    = 768;

static double  kNear_plane_dist     = 100.0;
static double  kFar_plane_dist      = 3000.0;
static double  kPerspective_FoV     = 60.0; 
const float    kMaskVal             = -2553.0f;

void Make_Mars_tiles       ();
void Make_Mars_normal_map  (float height_scale); 

//
//
struct Simple_obj : public Renderable 
   {

   Simple_obj () { 
      }

   virtual glm::fvec3&  Pos  () { return pos; } 
   virtual glm::fvec3&  Rot  () { return rot; } 
   virtual glm::fvec3&  Scl  () { return scl; }

   virtual GLuint       Bin_ID   () { return 0; } 
   virtual GLuint       ROp_ID   () { return 0; } 

   virtual void         Setup_RS(const Rn::UniformMap& uniformMap, const Rn::AttributeMap& attribMap); 

   glm::fvec3           pos; 
   glm::fvec3           rot; 
   glm::fvec3           scl; 
   GLuint               txrID; 
   }; 

//
void Simple_obj::Setup_RS (
   const Rn::UniformMap& uniformMap, 
   const Rn::AttributeMap& attribMap)
{
   int   texture_stage = 0;

   BOOST_ASSERT(uniformMap.count("colorMap"));

   Rn::UniformMap::const_iterator col_It = uniformMap.find("colorMap");


   //
   // setup color texture
   glActiveTexture(GL_TEXTURE_stage(texture_stage));
   glBindTexture(GL_TEXTURE_2D, txrID);
   Validate_GL_call();

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   Validate_GL_call();

   glUniform1i(col_It->second, texture_stage);
   Validate_GL_call();
   texture_stage++;

   glEnable(GL_TEXTURE_2D);
   Validate_GL_call();

}

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

   virtual glm::fvec3&  Pos   () { return pos; } 
   virtual glm::fvec3&  Rot   () { return rot; }
   virtual glm::fvec3&  Scl   () { return scl; }
   virtual GLuint       Bin_ID() { return 0; }
   virtual GLuint       ROp_ID() { return 0; } 

   virtual void         Setup_RS(const Rn::UniformMap& uniformMap, const Rn::AttributeMap& attribMap)
   {}
      
   Type                       type; 
   glm::fvec3                 pos; 
   glm::fvec3                 rot;
   glm::fvec3                 scl;
   glm::fvec3                 dir;
   double                     radius; 

   std::vector<glm::fvec3>    geom; 

   GLuint                     col_ID; 
   GLuint                     hgt_ID; 
   }; 





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
      RT_Position,
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

   mrt.width   = wd;
   mrt.height  = ht; 
   glGenFramebuffers (1, &mrt.ID); 

   glBindFramebuffer (GL_FRAMEBUFFER, mrt.ID); 

   glGenTextures (MRT_frame_buffer::Num_RT_types, mrt.rt_IDs); 


   for (int i = 0; i < MRT_frame_buffer::Num_RT_types; i++)
   {
      glBindTexture (GL_TEXTURE_2D, mrt.rt_IDs[i]); 
      switch (i)
      {
      case MRT_frame_buffer::RT_Color:
         glTexImage2D   (GL_TEXTURE_2D, 0, GL_RGB, mrt.width, mrt.height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
         glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mrt.rt_IDs[i], 0); 
         break; 

      case MRT_frame_buffer::RT_Position:
         glTexImage2D   (GL_TEXTURE_2D, 0, GL_RGB32F, mrt.width, mrt.height, 0, GL_RGB, GL_FLOAT, 0);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
         glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mrt.rt_IDs[i], 0); 
         break; 
      case MRT_frame_buffer::RT_Norm:
         glTexImage2D   (GL_TEXTURE_2D, 0, GL_RGB16F, mrt.width, mrt.height, 0, GL_RGB, GL_FLOAT, 0);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
         glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mrt.rt_IDs[i], 0); 
         break; 
      }
      Validate_GL_call (); 
   }



   GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

   if (Status != GL_FRAMEBUFFER_COMPLETE) {
      printf("FB error, status: 0x%x\n", Status);
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

   void init_graphics_objects (); 
   void init_scene_objects    (); 
   void draw_simple           (); 
   // 
   Rn::ShaderTable    shader_Table;
   Rn::UniformMap     uniformLoc_map;
   Rn::AttributeMap   attribLoc_map;


   std::shared_ptr<sy::Graphics_window>   windo; 
   bool                                   init_; 
   glm::ivec2                             view_dim; 
   

   MRT_frame_buffer              framebuffer;
   View_params                   viewparams;

   struct DeferDat {
      std::vector<Light_obj>     lights; 
      std::vector<Simple_obj>    cubes; 
      std::vector<Renderable*>   rend_lit; 
      std::vector<Renderable*>   rend_obj; 
      } dat; 

   std::vector<Renderable*>   objs;

   void init_graphics   (sy::System_context* sys);
   void update_input    (sy::System_context* sys); 
   void render          (sy::System_context* sys);

public: 

   Defer_test (sy::System_context* sys);
   virtual ~Defer_test ();

   // task
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



UniformDef cube_uniforms[] = {
   { "mat_Model", UT_MAT4F, 1, 0 },
   { "mat_View", UT_MAT4F, 1, 0 },
   { "mat_Proj", UT_MAT4F, 1, 0 },
   { "colorMap", UT_SAMPLER, 1, 0 },
   };

AttributeDef cube_attribs[] = {
   { "vPosi", AT_VEC3F, 0 },
   { "vTxcd", AT_VEC2F, 0 },
   };

void Defer_test::init_graphics (sy::System_context* sys)
{
   glewInit (); 
}

void Defer_test::init_graphics_objects () 
{
   viewparams.pos[0] = 0.0;
   viewparams.pos[1] = 9.0;
   viewparams.pos[2] = 0.0;

   viewparams.rot[0] = glm::pi<double>();
   viewparams.rot[1] = glm::pi<double>();

   MRT_Initialize(framebuffer, kInitial_window_width, kInitial_window_height);

   {  // basic shader
      std::vector<char> vs, fs;
      ut::Load_text_file(vs, "C:/Quarantine/hardcore/shader/Cubes/basic_shader.vp");
      ut::Load_text_file(fs, "C:/Quarantine/hardcore/shader/Cubes/basic_shader.fp");
   
      shader_Table["basic_vp"] = Create_shader((GLchar*)vs.data(), GL_VERTEX_SHADER)   ;
      shader_Table["basic_fp"] = Create_shader((GLchar*)fs.data(), GL_FRAGMENT_SHADER) ;
      GLuint shaders[3] = {
         shader_Table["basic_vp"], 
         shader_Table["basic_fp"], 
         0
         };
   
      shader_Table["basic_prog"] = Build_shader_program(shaders);

      Uniform_locations  (uniformLoc_map, cube_uniforms, El_count(cube_uniforms), shader_Table["basic_prog"]);
      Attribute_locations(attribLoc_map, cube_attribs, El_count(cube_attribs), shader_Table["basic_prog"]);
   }



   wat ();

   

}

void Defer_test::init_scene_objects () 
{
   const std::string rgbroot = "C:/Quarantine/awsum/Cubes/rgb/";
      
   int wd = 512;
   int ht = 512;
   int numpixels = wd * ht;

   char* files[]= {
      "tex_6",
      "tex_3",
      "tex_5",
      "tex_1",
      "tex_0",
      "tex_7",
      "tex_4",
      "tex_2",
   };
   int numfiles = El_count (files); 

   int randshuffle = rand () % 20; 

   for (int i = 0; i < randshuffle; i++)
      std::next_permutation (files, files + numfiles); 

   std::vector<unsigned char> imgbuf (numpixels * 3);
   std::vector<GLuint> txIDs (numfiles); 
   glGenTextures(numfiles, txIDs.data()); 
   for(int i = 0; i < numfiles; i++) 
   {
      std::string currfile = rgbroot + files[i]; 
      std::shared_ptr<FILE> rs(fopen(currfile.c_str(), "rb"), fclose); 
      fread(imgbuf.data(), 1, numpixels * 3, rs.get()); 
      glBindTexture (GL_TEXTURE_2D, txIDs[i]); 
      
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wd, ht, 0, GL_RGB, GL_UNSIGNED_BYTE, imgbuf.data());
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      Validate_GL_call(); 
   }

   objs.clear();
   // setup objects
   dat.cubes.resize (numfiles); 
   float fRotMult= Ma::TwoPi * 0.01; 
   for (int i = 0; i < dat.cubes.size(); i++)
   {

      if (i) {
         dat.cubes[i].pos = glm::fvec3(float(rand() % 2000) - 1000.0f, 100.0f + float(rand() % 100), float(rand() % 2000) - 1000.0f);
         dat.cubes[i].scl = glm::fvec3(float(rand() % 100) + 20.0f, float(rand() % 100) + 20.0f, float(rand() % 100) + 20.0f);
         dat.cubes[i].rot = glm::fvec3(fRotMult * float(rand() % 100), fRotMult * float(rand() % 100), fRotMult * float(rand() % 100));

      }
      else {  // we want this one to be the floor
         dat.cubes[i].pos = glm::fvec3(0.0f, 0.0f, 0.0f);
         dat.cubes[i].scl = glm::fvec3(1000.0f, 1.0f, 1000.0f);
         dat.cubes[i].rot = glm::fvec3(0.0f, 0.0f, 0.0f);
      }

      dat.cubes[i].txrID = txIDs[i]; 


      objs.push_back(&dat.cubes[i]); 
   }

}


//void make_cube_textures();
//
int Defer_test::Initialize (sy::System_context* sys)
{
//   make_cube_textures (); 
   windo.reset (sys->Create_GraphicsWindow (this, "hello Mars", kInitial_window_width, kInitial_window_height, false)); 
   //
   init_graphics (sys); 
   //
   init_graphics_objects (); 
   //
   init_scene_objects (); 
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



   //
   //
   if (sy::Is_keydown (sy::SC_Q, kb) || sy::Is_keydown (sy::SC_ESCAPE, kb)) 
   {
      sys->SysReq (sy::SR_Exit); 
   }
   else
   {

      double camera_y_asp = double (view_dim[0]) / double(view_dim[1]); 
      viewparams.FoV = Ma::Pi * kPerspective_FoV / 180.0;
      viewparams.Asp_ratio = camera_y_asp ; 
      viewparams.dist_Near = kNear_plane_dist; 
      viewparams.dist_Far =  kFar_plane_dist; 
      static double Dt = 1.0;
      // -> is a quat mars.view_Rot, not euler
      Update_view_transform (viewparams.pos, viewparams.rot, Dt , kb, ms); 
   } 
     
}


//
// 
void Defer_test::render (sy::System_context* sys)
{   
   glViewport(0, 0, view_dim[0], view_dim[1]);
   
  
   //
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 
   
   
   
   glClearColor (0.7f, 0.8f, 0.9f, 1.0f); 
   
   //

   draw_simple();

//
//
//   // draw stuff 
//
//   //
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


void Defer_test::draw_simple ()
{
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();

   glUseProgram(shader_Table["basic_prog"]);

   // face cull
   static bool enable_culling = true;
   (enable_culling ? glEnable : glDisable) (GL_CULL_FACE);
   glFrontFace(GL_CCW);

   // polygon mode   
   static int DBG_polygon_mode = 2;
   GLenum polygon_Mode[] = { GL_POINT, GL_LINE, GL_FILL };
   glPolygonMode(GL_FRONT_AND_BACK, polygon_Mode[DBG_polygon_mode]);
   Validate_GL_call();

   glEnable(GL_TEXTURE_2D);
   Validate_GL_call();

   //
   // geometry streams  
   glEnableVertexAttribArray(attribLoc_map["vPosi"]);
   glVertexAttribPointer (attribLoc_map["vPosi"], 3, GL_FLOAT, GL_FALSE, 0, cube_geom);
   Validate_GL_call();
   glEnableVertexAttribArray(attribLoc_map["vTxcd"]);
   glVertexAttribPointer(attribLoc_map["vTxcd"], 2, GL_FLOAT, GL_FALSE, 0, cube_txc0);
   Validate_GL_call();


   //
   // view matrix 
   glm::dmat4 mat_View;
   {
      const glm::dvec3  v_right(1.0, 0.0, 0.0),
         v_up(0.0, 1.0, 0.0),
         v_fwd(0.0, 0.0, 1.0);

      mat_View = glm::translate(viewparams.pos) * glm::eulerAngleYX(viewparams.rot[1], viewparams.rot[0]);
      mat_View = glm::inverse(mat_View);

      float fMat_view[16]; // double precision (glUniformMatrix4dv) not available
      std::copy(glm::value_ptr(mat_View), glm::value_ptr(mat_View) + 16, fMat_view);
      glUniformMatrix4fv(uniformLoc_map["mat_View"], 1, GL_FALSE, fMat_view);

      Validate_GL_call();

   }

   //
   // projection 
   {

      glm::fmat4 matproj = glm::perspective(
         (float)viewparams.FoV,       //kPerspective_FoV,  
         (float)viewparams.Asp_ratio, //camera_y_asp,
         (float)viewparams.dist_Near, //kNear_plane_dist,  
         (float)viewparams.dist_Far   //kFar_plane_dist  
         );


      // float fMat_proj[16];// double precision (glUniformMatrix4dv) not available
      // std::copy (glm::value_ptr (mat_Proj), glm::value_ptr (mat_Proj) + 16, fMat_proj); 
      glUniformMatrix4fv(uniformLoc_map["mat_Proj"], 1, GL_FALSE, glm::value_ptr(matproj));
      Validate_GL_call();
   }

   //
   for (int i = 0; i < objs.size(); i++)
   {
      // this is draw loop 
      // set up grid first  // abstract data set into interface?   
      Validate_GL_call();

      const glm::fvec3& fPos = objs[i]->Pos();
      const glm::fvec3& fRot = objs[i]->Rot();
      const glm::fvec3& fScl = objs[i]->Scl();

      glm::fmat4 matScale = glm::scale(fScl);

      glm::fmat4 matRot = glm::rotate(fRot.x, glm::fvec3(1.0f, 0.0f, 0.0f)) 
                        * glm::rotate(fRot.y, glm::fvec3(0.0f, 1.0f, 0.0f)) 
                        * glm::rotate(fRot.z, glm::fvec3(0.0f, 0.0f, 1.0f)); 
      glm::fmat4 matPos = glm::translate(fPos);

      glm::fmat4 mat_Model = matPos * matRot * matScale;
      glUniformMatrix4fv(uniformLoc_map["mat_Model"], 1, GL_FALSE, glm::value_ptr(mat_Model));
      Validate_GL_call();

      //      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE); 
      objs[i]->Setup_RS(uniformLoc_map, attribLoc_map);
      glDrawArrays(GL_TRIANGLES, 0, El_count(cube_geom));

      Validate_GL_call();
   }

   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   glUseProgram(0);

   glEnable(GL_DEPTH_TEST);
   glDisableVertexAttribArray(attribLoc_map["patch_coord"]);
   glDisableVertexAttribArray(attribLoc_map["tex_coord"]);
   glMatrixMode(GL_MODELVIEW);
   glPopMatrix();

}



//
// main entry
int main (int argv, char** argc)
   {
std::shared_ptr<sy::System_context>  sys   (sy::Create_context ()); 
std::shared_ptr<Defer_test>          test  (Create_Defer_test(sys.get())); 
return sy::Run_realtime_task (sys.get(), test.get()); 
   }





void make_cube_textures()
{
   FreeImage_Initialise();
   const std::string imgroot = "C:/Quarantine/awsum/Cubes/";
   const std::string rgbroot = "C:/Quarantine/awsum/Cubes/rgb/";

   const char* files[] = {
      "169cover.jpg",
      "22cbf28a6a79cb48d46629c24907a470.jpg",
      "abstract_colorful_textures_widescreen_desktop_background_picture - 318.jpg",
      "abstract_colorful_textures_widescreen_desktop_background_picture.jpg",
      "blue - square - pattern.jpg",
      "cloud_texture_by_nos2002.jpg",
      "colorful - textures - high - resolution - 1024x576.jpg",
      "colorful - textures - stock - images1 - 1024x640.jpg",
      "colorful - textures - wide - awesome - 1024x640.jpg",
      "digital_art_texture_68_by_mercurycode - d713utv.jpg",
      "fountain_water_texture_2_by_fantasystock.jpg",
      "images.jpg",
      "manhole_texture_4250873.JPG",
      "nbnGLL0.jpg",
      "space_texture_by_extince.jpg",
      "squares - texture - wallpaper - 15899.jpg",
      "textures_131_by_inthename_stock.jpg",
      "texture_102_by_sirius_sdz - d1rlzx7.jpg",
      "texture_304_by_sirius_sdz - d5l8pu6.jpg",
      "texture_326_by_sirius_sdz - d65gs3s.jpg",
      "watercolor_painting_texture_by_enchantedgal_stock.jpg",

   };

   size_t numfiles = El_count(files);

   int imgcount = 0;

   RGBQUAD rgb;
   for (int i = 0; i < numfiles; i++)
   {
      std::string currfile = imgroot + files[i];
      if (FIBITMAP* img = FreeImage_Load(FIF_JPEG, currfile.c_str()))
      {
         ptru        dat = { FreeImage_GetBits(img) };
         glm::ivec2  dim(FreeImage_GetWidth(img), FreeImage_GetHeight(img));
         int         bpp = FreeImage_GetBPP(img);
         FREE_IMAGE_COLOR_TYPE ctyp = FreeImage_GetColorType(img);
         BITMAPINFOHEADER* info = FreeImage_GetInfoHeader(img);
         FREE_IMAGE_TYPE ityp = FreeImage_GetImageType(img);

         if (dim.x >= 512 && dim.y >= 512)
         {
            const std::string fname = rgbroot + "tex_" + boost::lexical_cast<std::string> (imgcount);
            std::shared_ptr<FILE> ws(fopen(fname.c_str(), "wb"), fclose);

            for (int iy = 0; iy < 512; iy++)
            {
               for (int ix = 0; ix < 512; ix++)
               {
                  FreeImage_GetPixelColor(img, ix, iy, &rgb);
                  fwrite(&rgb, 1, 3, ws.get());
               }
            }
            imgcount++;
         }

         FreeImage_Unload(img);
      }
      else
      {
         printf("\n Failed loading: %s", currfile.c_str());
      }

   }

   FreeImage_DeInitialise();
}
