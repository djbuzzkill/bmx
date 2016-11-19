
#include "Balls.h"



const int kInitial_window_width     = 1024;
const int kInitial_window_height    = 768;

static double  kNear_plane_dist     = 30.0;
static double  kFar_plane_dist      = 6000.0;
static double  kPerspective_FoV     = 60.0; 
const float    kMaskVal             = -2553.0f;

void Make_Mars_tiles       ();
void Make_Mars_normal_map  (float height_scale); 

UniformDef ModelUniform = { "mat_Model", UT_MAT4F, 1, 0 };
UniformDef ViewUniform  = { "mat_View", UT_MAT4F, 1, 0 }; 
UniformDef ProjUniform  = { "mat_Proj", UT_MAT4F, 1, 0 };
UniformDef ColorMap     = { "colorMap", UT_SAMPLER, 1, 0 };
UniformDef LightPos     = { "lit_Pos0", UT_VEC3F, 1, 0 };  

Dx::Resource_def BasicCubeGeom[] = { 
   { "indices", Dx::RD_USHORT },
   { "posi", Dx::RD_VEC3F },
   { "norms", Dx::RD_VEC3F },
   { "txcd0", Dx::RD_VEC2F },
   };



UniformDef ModelRotation = { "mat_ModelRot", UT_MAT3F, 1, 0 };

UniformDef CubeUniforms[] = {
   ModelUniform , 
   ViewUniform  , 
   ProjUniform  , 
   //ModelRotation, 
   ColorMap    , 
   LightPos
   };

AttributeDef VertPosi = { "vPosi", AT_VEC3F, 0 };
AttributeDef VertTxcd = { "vTxcd", AT_VEC2F, 0 };
AttributeDef VertNorm = { "vNorm", AT_VEC3F, 0 };


AttributeDef CubeAttributes[] = {
   VertPosi,
   VertTxcd,
   VertNorm,
   };




//
//
struct Simple_obj : public Renderable
{

   Simple_obj() {
   }

   virtual glm::fvec3&  Pos() { return pos; }
   virtual glm::fvec3&  Rot() { return rot; }
   virtual glm::fvec3&  Scl() { return scl; }

   virtual GLuint       Bin_ID() { return 0; }
   virtual GLuint       ROp_ID() { return 0; }

   virtual void Setup_RS(
      const Rn::UniformMap& uniformMap,
      const Rn::UniformValueMap& uniformVals,
      const Rn::AttributeMap& attribMap);

   glm::fvec3           pos;
   glm::fvec3           rot;
   glm::fvec3           scl;
   GLuint               txrID;
};

//
void Simple_obj::Setup_RS(
   const Rn::UniformMap& uniformMap,
   const Rn::UniformValueMap& uniformVals,
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
   Update_uniform(uniformMap, uniformVals, ColorMap);
   //glUniform1i(col_It->second, texture_stage);
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

   Light_obj() {
   }

   virtual glm::fvec3&  Pos() { return pos; }
   virtual glm::fvec3&  Rot() { return rot; }
   virtual glm::fvec3&  Scl() { return scl; }
   virtual GLuint       Bin_ID() { return 0; }
   virtual GLuint       ROp_ID() { return 0; }

   virtual void Setup_RS(
      const Rn::UniformMap& uniformMap,
      const Rn::UniformValueMap& uniformVals,
      const Rn::AttributeMap& attribMap)
   {
   }

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

   enum shading { 
      shade_basic,
      shade_lighting
   };

   void init_graphics_objects (); 
   void init_scene_objects    (); 

   void update_scene(sy::System_context* sys);

   shading shading_mode;
   void draw_simple           (); 
   void draw_lighting         (); 
   void draw_deferred         (); 
   // 
   Rn::ShaderTable      shader_Table;
   Rn::UniformMap       uniformLoc_map;
   Rn::AttributeMap     attribLoc_map;
   Rn::UniformValueMap  uniformValueMap;

   std::shared_ptr<sy::Graphics_window>   windo; 
   bool                                   init_; 
   glm::ivec2                             view_dim; 
   glm::fmat4                             matrices[3];
   glm::fmat3                             matrot;
   glm::fvec3                             lightPos; 
   MRT_frame_buffer              framebuffer;
   View_params                   viewparams;

   std::vector<glm::fvec3> warped_verts;
   std::vector<glm::fvec3> warped_norms;
   std::vector<glm::fvec2> warped_txcrd;


   struct DeferDat {
      std::vector<Light_obj>     lights; 
      std::vector<Simple_obj>    cubes; 
      std::vector<Renderable*>   rend_lit; 
      std::vector<Renderable*>   rend_obj; 
      } dat; 

   std::vector<Renderable*>   objs;
   std::vector<glm::fvec3>    objrot;
   std::vector<glm::fvec3>    objpos;

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
   , shading_mode(shade_lighting)
   , lightPos(-500.0f, 2000.0f, 500.0f)
   , dat()
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
}

void Defer_test::init_graphics_objects () 
{
   viewparams.pos[0] = 0.0f;
   viewparams.pos[1] = 300.0f;
   viewparams.pos[2] = 0.0f;

   viewparams.rot[0] = 0.0f; // glm::pi<double>();
   viewparams.rot[1] = glm::pi<double>();

   MRT_Initialize(framebuffer, kInitial_window_width, kInitial_window_height);
   

   std::vector<char> vs, fs;
   switch (shading_mode)
   {  
   // basic shader
   case shade_basic: {
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
      } break;
   // simple lighting
   case shade_lighting:  {
      ut::Load_text_file(vs, "C:/Quarantine/hardcore/shader/Cubes/simple_light.vp");
      ut::Load_text_file(fs, "C:/Quarantine/hardcore/shader/Cubes/simple_light.fp");
      shader_Table["light_vp"] = Create_shader((GLchar*)vs.data(), GL_VERTEX_SHADER);
      shader_Table["light_fp"] = Create_shader((GLchar*)fs.data(), GL_FRAGMENT_SHADER);
      GLuint shaders[3] = {
         shader_Table["light_vp"],
         shader_Table["light_fp"],
         0
         };
      shader_Table["light_prog"] = Build_shader_program(shaders);
      } break;
   //
   default: 
	   //DX_ASSERT ("invalid shader"); 
	   break;
   }

   Uniform_locations    (uniformLoc_map, CubeUniforms, El_count(CubeUniforms), shader_Table["light_prog"]);
   Attribute_locations  (attribLoc_map, CubeAttributes, El_count(CubeAttributes), shader_Table["light_prog"]);


   uniformValueMap["mat_Model"].p      = &matrices[0];
   uniformValueMap["mat_View"].p       = &matrices[1];
   uniformValueMap["mat_Proj"].p       = &matrices[2];
   //uniformValueMap["mat_ModelRot"].p   = glm::value_ptr(matrot); // stage 0
   uniformValueMap["colorMap"].i       = 0; // stage 0
   uniformValueMap["lit_Pos0"].p       = glm::value_ptr(lightPos);

   //UniformDef ModelRotation = { "mat_ModelRot", UT_MAT3F, 1, 0 };

   wat ();

   

}

void Defer_test::init_scene_objects () 
{
   Dx::Resource_obj warped_res; 
   Dx::Read_resource(warped_res, "C:/usr/qube10.geom");
   
   (BasicCubeGeom, El_count(BasicCubeGeom));

   warped_verts.resize(warped_res.fieldMap["indices"].count);
   warped_norms.resize(warped_res.fieldMap["indices"].count);
   warped_txcrd.resize(warped_res.fieldMap["indices"].count);

   unsigned short*   inds = static_cast<unsigned short*> (warped_res.fieldMap["indices"].mem);
   glm::fvec3*       verts = static_cast<glm::fvec3*> (warped_res.fieldMap["posi"].mem);
   glm::fvec3*       norms = static_cast<glm::fvec3*> (warped_res.fieldMap["norms"].mem);
   glm::fvec2*       txcd0 = static_cast<glm::fvec2*> (warped_res.fieldMap["txcd0"].mem);

   for (int i = 0; i < warped_res.fieldMap["indices"].count; i++)
   {
      warped_verts[i] = verts[inds[i]]; 
      warped_norms[i] = norms[inds[i]]; 
      warped_txcrd[i] = txcd0[inds[i]]; 
   }


   const std::string rgbroot = "C:/Quarantine/awsum/Cubes/rgb/";
      
   int wd = 512;
   int ht = 512;
   int numpixels = wd * ht;

   char* files[]= {
      "tex_2",
      "tex_6",
      "tex_3",
      "tex_0",
      "tex_7",
      "tex_4",
      "tex_5",
      "tex_1",
   };
   int numfiles = El_count (files); 
   int random_shuffle = 21; 

   for (int i = 0; i < random_shuffle; i++)
      std::random_shuffle (files, files + numfiles); 

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

   // setup objects
   objs.clear();
   dat.cubes.resize(numfiles);

   float fRotMult = Ma::TwoPi * 0.01;
   float fPi      = Ma::Pi * 0.01f;
   objrot.resize (numfiles);
   objpos.resize (numfiles);

   for (int i = 0; i < dat.cubes.size(); i++)
   {

      if (i) {
         dat.cubes[i].pos = glm::fvec3(float(rand() % 2000) - 1000.0f, 100.0f + float(rand() % 500), float(rand() % 2000) - 1000.0f);
         dat.cubes[i].scl = glm::fvec3(50.0f, 50.0f, 50.0f);
         dat.cubes[i].rot = glm::fvec3(fRotMult * float(rand() % 100) - fPi, fRotMult * float(rand() % 100) - fPi, fRotMult * float(rand() % 100) - fPi);
         objrot[i] = glm::fvec3(0.0001f * ((rand() % 200) - 100.0f), 0.0001f * ((rand() % 200) - 100.0f), 0.0001f * ((rand() % 200) - 100.0f));
      }
      else {  //  be the floor
         dat.cubes[i].pos = glm::fvec3(0.0f, -2500.0f, 0.0f);
         dat.cubes[i].scl = glm::fvec3(1000.0f, 700.0f, 1000.0f);
         dat.cubes[i].rot = glm::fvec3(0.0f, 0.0f, 0.0f);
         objrot[i] = glm::fvec3();
         objpos[i] = glm::fvec3();
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
   windo.reset (sys->Create_GraphicsWindow (this, "Cubez", kInitial_window_width, kInitial_window_height, false)); 
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
   update_scene(sys);

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

void Defer_test::update_scene (sy::System_context* sys)
{


   for (int i = 1; i < objs.size(); i++)
   {
      //objpos[i] += glm::fvec3(0.001f * ((rand() % 200) - 100.0f), 0.001f * ((rand() % 200) - 100.0f), 0.001f * ((rand() % 200) - 100.0f));

      //objs[i]->Pos() += objpos[i]; 
      objs[i]->Rot() += objrot[i]; 

   }

}

//
// 
void Defer_test::render (sy::System_context* sys)
{   
   glViewport(0, 0, view_dim[0], view_dim[1]);

   switch (shading_mode)
   {
   case shade_basic: 
      glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 
      glClearColor (0.2f, 0.0f, 0.0f, 1.0f); 
      draw_simple();
      break;
   case shade_lighting: 

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
      glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
      draw_lighting();
      break;
   

   }

   

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

   
   // geometry streams  
   glEnableVertexAttribArray(attribLoc_map["vPosi"]);
   //glVertexAttribPointer(attribLoc_map["vPosi"], 3, GL_FLOAT, GL_FALSE, 0, cube_geom);
   glVertexAttribPointer(attribLoc_map["vPosi"], 3, GL_FLOAT, GL_FALSE, 0, warped_verts.data());
   Validate_GL_call();
   glEnableVertexAttribArray(attribLoc_map["vTxcd"]);
   glVertexAttribPointer(attribLoc_map["vTxcd"], 2, GL_FLOAT, GL_FALSE, 0, warped_txcrd.data());
   Validate_GL_call();

   //
   // view, proj 

   {
      glm::fmat4& mat_View = *(glm::fmat4*)uniformValueMap["mat_View"].p;
      mat_View = glm::translate(viewparams.pos) * glm::eulerAngleYX(viewparams.rot[1], viewparams.rot[0]);
      mat_View = glm::inverse(mat_View);
      Update_uniform(uniformLoc_map, uniformValueMap, ViewUniform);

      glm::fmat4& mat_Proj= *(glm::fmat4*)uniformValueMap["mat_Proj"].p;
      mat_Proj = glm::perspective(
         (float)viewparams.FoV,       //kPerspective_FoV,  
         (float)viewparams.Asp_ratio, //camera_y_asp,
         (float)viewparams.dist_Near, //kNear_plane_dist,  
         (float)viewparams.dist_Far   //kFar_plane_dist  
         );
      Update_uniform(uniformLoc_map, uniformValueMap, ProjUniform);
   }
   

   //
   for (int i = 0; i < objs.size(); i++)
   {
      // this is draw loop 
      // set up grid first  // abstract data set into interface?   
      Validate_GL_call();
      {
         glm::fmat4& mat_Model = *(glm::fmat4*)uniformValueMap["mat_Model"].p;
         const glm::fvec3& fPos = objs[i]->Pos();
         const glm::fvec3& fRot = objs[i]->Rot();
         const glm::fvec3& fScl = objs[i]->Scl();

         glm::fmat4 matScale = glm::scale(fScl);

         glm::fmat4 matRot = glm::rotate(fRot.x, glm::fvec3(1.0f, 0.0f, 0.0f)) 
                           * glm::rotate(fRot.y, glm::fvec3(0.0f, 1.0f, 0.0f)) 
                           * glm::rotate(fRot.z, glm::fvec3(0.0f, 0.0f, 1.0f)); 
         glm::fmat4 matPos = glm::translate(fPos);

         //matrot = glm::fmat3(matRot);
         //Update_uniform(uniformLoc_map, uniformValueMap, ModelRotation);


         mat_Model = matPos * matRot * matScale;
      }
      
      Update_uniform (uniformLoc_map, uniformValueMap, ModelUniform);


      //      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE); 
      objs[i]->Setup_RS(uniformLoc_map, uniformValueMap, attribLoc_map);
      glDrawArrays(GL_TRIANGLES, 0, warped_verts.size());

      Validate_GL_call();
   }


   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   glUseProgram(0);

   glEnable(GL_DEPTH_TEST);
   glMatrixMode(GL_MODELVIEW);
   glPopMatrix();

}


void Defer_test::draw_lighting ()
{
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();

   glUseProgram(shader_Table["light_prog"]);

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

   // geometry streams  
   glEnableVertexAttribArray(attribLoc_map["vPosi"]);
   glVertexAttribPointer(attribLoc_map["vPosi"], 3, GL_FLOAT, GL_FALSE, 0, warped_verts.data());
   Validate_GL_call();
   glEnableVertexAttribArray(attribLoc_map["vTxcd"]);
   glVertexAttribPointer(attribLoc_map["vTxcd"], 2, GL_FLOAT, GL_FALSE, 0, warped_txcrd.data());
   Validate_GL_call();

   glEnableVertexAttribArray(attribLoc_map["vNorm"]);
   glVertexAttribPointer(attribLoc_map["vNorm"], 3, GL_FLOAT, GL_FALSE, 0, warped_norms.data());
   Validate_GL_call();
   //
   // view, proj 
   {
      glm::fmat4& mat_View = *(glm::fmat4*)uniformValueMap["mat_View"].p;
      mat_View = glm::translate(viewparams.pos) * glm::eulerAngleYX(viewparams.rot[1], viewparams.rot[0]);
      mat_View = glm::inverse(mat_View);
      Update_uniform(uniformLoc_map, uniformValueMap, ViewUniform);

      glm::fmat4& mat_Proj = *(glm::fmat4*)uniformValueMap["mat_Proj"].p;
      mat_Proj = glm::perspective(
         (float)viewparams.FoV,       //kPerspective_FoV,  
         (float)viewparams.Asp_ratio, //camera_y_asp,
         (float)viewparams.dist_Near, //kNear_plane_dist,  
         (float)viewparams.dist_Far   //kFar_plane_dist  
         );
      Update_uniform(uniformLoc_map, uniformValueMap, ProjUniform);
   }

   {
      glm::fvec3& litPos = *(glm::fvec3*)uniformValueMap["lit_Pos0"].p;
      Update_uniform(uniformLoc_map, uniformValueMap, LightPos); 
   }
   //
   for (int i = 0; i < objs.size(); i++)
   {
      // this is draw loop 
      // set up grid first  // abstract data set into interface?   
      Validate_GL_call();
      {
         glm::fmat4& mat_Model = *(glm::fmat4*)uniformValueMap["mat_Model"].p;
         const glm::fvec3& fPos = objs[i]->Pos();
         const glm::fvec3& fRot = objs[i]->Rot();
         const glm::fvec3& fScl = objs[i]->Scl();

         glm::fmat4 matScale = glm::scale(fScl);

         glm::fmat4 matRot = glm::rotate(fRot.x, glm::fvec3(1.0f, 0.0f, 0.0f))
            * glm::rotate(fRot.y, glm::fvec3(0.0f, 1.0f, 0.0f))
            * glm::rotate(fRot.z, glm::fvec3(0.0f, 0.0f, 1.0f));
         glm::fmat4 matPos = glm::translate(fPos);

         mat_Model = matPos * matRot * matScale;
      }

      Update_uniform(uniformLoc_map, uniformValueMap, ModelUniform);


      //      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE); 
      objs[i]->Setup_RS(uniformLoc_map, uniformValueMap, attribLoc_map);
      glDrawArrays(GL_TRIANGLES, 0, warped_verts.size());

      Validate_GL_call();
   }


   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   glUseProgram(0);

   glEnable(GL_DEPTH_TEST);
   glMatrixMode(GL_MODELVIEW);
   glPopMatrix();
}
void Defer_test::draw_deferred()
{
   MRT_BindForRendering(framebuffer);
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glUseProgram(shader_Table["deferred_geom_prog"]);



   // bind default
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
   glUseProgram(shader_Table["deferred_light_prog"]);

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
