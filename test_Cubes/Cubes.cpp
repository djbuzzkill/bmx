
#include "Cubes.h"



const int kInitial_window_width     = 1024;
const int kInitial_window_height    = 768;

static double  kNear_plane_dist     = 200.0;
static double  kFar_plane_dist      = 30000.0;
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

   virtual glm::dvec3&  Get_pos  () { return pos; } 
   virtual glm::dvec3&  Get_rot  () { return rot; } 
   virtual GLuint       Bin_ID   () { return 0; } 
   virtual GLuint       ROp_ID   () { return 0; } 

   virtual void         Setup_RS(const Rn::UniformMap& uniformMap, const Rn::AttributeMap& attribMap)
   {}

   glm::dvec3           pos; 
   glm::dvec3           rot; 
   glm::dvec3           scl; 
   GLuint               txrID; 
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

   virtual glm::dvec3&  Pos   () { return pos; } 
   virtual glm::dvec3&  Rot   () { return rot; } 
   virtual GLuint       Bin_ID() { return 0; } 
   virtual GLuint       ROp_ID() { return 0; } 

   virtual void         Setup_RS(const Rn::UniformMap& uniformMap, const Rn::AttributeMap& attribMap)
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
   }

   wat ();

   

}

void Defer_test::init_scene_objects () 
{
   const std::string rgbroot = "C:/Quarantine/awsum/Cubes/rgb/";
      
   int wd = 512;
   int ht = 512;
   int numpixels = wd * ht;

   const char* files[]= {
      "tex_0",
      "tex_1",
      "tex_2",
      "tex_3",
      "tex_4",
      "tex_5",
      "tex_6",
      "tex_7",
      };
   int numfiles = El_count (files); 
   

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

   wat (); 


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
      static double Dt = 20.0;
      // -> is a quat mars.view_Rot, not euler
      Update_view_transform (viewparams.pos, viewparams.rot, Dt , kb, ms); 
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
