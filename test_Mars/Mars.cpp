
#include "Mars.h"

const int kInitial_window_width  = 1024;
const int kInitial_window_height = 768;

static double  kNear_plane_dist    = 10.0;
static double  kFar_plane_dist     = 8000.0;
static double  kPerspective_FoV    = 60.0; 
const float    kMaskVal             = -2553.0f;

void Make_Mars_tiles       ();
void Make_Mars_normal_map  (float height_scale); 


//
//


std::string igm_tile_name(int X, int Y)
{
   return ("mars_tile_2f32_" + index_2_str (Y, X) + ".igm");
}

std::string height_tile_name (int Y, int X)
{
   return ("mars_tile_f32_" + index_2_str(Y, X) + ".hgt");
}

std::string color_tile_name (int Y, int X)
{
   return ("mars_tile_f32_" + index_2_str(Y, X) + ".col");
}

std::string normal_tile_name (int Y, int X)
{
   return ("F:/Quarantine/Mars/" + index_2_str (Y, X) + ".nrm");
}



/*

   //
   // STRUCT MarsTile_data
   struct MarsTile_data
      {

   MarsTile_data () : height(), color (), igm() {
      }

   std::vector<glm::f32>      height;
   std::vector<glm::f32>      color;
   std::vector<glm::dvec2>    igm;

      };

   //
   //
   MarsTile_data& Set (
      MarsTile_data& tile, 
      unsigned       numpxls, 
      glm::f32*      col = nullptr, 
      glm::f32*      hgt = nullptr, 
      glm::dvec2*    igm = nullptr) 
   {

      if (numpxls)
      {
         tile.height.resize(numpxls); 
         tile.color.resize (numpxls);
         tile.igm.resize   (numpxls); 

         tile.color.assign (col, col + numpxls); 
         tile.height.assign(hgt, hgt + numpxls); 
         tile.igm.assign   (igm, igm + numpxls); 
      }
      else
      {
         tile.height.clear (); 
         tile.color.clear  ();
         tile.igm.clear    (); 
      }
      return tile;
   }

*/
// typedef std::map<std::string, GLuint> UniformMap;
// typedef std::map<std::string, GLuint> AttributeMap;
// 
// struct Renderable {
// 
//    virtual glm::dvec3&  Get_pos  ()   = 0; 
//    virtual glm::dvec3&  Get_rot  ()   = 0; 
//    virtual GLuint       Bin_ID   ()   = 0; 
//    virtual GLuint       ROp_ID   ()   = 0; 
//    virtual void         Setup_RS (const UniformMap& uniformMap, const AttributeMap& attribMap) = 0; 
// 
// protected: 
//    Renderable() {
//       }
// };
// 
// 
// STRUCT Terrain_patch 
struct Terrain_patch : public Renderable
   {
   

   Terrain_patch() : pos(), rot(), col_ID(0), hgt_ID(0) {
      } 
   
   Terrain_patch(
      const glm::dvec3& p, 
      const glm::dvec3& r)  
   : pos(p), rot(r) , col_ID(0), hgt_ID(0) {
      } 

   virtual glm::dvec3&  Get_pos  () { return pos; } 
   virtual glm::dvec3&  Get_rot  () { return rot; } 
   virtual GLuint       Bin_ID() { return 0; } 
   virtual GLuint       ROp_ID() { return 0; } 
   
   virtual void Setup_RS(const Rn::UniformMap& uniformMap, const Rn::AttributeMap& attribMap)
   {
      bool  use_height_texture   = true; 
      int   texture_stage        = 0;

      BOOST_ASSERT (uniformMap.count ("heightMap")   ); 
      BOOST_ASSERT (uniformMap.count ("colorMap")    ); 

      Rn::UniformMap::const_iterator hgt_It    = uniformMap.find ("heightMap"); 
      Rn::UniformMap::const_iterator col_It    = uniformMap.find ("colorMap"); 
      
      // setup height texture
      if (use_height_texture)
      {
         glActiveTexture (GL_TEXTURE_stage (texture_stage)); 
         glBindTexture (GL_TEXTURE_2D, hgt_ID); 
         Validate_GL_call (); 
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      
         glUniform1i (hgt_It->second, texture_stage); 
         Validate_GL_call (); 
         texture_stage++; 
      }

      //
      // setup color texture
      glActiveTexture (GL_TEXTURE_stage (texture_stage)); 
      glBindTexture (GL_TEXTURE_2D, col_ID); 
      Validate_GL_call (); 

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      Validate_GL_call (); 
      
      glUniform1i (col_It->second, texture_stage); 
      Validate_GL_call (); 
      texture_stage++; 

      glEnable (GL_TEXTURE_2D); 
      Validate_GL_call (); 

   } 

   glm::dvec3  pos; 
   glm::dvec3  rot; 
   GLuint      col_ID; 
   GLuint      hgt_ID;
   GLuint      nrm_ID;

protected: 

   }; 




//
// CLASS Mars_test 
class Mars_test : public sy::RT_Task, public sy::Window_listener, public cx::Destructor
{

//   unsigned char* Get_color_tile (int X, int Y)
//   {
//      return mars.color_Tiles.get() + (Y * kTile_texture_dim * kTile_texture_dim * mars.terrain.num_X_tiles) + (X * kTile_texture_dim * kTile_texture_dim);
//   }
//
//   float* Get_height_tile (int X, int Y)
//   {
//      return mars.height_Tiles.get() + (Y * kTile_texture_dim * kTile_texture_dim * mars.terrain.num_X_tiles) + (X * kTile_texture_dim * kTile_texture_dim);
//   }
   
      // this is the resolution of the texture
   static const int kTile_texture_dim;
   // resolution of our test data
   static const int img_X_dim;
   static const int img_Y_dim; 

   // 
   std::shared_ptr<Terrain_renderer>      bg_renderer; 
   std::shared_ptr<sy::Graphics_window>   windo; 
   bool                                   init_; 
   glm::ivec2                             view_dim; 
   View_params                            viewparams;

   float tessFactor; 

   struct MarsDat
   {
      // tiles
      // sh_arr<float>                    height_Tiles; 
      // sh_arr<unsigned char>            color_Tiles; 
      // sh_arr<GLuint>                   txr_IDs; 
      Terrain_params             terrain; 
      std::vector<Terrain_patch> patch;                       
      std::vector<Renderable*>   robjs; 
      glm::fvec3                 patchScale; 
   } mars; 


   void init_terrain    (sy::System_context* sys);
   void init_graphics   (sy::System_context* sys);
   void update_input    (sy::System_context* sys); 
   void render          (sy::System_context* sys);

public: 

   Mars_test (sy::System_context* sys);
   virtual ~Mars_test ();

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



const int         Mars_test::kTile_texture_dim  = 1024; 
const int         Mars_test::img_X_dim          = 6900;          
const int         Mars_test::img_Y_dim          = 17177;          
const std::string Mars_test::kOut_path          = "J:/Quarantine/Mars"; 
const std::string Mars_test::kBase_fname        = "/ESP_018065_1975_RED_ESP_019133_1975_RED"; 


void Mars_test::init_graphics (sy::System_context* sys)
{
   
   glewInit (); 

   bg_renderer->Initialize_draw_context (); 
}

//
Mars_test::Mars_test (sy::System_context* sys)
   : windo     ()
   , init_     (false)
   , view_dim  (kInitial_window_width, kInitial_window_height)
   , mars      () 
   , tessFactor (6.9)
{  
   bg_renderer.reset  (Terrain_renderer::Create ( std::string("hw") )); 
}

//
Mars_test::~Mars_test ()
{
   Deinitialize (0); 
}

//const std::string MARS_TILE_PATH = "F:/Quarantine/Mars/";
const std::string MARS_TILE_PATH = "C:/Quarantine/awsum/Mars/tile/";




void make_tiles_from_programmer_art ()
{
   
   bool generate_tiles = false; 
   
   FreeImage_Initialise();
   static bool generate_colors = generate_tiles;
   if (generate_colors)
   { // color
      const std::string fname = "C:/Quarantine/awsum/rando/color_main_00.bin";
      const std::string basename = "rando_col_";
      const std::string outpath  = "C:/Quarantine/awsum/rando/tile";
      generate_color_tiles (fname, basename, 1280, 1280, outpath, 10, 10);
   }
   else
   {
      const std::string basename = "rando_col_";
      const std::string outpath  = "C:/Quarantine/awsum/rando/tile";
      for (int ity = 0; ity < 10; ity++)
      {
         for (int itx = 0; itx < 10; itx++)
         {
            std::string fcurr = outpath + "/" + basename + index_2_str(ity, itx) + "_3uc.col";
            std::shared_ptr<FILE> rs(fopen(fcurr.c_str(), "rb"), fclose);
            std::vector<glm::u8vec3> nrmtile(128 * 128);

            fread(nrmtile.data(), sizeof(glm::u8vec3), 128 * 128, rs.get());
            nrmtile.size();

         }
      }
   }

   static bool generate_height = generate_tiles;
   if (generate_height)
   { // height
      const std::string fname    = "C:/Quarantine/awsum/rando/height_main.bin";
      const std::string basename = "rando_hgt_";
      const std::string outpath  = "C:/Quarantine/awsum/rando/tile";
      float             scale    = 100.0f / 255.0f;
      generate_height_tiles(fname, basename, 2560, 2560, outpath, 10, 10, scale);
   }
   else
   {
      const std::string basename = "rando_hgt_";
      const std::string outpath = "C:/Quarantine/awsum/rando/tile";
      for (int ity = 0; ity < 10; ity++)
      {
         for (int itx = 0; itx < 10; itx++)
         {
            std::string fcurr = outpath + "/" + basename + index_2_str(ity, itx) + "_1f.hgt";
            std::shared_ptr<FILE> rs(fopen(fcurr.c_str(), "rb"), fclose);
            std::vector<float> nrmtile(256 * 256);

            fread(nrmtile.data(), sizeof(float), 256 * 256, rs.get());
            nrmtile.size();

         }
      }
   }

   static bool generate_normals = generate_tiles;
   if (generate_normals)
   {
      const std::string fname    = "C:/Quarantine/awsum/rando/height_main.bin";
      const std::string basename = "rando_nrm_";
      const std::string outpath  = "C:/Quarantine/awsum/rando/tile";
      float             scale    = 100.0f / 255.0f;
      generate_normal_tiles (fname, basename, 2560, 2560, outpath, 10, 10);
   }
   else
   {  // examine normal tiles
      //fname, basename, 2560, 2560, outpath, 10, 10);
      const std::string basename = "rando_nrm_";
      const std::string outpath  = "C:/Quarantine/awsum/rando/tile";
      for (int ity = 0; ity < 10; ity++)
      {
         for (int itx = 0; itx < 10; itx++)
         {
            std::string fcurr    = outpath + "/" + basename + index_2_str(ity, itx) + "_3f.nrm";
            std::shared_ptr<FILE> rs(fopen(fcurr.c_str(), "rb"), fclose);
            std::vector<glm::fvec3> nrmtile(256 * 256);

            fread(nrmtile.data(), sizeof(glm::fvec3), 256 * 256, rs.get());
            nrmtile.size();
         }
      }
   }

   const std::string root  = "C:/Quarantine/awsum/Mars";
   const std::string height= "height_main.png"  ;
   const std::string color = "color_main.00.png";

   FreeImage_DeInitialise();
}


//
void Make_tiles_from_image ()
{
   // tile dim: 7x17
   const int kXdim = 10; 
   const int kYdim = 10; 

   FreeImage_Initialise();

   for (int iY = 0; iY < kYdim; iY++)
   { 
      for (int iX = 0; iX < kXdim; iX++)
      {
         std::string fname_hgt = height_tile_name(iY, iX);
         std::string fname_col = color_tile_name (iY, iX);
      }
   }

   FreeImage_DeInitialise();
}

double Height_at (double xpos, double zpos)
{
   return 0; 
}

typedef std::tuple<double, double> minmax;
double& mn(minmax& ext) { return std::get<0>(ext); }
double& mx(minmax& ext) { return std::get<1>(ext); }

const double& mn(const minmax& ext) { return std::get<0>(ext); }
const double& mx(const minmax& ext) { return std::get<1>(ext); }
//
void Mars_test::init_terrain(sy::System_context* sys)
{
   using namespace glm; 

   const int col_dim = 128;
   const int hgt_dim = 256;
   const int nrm_dim = 256;

   const int num_col_pxls = col_dim * col_dim;
   const int num_hgt_pxls = hgt_dim * hgt_dim;
   const int num_nrm_pxls = nrm_dim * nrm_dim;

   mars.terrain.num_X_tiles         = 10;
   mars.terrain.num_Y_tiles         = 10;
   mars.terrain.easting_Tile_step   = 256.0;
   mars.terrain.northing_Tile_step  = 256.0;
   mars.terrain.height_offset       = 0.0;
   mars.terrain.height_range        = 100.0;
   
   const std::string tile_root = "C:/Quarantine/awsum/rando/tile";

   const int total_num_tiles = mars.terrain.num_X_tiles * mars.terrain.num_Y_tiles;

   //
   // prepare terrain/tiles + opengl specific things 
   int total_tiles = mars.terrain.num_X_tiles * mars.terrain.num_Y_tiles;
   mars.patch.resize(total_tiles);
   mars.robjs.resize(total_tiles);

   std::vector<GLuint> txrIDs(3 * total_tiles);
   glGenTextures(3 * total_tiles, txrIDs.data());
   for (int i = 0; i < total_tiles; i++)
   {
      mars.robjs[i] = &mars.patch[i];
      mars.patch[i].col_ID = txrIDs[3 * i + 0];
      mars.patch[i].hgt_ID = txrIDs[3 * i + 1];
      mars.patch[i].nrm_ID = txrIDs[3 * i + 2];
   }

   // because imagery is reverse the offsets start on opposite corner
   double total_Northing_dist = mars.terrain.northing_Tile_step * mars.terrain.num_Y_tiles;
   double total_Easting_dist = mars.terrain.easting_Tile_step * mars.terrain.num_X_tiles;

   std::vector<u8vec3>  coldat (num_col_pxls);
   std::vector<float>   hgtdat (num_hgt_pxls);
   std::vector<fvec3>   nrmdat (num_nrm_pxls);

   // init patch/terrain vars + bind/upload textures 
   for (int iY = 0; iY < mars.terrain.num_Y_tiles; iY++)
   {
      for (int iX = 0; iX < mars.terrain.num_X_tiles; iX++)
      {
         int tile_ind = iY * mars.terrain.num_X_tiles + iX;

         const std::string colt = tile_root + "/rando_col_" + index_2_str(iY, iX) + "_3uc.col"; 
         const std::string hgtt = tile_root + "/rando_hgt_" + index_2_str(iY, iX) + "_1f.hgt"; 
         const std::string nrmt = tile_root + "/rando_nrm_" + index_2_str(iY, iX) + "_3f.nrm"; 

         std::shared_ptr<FILE> c_(fopen(colt.c_str(), "rb"), fclose);
         std::shared_ptr<FILE> h_(fopen(hgtt.c_str(), "rb"), fclose);
         std::shared_ptr<FILE> n_(fopen(nrmt.c_str(), "rb"), fclose);

         fread(coldat.data(), sizeof(u8vec3), num_col_pxls, c_.get());
         fread(hgtdat.data(), sizeof(float), num_hgt_pxls, h_.get());
         fread(nrmdat.data(), sizeof(fvec3), num_nrm_pxls, n_.get());
         // color
         glBindTexture(GL_TEXTURE_2D, mars.patch[tile_ind].col_ID);
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, col_dim, col_dim, 0, GL_RGB, GL_UNSIGNED_BYTE, coldat.data());
         Validate_GL_call();
         // height
         glBindTexture(GL_TEXTURE_2D, mars.patch[tile_ind].hgt_ID);
         glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, hgt_dim, hgt_dim, 0, GL_RED, GL_FLOAT, hgtdat.data());
         Validate_GL_call();
         // normal
         glBindTexture(GL_TEXTURE_2D, mars.patch[tile_ind].nrm_ID);
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, nrm_dim, nrm_dim, 0, GL_RED, GL_FLOAT, nrmdat.data());
         Validate_GL_call();

         // the pos doesnt change
         mars.patch[tile_ind].pos = glm::dvec3(iX * mars.terrain.easting_Tile_step, 0.0, total_Northing_dist - iY * mars.terrain.northing_Tile_step);

         // always 0 for now
         mars.patch[tile_ind].rot = glm::dvec3(0.0, 0.0, 0.0);
      }
   }

   float ht_range = 100.0;

   mars.patchScale.x = mars.terrain.easting_Tile_step;
   mars.patchScale.y = 1.0;
   mars.patchScale.z = mars.terrain.northing_Tile_step;

   bg_renderer->Patch_scale(mars.patchScale.x, mars.patchScale.y, mars.patchScale.z);
   viewparams.pos[0] = 1000.0;
   viewparams.pos[1] = Height_at(viewparams.pos[0], viewparams.pos[2]) + 100.0;
   viewparams.pos[2] = 1000.0;;

   viewparams.rot[0] = 0.5* glm::pi<double>();
   viewparams.rot[1] = glm::pi<double>();


}

int Mars_test::Initialize(sy::System_context* sys)
{
   // Make_Mars_tiles (); 
   void* saddr = &sys; 
   printf ("\naddr: %p", &sys);

   windo.reset (sys->Create_GraphicsWindow (this, "hello Mars", kInitial_window_width, kInitial_window_height, false)); 

   init_graphics(sys);


   minmax colext (0.0, 0.0);
   mn(colext) = 1.0; 
   mx(colext) = 1.0; 


#ifdef USE_ORIGINAL_MARS_DATA
   //
   // all images are the same dimensions: 6900x17177
   (Mars_test::img_X_dim, Mars_test::img_Y_dim);         


   const int num_pxls   = img_X_dim * img_Y_dim ; 

   const int num_tile_pxls = Mars_test::kTile_texture_dim * Mars_test::kTile_texture_dim;

   // textures overlap with previous pixel, except first
   // Tiles = (Pixels - 1) / 1023
   mars.terrain.num_X_tiles = img_X_dim / Mars_test::kTile_texture_dim; 
   mars.terrain.num_Y_tiles = img_Y_dim / Mars_test::kTile_texture_dim; 

   // maybe extra tile for remainder  
   int partial_X_tile = img_X_dim % Mars_test::kTile_texture_dim; 
   int partial_Y_tile = img_Y_dim % Mars_test::kTile_texture_dim; 

   // maybe extra tile for remainder  
   mars.terrain.num_X_tiles += (partial_X_tile) ? 1 : 0; 
   mars.terrain.num_Y_tiles += (partial_Y_tile) ? 1 : 0; 
 
   // 
   // 
   int total_num_tiles = mars.terrain.num_X_tiles * mars.terrain.num_Y_tiles; 

   //
   // last tile index each dimension
   int last_X_tile_ind  = mars.terrain.num_X_tiles - 1; 
   int last_Y_tile_ind  = mars.terrain.num_Y_tiles - 1;
   int overlap_dim      = kTile_texture_dim - 1; 

   // color 
   minmax<float> colexts = { 0.0f, 0.0f }; // init to 0 is ok because we know range pases thru zero
   {    
      for (int iY = 0; iY < mars.terrain.num_Y_tiles; iY++)
      {
         for (int iX = 0; iX < mars.terrain.num_X_tiles; iX++)
         {
            std::string             fname = color_tile_name (iY, iX); 
            std::shared_ptr<FILE>   f (fopen (fname.c_str(), "rb"), fclose); 
            sh_arr<float>           fdat  (new float[num_tile_pxls]); 
            ptru                    colp = { fdat.get() }; 
            fread(colp.v, sizeof(float), num_tile_pxls, f.get());
            f.reset(); 

            for (int iPx = 0; iPx < num_tile_pxls; iPx++)
            {
               if (colp.f[iPx] > -1.0)
               {
               colexts.mn = (std::min) (colexts.mn , colp.f[iPx]); 
               colexts.mx = (std::max) (colexts.mx , colp.f[iPx]); 
               }
            }
         }
      }
   }

   // height
   minmax<float> hgtexts = { 0.0f, -2553.0f };
   {  
      for (int iY = 0; iY < mars.terrain.num_Y_tiles; iY++)
      {
         for (int iX = 0; iX < mars.terrain.num_X_tiles; iX++) 
         {
            std::string             fname    = height_tile_name (iY, iX); 
            std::shared_ptr<FILE>   f        (fopen(fname.c_str(), "rb"), fclose); 
            sh_arr<float>           dat      (new float[num_tile_pxls]); 
            ptru                    srcp     = { dat.get() }; 
            fread (srcp.v, sizeof(float), num_tile_pxls, f.get());

            for (int ipxl = 0; ipxl < num_tile_pxls; ipxl++) 
            {
            if (srcp.f[ipxl] > -2300.0f)
               {
            hgtexts.mn = (std::min) (hgtexts.mn , srcp.f[ipxl]); 
            hgtexts.mx = (std::max) (hgtexts.mx , srcp.f[ipxl]); 
               }
            }
         }
      }

   }


   // precomputd values
   mars.terrain.easting_Tile_step = 1834.3372934956; 
   mars.terrain.northing_Tile_step = 1911.6892137200; 

   if (false) // coord
   {  
      
      //
      // the IGM was was only used to analyze the approximate the values 
      // for more accurate scaling. not used as actual UVs for rendering
      std::string             kFname_coord   = kOut_path + kBase_fname + "-IGM.dat"; 
      std::shared_ptr<FILE>   f              (fopen (kFname_coord.c_str(), "rb"), fclose); 
      sh_arr<double>          fdat_          (new double[2 * num_pxls]); 
      ptru coords =  { fdat_ .get() }; 

      minmax<double> E_exts; 
      minmax<double> N_exts; 


      fread (coords.v, sizeof(double), 2 * num_pxls, f.get()); 

      E_exts.mn = coords.en[0].e; 
      N_exts.mn = coords.en[0].n; 
      E_exts.mx = coords.en[0].e; 
      N_exts.mx = coords.en[0].n; 

      
      int last_sampl_ind = img_X_dim - 1; 
      int last_line_ind = img_Y_dim - 1; 

      EN_coord top_left    = coords.en[img_X_dim * 0             + 0];
      EN_coord top_right   = coords.en[img_X_dim * 0             + last_sampl_ind];
      EN_coord bot_right   = coords.en[img_X_dim * last_line_ind + last_sampl_ind];
      EN_coord bot_left    = coords.en[img_X_dim * last_line_ind + 0];

      for (int iPx = 0; iPx < num_pxls; iPx++) 
         { 
      E_exts.mn = (std::min) (E_exts.mn, coords.en[iPx].e);
      N_exts.mn = (std::min) (N_exts.mn, coords.en[iPx].n);
      E_exts.mx = (std::max) (E_exts.mx, coords.en[iPx].e);
      N_exts.mx = (std::max) (N_exts.mx, coords.en[iPx].n);
         }

      std::vector<EN_coord> avg_E_dir (img_Y_dim);       


      for (int iN = 0; iN < img_Y_dim; iN++)
      {      
         int iX = img_X_dim - 1; 
         EN_coord dir = { 0.0, 0.0 }; 

         EN_coord* line = coords.en + img_X_dim * iN;
         while (iX)
            {
         EN_incr (dir, EN_sub (line[iX], line[iX - 1])); 
         iX--; 
            }  
         avg_E_dir[iN] = EN_dir (dir); 
      }

      // angle difference between first and last line
      double dtheta = acos (EN_dot (avg_E_dir[img_Y_dim - 1], avg_E_dir[0]));  
      double theta_degrs = glm::degrees<double>( dtheta ); 
                     
      std::vector<EN_coord> avg_N_dir (img_X_dim);       
      for (int iE = 0; iE < img_X_dim; iE++)
      {
         int iN = img_Y_dim - 1; 
         EN_coord dir = { 0.0, 0.0 }; 

         while (iN)
         {
            int prev_line_ind = img_X_dim * (iN-1) + iE; 
            int curr_line_ind = img_X_dim * iN + iE; 

            EN_incr (dir , EN_sub (coords.en [curr_line_ind], coords.en [ prev_line_ind ])); 
            iN--; 
         }

         avg_N_dir[iE] = EN_dir(dir); 

      }
      // angle difference between first and last column

      double dphi = acos (EN_dot (avg_N_dir[img_X_dim - 1] , avg_N_dir[0])); 
      double phi_degrs = glm::degrees<double>( dphi ); 

      // image data completely fills tile

      // tiles at the end will have masked data

      static bool use_preComp_val = true; 

      int num_whole_X_tiles = 0; 
      int num_whole_Y_tiles = 0; 

      //
      for (int iN = 0; iN < img_Y_dim; iN++)
      {
         EN_coord* line = coords.en + iN * img_X_dim; 
         for (int iE = 0; iE < num_whole_X_tiles; iE++) 
         {
            int pxl_ind  = iE * kTile_texture_dim; 
            mars.terrain.easting_Tile_step  += line[pxl_ind + kTile_texture_dim].e - line[pxl_ind].e;
         }
      }
      mars.terrain.easting_Tile_step /= (img_Y_dim * num_whole_X_tiles); 


      //
      for (int iE = 0; iE < img_X_dim; iE++)
      {
         for (int iN = 0; iN < num_whole_Y_tiles; iN++)
         {
         int pxl_ind = iN * kTile_texture_dim * img_X_dim + iE; 
         mars.terrain.northing_Tile_step += coords.en[pxl_ind].n - coords.en[pxl_ind + kTile_texture_dim * img_X_dim ].n ; 
         }
      } 
      mars.terrain.northing_Tile_step /= (num_whole_Y_tiles * img_X_dim); 

      f.reset(); 
   }


   //
   // prepare terrain/tiles + opengl specific things 
   int total_tiles      = mars.terrain.num_X_tiles * mars.terrain.num_Y_tiles; 
   mars.patch.resize    (total_tiles); 
   mars.robjs.resize    (total_tiles); 

   std::vector<GLuint> txrIDs (2 * total_tiles); 
   glGenTextures (2 * total_tiles, txrIDs.data());
   for (int i = 0; i < total_tiles; i++)
   {
      mars.robjs[i] = &mars.patch[i];
      mars.patch[i].col_ID = txrIDs[2 * i + 0]; 
      mars.patch[i].hgt_ID = txrIDs[2 * i + 1]; 
   }

   // because imagery is reverse the offsets start on opposite corner
   double total_Northing_dist = mars.terrain.northing_Tile_step * mars.terrain.num_Y_tiles; 
   double total_Easting_dist  = mars.terrain.easting_Tile_step * mars.terrain.num_X_tiles; 

   std::vector<float>         cdat    (num_tile_pxls); 
   std::vector<float>         hdat    (num_tile_pxls);
   std::vector<unsigned char> ucbuf   (num_tile_pxls); 
   

   // init patch/terrain vars + bind/upload textures 
   for (int iY = 0; iY < mars.terrain.num_Y_tiles; iY++)
   {
      for (int iX = 0; iX < mars.terrain.num_X_tiles; iX++) 
      {
         int tile_ind = iY * mars.terrain.num_X_tiles + iX; 
         
         std::shared_ptr<FILE> c_ (fopen (color_tile_name (iY, iX).c_str(), "rb"), fclose); 
         std::shared_ptr<FILE> h_ (fopen (height_tile_name (iY, iX).c_str(), "rb"), fclose); 

         fread (cdat.data(), sizeof(float), num_tile_pxls, c_.get()); 
         fread (hdat.data(), sizeof(float), num_tile_pxls, h_.get()); 

         // quantize color
         float colmul = 255.0f / colexts.mx - colexts.mn; 

         for (int ipxl = 0; ipxl < num_tile_pxls; ipxl++) 
            ucbuf[ipxl] =  static_cast<unsigned char> (colmul * (cdat[ipxl] - colexts.mn)); 

         //float hgtrange =  hgtexts.mx - hgtexts.mn; 

         glBindTexture  (GL_TEXTURE_2D, mars.patch[tile_ind].col_ID); 
         glTexImage2D   (GL_TEXTURE_2D, 0, GL_RED, BG_tile::kTextureWidth, BG_tile::kTextureHeight, 0, GL_RED, GL_UNSIGNED_BYTE, ucbuf.data());
         Validate_GL_call (); 

         glBindTexture  (GL_TEXTURE_2D, mars.patch[tile_ind].hgt_ID); 
         glTexImage2D   (GL_TEXTURE_2D, 0, GL_R32F, BG_tile::kTextureWidth, BG_tile::kTextureWidth, 0, GL_RED, GL_FLOAT, hdat.data()); 
         Validate_GL_call (); 

         // the pos doesnt change
         mars.patch[tile_ind].pos = glm::dvec3 (iX * mars.terrain.easting_Tile_step, 0.0, total_Northing_dist - iY * mars.terrain.northing_Tile_step); 

         // always 0 for now
         mars.patch[tile_ind].rot = glm::dvec3 (0.0, 0.0, 0.0); 
      }
   }

   float ht_range = hgtexts.mx - hgtexts.mn;

   mars.patchScale.x = mars.terrain.easting_Tile_step;
   mars.patchScale.y = 1.0;
   mars.patchScale.z = mars.terrain.northing_Tile_step ;

   bg_renderer->Patch_scale (mars.patchScale.x, mars.patchScale.y, mars.patchScale.z); 
   viewparams.pos[0] = 0.0; 
   viewparams.pos[1] = Height_at (viewparams.pos[0], viewparams.pos[2]) + 2.0;
   viewparams.pos[2] = 0.0;;

   viewparams.rot[0] = glm::pi<double>(); 
   viewparams.rot[1] = glm::pi<double>(); 
#else 

   init_terrain (sys);

#endif // USE_ORIGINAL_MARS_DATA   

   return 0;
}
  
//
int Mars_test::Deinitialize (sy::System_context*)
{
   int num_tiles  = mars.terrain.num_X_tiles * mars.terrain.num_Y_tiles;
   int num_tx_ids = 3 * num_tiles;

   std::vector<GLuint> v ; 
   v.reserve (num_tx_ids);

   for (int i = 0; i < mars.terrain.num_X_tiles * mars.terrain.num_Y_tiles; i++)
   {
      v.push_back (mars.patch[i].col_ID); 
      v.push_back (mars.patch[i].hgt_ID); 
   }
   glDeleteTextures (num_tx_ids, v.data()); 


   return 0;
}
  
//
int Mars_test::Update (sy::System_context* sys)
{
   //   
   update_input (sys); 
   
   // 
   render (sys); 

   return 0;
}
  
void Mars_test::update_input (sy::System_context* sys)
{
   sy::Keyboard_state   kb; 
   sy::Mouse_state      ms;
   sys->Poll_input(ms, kb);

   //
   //
   if (sy::Is_keydown (sy::SC_Q, kb) || sy::Is_keydown (sy::SC_ESCAPE, kb)) 
   {
      sys->SysReq (sy::SR_Exit); 
   }
   else
   {   

      if (sy::Is_keydown (sy::SC_LEFTBRACKET, kb))
         tessFactor -= 0.0567;

      if (sy::Is_keydown (sy::SC_RIGHTBRACKET, kb))
         tessFactor += 0.0567;

      // clamp
      tessFactor = glm::clamp (tessFactor , 1.0f, 200.0f); 


      if (sy::Is_keydown (sy::SC_H, kb))
         mars.patchScale.y += 0.032; 

      if (sy::Is_keydown (sy::SC_N, kb))
         mars.patchScale.y -= 0.032;

      bg_renderer->Patch_scale (mars.patchScale.x, mars.patchScale.y, mars.patchScale.z); 

      double camera_y_asp = double (view_dim[0]) / double(view_dim[1]); 
      viewparams.FoV = Ma::Pi * kPerspective_FoV / 180.0;
      viewparams.Asp_ratio = camera_y_asp ; 
      viewparams.dist_Near = kNear_plane_dist; 
      viewparams.dist_Far =  kFar_plane_dist; 
      static float Dt = 0.56;
      // -> is a quat mars.view_Rot, not euler
      Update_view_transform (viewparams.pos, viewparams.rot, Dt, kb, ms);
   } 
     
}


//
//
void Mars_test::render (sy::System_context* sys)
{   
   //
   glViewport (0, 0, view_dim[0] , view_dim[1]); 

   //
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 



   glClearColor (0.0f, 0.05f, 0.3f, 1.0f); 

   //
   
   bg_renderer->Tesselation_factors (tessFactor, tessFactor);
   
   bg_renderer->Draw (viewparams, mars.robjs); 

   //
   windo->Swap_buffers (); 
}



//
// window listener
void Mars_test::OnWindowResize (int wd, int ht) 
{
   // viewport_wd = wd; 
   // viewport_ht = ht;

   view_dim[0] = wd;
   view_dim[1] = ht;
}
   
//
void Mars_test::OnWindowClose ()  
{
   int wat = 0; wat++; 
}

//
void Mars_test::OnWindowActivate	(bool activate)
{
   int wat = 0; wat++; 
}
     

//
//
Mars_test* Create_Mars_test (sy::System_context* sys)
{
   return new Mars_test (sys); 
}



void Make_Mars_normal_map (float height_scale)
{
#ifdef USE_ORIGINAL_MARS_DATA
   using namespace glm;
   const int img_X_dim  = 6900;          
   const int img_Y_dim  = 17177;          
   const int num_pxls   = img_X_dim * img_Y_dim; 

   std::string             kFname_heigh   = Mars_test::kOut_path + Mars_test::kBase_fname + "-DEM.6900x17177.hgt";  
   std::shared_ptr<FILE>   f              (fopen (kFname_heigh.c_str(), "rb"), fclose); 
   sh_arr<float>           fdat_          (new float[num_pxls]); 
   ptru hgt = { fdat_.get() } ; 
   
   fread (hgt.f, sizeof(float), num_pxls, f.get()); 

   minmax<float> hgtexts; 

   hgtexts.mn = *std::min_element (hgt.f, hgt.f + num_pxls); 
   hgtexts.mx = *std::max_element (hgt.f, hgt.f + num_pxls); 


   char sScale[64]; 
   sprintf (sScale, "%2.3f", height_scale); 
   std::string             fname_norms   = Mars_test::kOut_path + Mars_test::kBase_fname + "-sf." + sScale + ".6900x17177.nrm"; 

   std::shared_ptr<FILE>   fout (fopen (fname_norms.c_str (), "wb")); 

   const float pixel_X_step = static_cast<float> (1834.3372934956 / 1000.0); 
   const float pixel_Y_step = static_cast<float> (1911.6892137200 / 1000.0); 

   fvec3 vNrm; 
   for (int iY = 0; iY < img_Y_dim; iY++) 
   {
      int Ynx = iY + 1;
      int Ypr = iY - 1;

      if (iY == 0) // first row, no pxls above
      {
         for (int iX = 0; iX < img_X_dim; iX++) 
         {
            int Xnx = iX + 1; 
            int Xpr = iX - 1; 
            
            float fHt = hgt.f[iY  * img_X_dim + iX];

            if (iX == 0) // first column
            {
               fvec3 vDn (0.0f, -pixel_Y_step , hgt.f[Ynx * img_X_dim + iX] - fHt); 
               fvec3 vRt (pixel_X_step, 0.0f  , hgt.f[iY * img_X_dim + Xnx] - fHt); 
               
               vNrm = cross (-vRt, vDn);                 
            }
            else
            if (Xnx == img_X_dim) // last col.
            {
               fvec3 vDn (0.0f, -pixel_Y_step , hgt.f[Ynx * img_X_dim + iX] - fHt); 
               fvec3 vLt (pixel_X_step, 0.0f  , hgt.f[iY * img_X_dim + Xpr] - fHt); 
               
               vNrm = cross (vLt, vDn);  
            }
            else
            {
               fvec3 vDn (0.0f, -pixel_Y_step, hgt.f[Ynx * img_X_dim + iX] - fHt); 
               fvec3 vLt (-pixel_X_step, 0.0f, hgt.f[iY * img_X_dim + Xpr] - fHt); 
               fvec3 vRt (pixel_X_step, 0.0f , hgt.f[iY * img_X_dim + Xnx] - fHt); 

               vNrm = cross (-vDn, vLt) + cross (-vRt, vDn); 
            }

            vNrm = normalize (vNrm); 

         }
      }
      else // last row, no pxls below
      if (Ynx == img_Y_dim)
      {
         for (int iX = 0; iX < img_X_dim; iX++) 
         {
            int Xnx = iX + 1; 
            int Xpr = iX - 1; 

            float fHt = hgt.f[iY  * img_X_dim + iX];

            if (iX == 0)
            {
               fvec3 vUp (0.0f, pixel_Y_step, hgt.f[Ypr * img_X_dim + iX] - fHt); 
               fvec3 vRt (pixel_X_step, 0.0f, hgt.f[iY * img_X_dim + Xnx] - fHt); 

               vNrm = cross (vUp , vRt);
            }
            else
            if (Xnx == img_X_dim)
            {
               fvec3 vUp (0.0f, pixel_Y_step , hgt.f[Ypr * img_X_dim + iX] - fHt); 
               fvec3 vLt (-pixel_X_step, 0.0f, hgt.f[iY * img_X_dim + Xpr] - fHt); 

               vNrm = cross (vUp, vLt); 
            }
            else
            {
               fvec3 vUp (0.0f, pixel_Y_step , hgt.f[Ypr * img_X_dim + iX] - fHt); 
               fvec3 vLt (-pixel_X_step, 0.0f, hgt.f[iY * img_X_dim + Xpr] - fHt); 
               fvec3 vRt (pixel_X_step, 0.0f , hgt.f[iY * img_X_dim + Xnx] - fHt); 

               vNrm = cross (vUp, vLt) + cross (vRt, vUp);
            }

            vNrm = normalize (vNrm); 
         }

      }
      else // inner rows
      {
         for (int iX = 0; iX < img_X_dim; iX++) 
         {
            int Xnx = iX + 1; 
            int Xpr = iX - 1; 

            float fHt = hgt.f[iY  * img_X_dim + iX];

            if (iX == 0)
            {
               fvec3 vUp (0.0f, pixel_Y_step , hgt.f[Ypr * img_X_dim + iX] - fHt); 
               fvec3 vDn (0.0f, -pixel_Y_step, hgt.f[Ynx * img_X_dim + iX] - fHt); 
               fvec3 vRt (pixel_X_step, 0.0f , hgt.f[iY * img_X_dim + Xnx] - fHt); 

               vNrm = cross (vRt, vUp) + cross (-vRt, vDn); 
            }
            else
            if (Xnx == img_X_dim)
            {
               fvec3 vUp (0.0f, pixel_Y_step , hgt.f[Ypr * img_X_dim + iX] - fHt); 
               fvec3 vDn (0.0f, -pixel_Y_step, hgt.f[Ynx * img_X_dim + iX] - fHt); 
               fvec3 vLt (-pixel_X_step, 0.0f, hgt.f[iY * img_X_dim + Xpr] - fHt); 
               
               vNrm = cross (vUp, vLt) + cross (-vDn, vLt); 
            }
            else
            {
               fvec3 vUp (0.0f, pixel_Y_step , hgt.f[Ypr * img_X_dim + iX] - fHt); 
               fvec3 vDn (0.0f, -pixel_Y_step, hgt.f[Ynx * img_X_dim + iX] - fHt); 
               fvec3 vLt (-pixel_X_step, 0.0f, hgt.f[iY * img_X_dim + Xpr] - fHt); 
               fvec3 vRt (pixel_X_step, 0.0f , hgt.f[iY * img_X_dim + Xnx] - fHt); 

               vNrm = cross (vUp, vLt) + cross (vLt, vDn) + 
                     cross (-vRt, vDn) + cross (vRt, vUp); 

            }

            vNrm = normalize (vNrm); 
         }
      }
   }


#endif
}

//
// FUNC Make_Mars_tiles 
void Make_Mars_tiles ()
{
#ifdef USE_ORIGINAL_MARS_DATA
   const int         kTile_txr_dim     = 1024; 
   const int         kMars_img_X_dim   = 6900;          
   const int         kMars_img_Y_dim   = 17177;          

      
       
   const std::string kOut_path         = Mars_test::kOut_path  ; "J:/Quarantine/Mars"; 
   const std::string kBase_fname       = Mars_test::kBase_fname; "/ESP_018065_1975_RED_ESP_019133_1975_RED"; 

   int         num_X_tiles                = kMars_img_X_dim / kTile_txr_dim;
   int         num_Y_tiles                = kMars_img_Y_dim / kTile_txr_dim;

   num_X_tiles += (num_X_tiles * kTile_txr_dim < kMars_img_X_dim) ? 1 : 0; 
   num_Y_tiles += (num_Y_tiles * kTile_txr_dim < kMars_img_Y_dim) ? 1 : 0; 

   int         last_tile_X_partial_pxls   = kMars_img_X_dim % kTile_txr_dim; 
   int         last_tile_Y_partial_pxls   = kMars_img_Y_dim % kTile_txr_dim; 
   const int   num_pxls                   = kMars_img_X_dim * kMars_img_Y_dim; 
   const int   num_txr_pxls               = kTile_txr_dim * kTile_txr_dim; 


   static bool gen_color_flag             = false; 
   static bool gen_height_flag            = false; 
   static bool gen_igm_flag               = false; 
   static bool gen_normal_tiles           = true;

   static bool inspect_tiles_flag         = false; 
   
   //
   // color
   if (gen_color_flag) 
   {  std::string             kFname_color   = kOut_path + kBase_fname + "-DRG.6900x17177.dat"; 
      std::shared_ptr<FILE>   f              (fopen (kFname_color.c_str(), "rb"), fclose); 
      sh_arr<float>           fdat_          (new float[num_pxls]); 

      ptru srcp =  {fdat_ .get()}; 
      fread (srcp.f, sizeof(float), num_pxls, f.get()); 
   
      for (int iY = 0; iY < num_Y_tiles ; iY++)
      {
         int tile_start_Y = iY * kTile_txr_dim * kMars_img_X_dim; 
         
         for (int iX = 0; iX < num_X_tiles; iX ++) 
         {
            int tile_start_X     = iX * kTile_txr_dim;

            // is it full row of pixels or partial?
            int num_tile_pixels = (iX == kMars_img_X_dim / kTile_txr_dim ? last_tile_X_partial_pxls : kTile_txr_dim); 
            // .. same for line
            int num_tile_lines   = (iY == kMars_img_Y_dim / kTile_txr_dim ? last_tile_Y_partial_pxls : kTile_txr_dim); 

            // 
            std::string col_fn = color_tile_name (iY, iX); 
            std::shared_ptr<FILE> mars_colorf (fopen(col_fn.c_str (), "wb"), fclose); 
            // default fill with mask val
            for (int i = 0; i < num_txr_pxls; i++) 
               fwrite (&kMaskVal, sizeof(float), 1, mars_colorf.get()); 
            // reset pos            
            fseek (mars_colorf.get(), SEEK_SET, 0); 

            for (int iln = 0; iln < num_tile_lines; iln++) 
            {
               fwrite (
                  srcp.f + tile_start_X + tile_start_Y + iln * kMars_img_X_dim, 
                  sizeof(float) , 
                  num_tile_pixels, 
                  mars_colorf.get()
                  ); 
            }

            mars_colorf.reset(); 
         }
      }
   }

   //
   // height
   if (gen_height_flag) 
   {  
      std::string             kFname_heigh   = kOut_path + kBase_fname + "-DEM.6900x17177.hgt";  
      std::shared_ptr<FILE>   f              (fopen (kFname_heigh.c_str(), "rb"), fclose); 
      sh_arr<float>           fdat_          (new float[num_pxls]); 

      ptru srcp =  {fdat_ .get()}; 
      fread (srcp.f, sizeof(float), num_pxls, f.get()); 

      for (int iY = 0; iY < num_Y_tiles ; iY++)
      {
         int tile_start_Y = iY * kTile_txr_dim * kMars_img_X_dim; 
         
         for (int iX = 0; iX < num_X_tiles; iX ++) 
         {
            int tile_start_X     = iX * kTile_txr_dim;
            int num_tile_pixels  = (iX == kMars_img_X_dim / kTile_txr_dim ? last_tile_X_partial_pxls : kTile_txr_dim); 
            int num_tile_lines   = (iY == kMars_img_Y_dim / kTile_txr_dim ? last_tile_Y_partial_pxls : kTile_txr_dim); 

            std::string hgt_fn = height_tile_name (iY, iX);
            std::shared_ptr<FILE> mars_heightf (fopen(hgt_fn.c_str (), "wb"), fclose); 
            
            for (int i = 0; i < num_txr_pxls; i++)
               fwrite(&kMaskVal, sizeof(float), 1, mars_heightf.get()); 

            fseek (mars_heightf.get(), SEEK_SET, 0); 


            for (int iln = 0; iln < num_tile_lines; iln++) 
            {
               fwrite (
                  srcp.f + tile_start_X + tile_start_Y + iln * kMars_img_X_dim, 
                  sizeof(float) , 
                  num_tile_pixels, 
                  mars_heightf.get()
                  ); 
            }

            mars_heightf.reset(); 

         }
      }
   }



   //
   // gen IGM
   if (gen_igm_flag) 
   {  
      std::string                   kFname_coord   = kOut_path + kBase_fname + "-IGM.dat"; 
      std::shared_ptr<FILE>         f              (fopen (kFname_coord.c_str(), "rb"), fclose); 
      sh_arr<EN_coord>   fdat_          (new EN_coord[num_pxls]); 
      
      ptru coords =  { fdat_ .get() }; 
      fread (coords.v, sizeof(EN_coord), num_pxls, f.get()); 

      for (int iY = 0; iY < num_Y_tiles ; iY++)
      {
         int tile_start_Y = iY * kTile_txr_dim * kMars_img_X_dim; 
         
         for (int iX = 0; iX < num_X_tiles; iX ++) 
         {
            int tile_start_X     = iX * kTile_txr_dim;
            int num_tile_pixels  = (iX == kMars_img_X_dim / kTile_txr_dim ? last_tile_X_partial_pxls : kTile_txr_dim); 
            int num_tile_lines   = (iY == kMars_img_Y_dim / kTile_txr_dim ? last_tile_Y_partial_pxls : kTile_txr_dim); 


            std::string igm_fn = igm_tile_name (iY, iX); 
            std::shared_ptr<FILE> mars_igmf (fopen (igm_fn.c_str (), "wb"), fclose); 
            
            for (int i = 0; i < num_txr_pxls; i++)
            {
               EN_coord _ = { -2553.0, -2553.0 }; 
               fwrite (&_, sizeof EN_coord, 1, mars_igmf.get()); 
            }
            fseek (mars_igmf.get(), SEEK_SET, 0); 

            for (int iln = 0; iln < num_tile_lines; iln++) 
            {
               
               fwrite (
                  coords.en + tile_start_X + tile_start_Y + iln * kMars_img_X_dim, 
                  sizeof(EN_coord), 
                  num_tile_pixels, 
                  mars_igmf.get()
                  ); 
            }

            mars_igmf.reset (); 
         }
      }
   }

   //
   // generate_normal_maps; 
   if (gen_normal_tiles)
   {


   }


   //
   //
   //
   if (inspect_tiles_flag) 
   {

      if (gen_height_flag)
      {
      std::string             hgt_fn = height_tile_name (8, 3);
      sh_arr<float>           hgtp (new float[num_txr_pxls] ); 
      std::shared_ptr<FILE>   hgtf (fopen(hgt_fn.c_str (), "rb"), fclose); 
      fread (hgtp.get(), sizeof(float), num_txr_pxls, hgtf.get()); 
      hgtp.get();
      }

      if (gen_color_flag)
      {
      std::string             col_fn = color_tile_name (8, 3);
      sh_arr<float>           colp (new float[num_txr_pxls] ); 
      std::shared_ptr<FILE>   colf (fopen(col_fn.c_str (), "rb"), fclose); 
      fread (colp.get(), sizeof(float), num_txr_pxls, colf.get()); 
      colp.get(); 
      }

      
      if (gen_igm_flag)
      {
      std::string                   igm_fn = igm_tile_name (10, 4);
      sh_arr<EN_coord>   igmp (new EN_coord[num_txr_pxls]); 
      std::shared_ptr<FILE>         igmf (fopen(igm_fn.c_str (), "rb"), fclose); 
      fread (igmp.get(), sizeof(EN_coord), num_txr_pxls, igmf.get()); 
      igmp.get();

      igm_fn = igm_tile_name (10, 3);
      igmf.reset (fopen(igm_fn.c_str (), "rb"), fclose); 
      fread (igmp.get(), sizeof(EN_coord), num_txr_pxls, igmf.get()); 
      igmp.get();

      igm_fn = igm_tile_name (9, 4);
      igmf.reset (fopen(igm_fn.c_str (), "rb"), fclose); 
      fread (igmp.get(), sizeof(EN_coord), num_txr_pxls, igmf.get()); 
      igmp.get();

      igm_fn = igm_tile_name (9, 3);
      igmf.reset (fopen(igm_fn.c_str (), "rb"), fclose); 
      fread (igmp.get(), sizeof(EN_coord), num_txr_pxls, igmf.get()); 
      igmp.get();

      EN_coord& c0 = igmp [0 * kTile_txr_dim];
      EN_coord& c1 = igmp [0 * kTile_txr_dim + 1023];
      EN_coord& c2 = igmp [1023 * kTile_txr_dim + 1023];
      EN_coord& c3 = igmp [1023 * kTile_txr_dim];
      igmp.get();

      }
   }

#endif

}



//
// main entry
int main (int argv, char** argc)
   {

   make_tiles_from_programmer_art();


std::shared_ptr<sy::System_context>  sys   (sy::Create_context ()); 
std::shared_ptr<Mars_test>          mars  (Create_Mars_test (sys.get())); 
return sy::Run_realtime_task (sys.get(), mars.get()); 
   }



// namespace Mars_private 
// {
// 
//    template<typename Ty>
// 	double Mean (const Ty* a, size_t n, size_t stride, size_t elem) {
// 		double sum = 0.0;
// 		size_t ind = elem;
// 		size_t last = n * stride + elem;
// 
// 		while (ind < last) {
// 			sum += a[ind];
// 			ind += stride;  
// 			}
// 
// 		return sum / n;
// 		}
//    
//    template<typename Ty>
// 	double StandardDeviation (const Ty* a, double dMean, size_t n, size_t stride, size_t elem) {
// 		double sum=0.0;
// 		size_t ind = elem;
// 		size_t last = n * stride + elem; 
// 		while (ind < last) { 
            
// 			sum += (a[ind] - dMean) * (a[ind] - dMean);
// 			ind += stride;
// 			}
// 
// 		return sqrt (sum / n);
// 		}
// 
// 	template <class T>
// 	void Scale_data (
// 		const T* data, 
// 		unsigned char* output, 
// 		double sigma, 
// 		int pixels_in, 
// 		int bands_in, 
// 		double alpha, 
// 		int bands_out, 
// 		const int* bandNumbers, 
// 		double* bandMeans, 
// 		double* bandStdDevs, 
// 		int recalculate = 1, 
// 		float* invalid_value = NULL
// 		);
// 
//    //
//    // Scale_data 
// 	template <class T>
// 	void Scale_data (
// 		const T* data, 
// 		unsigned char* output, 
// 		double sigma, 
// 		int pixels_in, 
// 		int bands_in, 
// 		double alpha, 
// 		int bands_out, 
// 		const int* bandNumbers, 
// 		double* bandMeans, 
// 		double* bandStdDevs, 
// 		int recalculate, 
// 		float* invalid_value)
// 	{
// 		bool check_invalid;
// 		T invalid_T;
// 		if (invalid_value != NULL){
// 			check_invalid = true;
// 			invalid_T = (T) *invalid_value;
// 		} else {
// 			check_invalid = false;
// 			invalid_T = 0;
// 		}
// 
// 		for (int iBand = 0; iBand < bands_out; iBand++) {
// 
// 			int band = bandNumbers[iBand];
// 			if (band < 0 || band >= bands_in)
// 				continue;
// 
// 			// line mean
// 			if (recalculate)
// 			{
// 				int count = 0;
// 				double line_mean = 0;
// 				for (int iPix = 0; iPix < pixels_in; iPix++) 
// 				{
// 					T val = data[iPix*bands_in+band];
// 					if (!check_invalid || val != invalid_T)
// 					{
// 						line_mean += (double) val;
// 						count++;
// 					}
// 				}
// 
// 				if (count)
// 				{
// 					line_mean /= count;
// 					bandMeans[iBand] = alpha*bandMeans[iBand] + (1.0-alpha) * line_mean;
// 
// 					// line standard deviation
// 					double sq_sum = 0;
// 
// 					for (int iPix = 0; iPix < pixels_in; iPix++)
// 					{
// 						T val = data[iPix*bands_in+band];
// 
// 						if (!check_invalid || val != invalid_T)
// 						{
// 							double v1 = val - line_mean;
// 							sq_sum += v1*v1;
// 						}
// 					}
// 
// 					sq_sum /= count;
// 					double line_stddev = sqrt (sq_sum);
// 					bandStdDevs[iBand] = alpha*bandStdDevs[iBand] + (1.0-alpha) * line_stddev;
// 				}
// 			}
// 
// 			if (output)
// 			{
// 				double minvalue = bandMeans[iBand] - sigma * bandStdDevs[iBand] ;
// 				double maxvalue = bandMeans[iBand] + sigma * bandStdDevs[iBand] ;
// 
// 				// don't divide by zero
// 				if (minvalue == maxvalue) maxvalue = minvalue + 1;
// 
// 				// Scale the image
// 				double c1 = 255.0 / (maxvalue - minvalue);
// 				double c2 = -c1 * minvalue;
// 
// 				for (int iPix = 0; iPix < pixels_in; iPix++)
// 				{
// 					T val = data[iPix*bands_in+band];
// 					if (check_invalid && val == invalid_T) {
// 						output[iPix*bands_out+iBand] = 0;
// 					} else {
// 						double v = (double) (val*c1+c2);
// 						output[iPix*bands_out+iBand] = (unsigned char)((std::min) (255.0, (std::max)(v, 1.0)));
// 					}
// 				}
// 			}
// 		}
// 	}
// 
// 	
//    //
//    // ScaleData_BSQ 
//    template <typename T>
//    void ScaleData_BSQ (
//       T* data, 
//       unsigned char* output, 
//       double sigma, 
//       int pixels_in, 
//       int bands_in, 
//       double alpha, 
//       int bands_out, 
//       const int* bandNumbers, 
//       double* bandMeans, 
//       double* bandStdDevs, 
//       int recalculate, 
//       float* invalid_value)
// 	{
// 		bool check_invalid;
// 		T invalid_T;
// 		if (invalid_value != NULL){
// 			check_invalid = true;
// 			invalid_T = (T) *invalid_value;
// 		} else {
// 			check_invalid = false;
// 			invalid_T = 0;
// 		}
// 
// 		for (int iBand = 0; iBand < bands_out; iBand++) {
// 
// 			int band = bandNumbers[iBand];
// 			if (band < 0 || band >= bands_in)
// 				continue;
// 
// 			// line mean
// 			if (recalculate)
// 			{
// 				int count = 0;
// 				double line_mean = 0;
// 				for (int iPix = 0; iPix < pixels_in; iPix++) 
// 				{
// 					T val = data[band*pixels_in + iPix];
// 					//T val = data[iPix*bands_in+band];
// 					if (!check_invalid || val != invalid_T)
// 					{
// 						line_mean += (double) val;
// 						count++;
// 					}
// 				}
// 
// 				if (count)
// 				{
// 					line_mean /= count;
// 					bandMeans[iBand] = alpha*bandMeans[iBand] + (1.0-alpha) * line_mean;
// 
// 					// line standard deviation
// 					double sq_sum = 0;
// 
// 					for (int iPix = 0; iPix < pixels_in; iPix++)
// 					{
// 						T val = data[band*pixels_in + iPix];
// 
// 						if (!check_invalid || val != invalid_T)
// 						{
// 							double v1 = val - line_mean;
// 							sq_sum += v1*v1;
// 						}
// 					}
// 
// 					sq_sum /= count;
// 					double line_stddev = sqrt (sq_sum);
// 					bandStdDevs[iBand] = alpha*bandStdDevs[iBand] + (1.0-alpha) * line_stddev;
// 				}
// 			}
// 
// 			if (output)
// 			{
// 				double minvalue = bandMeans[iBand] - sigma * bandStdDevs[iBand] ;
// 				double maxvalue = bandMeans[iBand] + sigma * bandStdDevs[iBand] ;
// 
// 				// don't divide by zero
// 				if (minvalue == maxvalue) maxvalue = minvalue + 1;
// 
// 				// Scale the image
// 				double c1 = 255.0 / (maxvalue - minvalue);
// 				double c2 = -c1 * minvalue;
// 
// 				for (int iPix = 0; iPix < pixels_in; iPix++)
// 				{
// 					T val = data[band*pixels_in + iPix];
// 					if (check_invalid && val == invalid_T) {
// 						output[iPix*bands_out+iBand] = 0;
// 						//output[iBand*pixels_in + iPix] = 0;
// 					} else {
// 						double v = (double) (val*c1+c2);
// 						output[iPix*bands_out+iBand] = (unsigned char)(std::min) (255.0, (std::max)(v, 1.0));
// 					}
// 				}
// 			}
// 		}
// 	}
// 
// 	template <typename T>
// 	void ScaleAndPadData_BSQ (
//       T* data, 
//       unsigned char* output, 
//       double sigma, 
//       int pixelsPerLine, 
//       int numberOfLines, 
//       int bands_in, 
//       double alpha, 
//       int bands_out, 
//       const int* bandNumbers, 
//       double* bandMeans, 
//       double* bandStdDevs, 
//       int recalculate, 
//       float* invalid_value, 
//       int bytesToPad)
// 	{
// 		bool check_invalid;
// 		T invalid_T;
// 		if (invalid_value != NULL){
// 			check_invalid = true;
// 			invalid_T = (T) *invalid_value;
// 		} else {
// 			check_invalid = false;
// 			invalid_T = 0;
// 		}
// 
// 		for (int iBand = 0; iBand < bands_out; iBand++) {
// 
// 			int band = bandNumbers[iBand];
// 			if (band < 0 || band >= bands_in)
// 				continue;
// 
// 			// line mean
// 			if (recalculate)
// 			{
// 				int count = 0;
// 				double line_mean = 0;
// 				for (int iPix = 0; iPix < pixelsPerLine * numberOfLines; iPix++) 
// 				{
// 					T val = data[band*pixelsPerLine*numberOfLines + iPix];
// 					if (!check_invalid || val != invalid_T)
// 					{
// 						line_mean += (double) val;
// 						count++;
// 					}
// 				}
// 
// 				if (count)
// 				{
// 					line_mean /= count;
// 					bandMeans[iBand] = alpha*bandMeans[iBand] + (1.0-alpha) * line_mean;
// 
// 					// line standard deviation
// 					double sq_sum = 0;
// 
// 					for (int iPix = 0; iPix < pixelsPerLine * numberOfLines; iPix++)
// 					{
// 						T val = data[band*pixelsPerLine*numberOfLines + iPix];
// 
// 						if (!check_invalid || val != invalid_T)
// 						{
// 							double v1 = val - line_mean;
// 							sq_sum += v1*v1;
// 						}
// 					}
// 
// 					sq_sum /= count;
// 					double line_stddev = sqrt (sq_sum);
// 					bandStdDevs[iBand] = alpha*bandStdDevs[iBand] + (1.0-alpha) * line_stddev;
// 				}
// 			}
// 
// 			if (output)
// 			{
// 				double minvalue = bandMeans[iBand] - sigma * bandStdDevs[iBand] ;
// 				double maxvalue = bandMeans[iBand] + sigma * bandStdDevs[iBand] ;
// 
// 				// don't divide by zero
// 				if (minvalue == maxvalue) maxvalue = minvalue + 1;
// 
// 				// Scale the image
// 				double c1 = 255.0 / (maxvalue - minvalue);
// 				double c2 = -c1 * minvalue;
// 
// 				int outputBytesPerLine = pixelsPerLine * bands_out + bytesToPad;
// 				for( int iLine = 0; iLine < numberOfLines; iLine++ )
// 				{
// 					for( int iPix = 0; iPix < pixelsPerLine; iPix++ )
// 					{
// 						T val = data[(band * pixelsPerLine * numberOfLines) + (pixelsPerLine * iLine) + iPix];
// 						if (check_invalid && val == invalid_T) 
// 						{
// 							output[(outputBytesPerLine * iLine) + (iPix * bands_out) + iBand] = 0;
// 						} 
// 						else 
// 						{
// 							double v = (double) (val*c1+c2);
// 							output[(outputBytesPerLine * iLine) + (iPix * bands_out) + iBand] = (unsigned char)min(255.0, max(v, 1.0));
// 						}
// 					}
// 					//for( int iPad = 0; iPad < bytesToPad; iPad++ )
// 					//{
// 					//	output[((outputBytesPerLine * iLine) + pixelsPerLine + iPad) * bands_out + iBand] = 0;
// 					//}
// 				}
// 			}
// 		}
// 	}
// 
// 	// data type - parameter identifying the type of data representation, where 
// 	// 1 =8 bit byte; 
// 	// 2 =16-bit signed integer; 
// 	// 3 =32-bit signed long integer; 
// 	// 4 =32-bit floating point; 
// 	// 5 =64-bit double precision floating point; 
// 	// 6 =2x32-bit complex, real-imaginary pair of double precision; 
// 	// 9 =2x64-bit double precision complex, real-imaginary pair of double precision; 
// 	// 12=16-bit unsigned integer; 
// 	// 13=32-bit unsigned long integer; 
// 	// 14=64-bit signed long integer; 
// 	// 15=64-bit unsigned long integer.
// 	int Scale_data (
// 		const void* data_in, 
// 		const int dataType, 
// 		unsigned char* output, 
// 		double sigmaFactor, 
// 		int pixels_in, 
// 		int bands_in, 
// 		double alpha, 
// 		int bands_out, 
// 		const int* bandNumbers, 
// 		double* bandMeans, 
// 		double* bandStdDevs, 
// 		int recalculate, 
// 		float* invalid_value)
// 	{
// 
// 		union
// 		{
// 			unsigned char* byteData;
// 			double* doubleData;
// 			float*  floatData;
// 			int*    intData;
// 			unsigned int* uintData;
// 			unsigned short* ushortData;
// 			short*  shortData;
// 			long long* i64Data;
// 			unsigned long long* ui64Data;
// 		};
// 
// 
// 		byteData = (unsigned char*) data_in;
// 
// 	switch (dataType) {
// 		case 1: // ENVI_CHAR:
// 
// 			Scale_data (byteData, output, sigmaFactor, pixels_in, bands_in, alpha, bands_out, bandNumbers, bandMeans, bandStdDevs,recalculate, invalid_value);
// 			break;
// 
// 		case 2: // ENVI_SIGNED_SHORT:
// 			Scale_data (shortData, output, sigmaFactor, pixels_in, bands_in, alpha, bands_out, bandNumbers, bandMeans, bandStdDevs,recalculate, invalid_value);
// 			break;
// 
// 		case 3: // ENVI_INTEGER:
// 			Scale_data (intData, output, sigmaFactor, pixels_in, bands_in, alpha, bands_out, bandNumbers,  bandMeans, bandStdDevs,recalculate, invalid_value);
// 			break;
// 
// 		case 4: // ENVI_FLOAT:
// 			Scale_data (floatData, output, sigmaFactor, pixels_in, bands_in, alpha, bands_out, bandNumbers,  bandMeans, bandStdDevs,recalculate, invalid_value);
// 			break;
// 
// 		case 5: // ENVI_DOUBLE:
// 			Scale_data (doubleData, output, sigmaFactor, pixels_in, bands_in, alpha, bands_out, bandNumbers,  bandMeans, bandStdDevs,recalculate, invalid_value);
// 			break;
// 
// 		case 12: // ENVI_UNSIGNED_SHORT:
// 			Scale_data (ushortData, output, sigmaFactor, pixels_in, bands_in, alpha, bands_out, bandNumbers,  bandMeans, bandStdDevs,recalculate, invalid_value);
// 			break;
// 
// 		case 13: // ENVI_UNSIGNED_INT:
// 			Scale_data (uintData, output, sigmaFactor, pixels_in, bands_in, alpha, bands_out, bandNumbers,  bandMeans, bandStdDevs,recalculate, invalid_value);
// 			break;
// 
// 		case 14: // ENVI_I64:
// 			Scale_data (i64Data, output, sigmaFactor, pixels_in, bands_in, alpha, bands_out, bandNumbers,  bandMeans, bandStdDevs,recalculate, invalid_value);
// 			break;
// 
// 		case 15: // ENVI_UNSIGNED_I64:
// 			Scale_data (ui64Data, output, sigmaFactor, pixels_in, bands_in, alpha, bands_out, bandNumbers,  bandMeans, bandStdDevs,recalculate, invalid_value);
// 			break;
// 
// 			default:
// 				return -1;
// 				break;
// 		}
// 
// 		return 0;
// 	}
// 
// }
// 


// void save_out_mars_files ()
// {
// 
//    using namespace Mars_private;    
// 
//    // color 
//    const float kSigma = 4.0f; 
// 
//    union ptru { 
//       void* v;  float* f;  double* d;  unsigned char* uc;  char* c;  unsigned short* us;  short* s; 
//       }; 
//    
//    std::string kOut_path   = "J:/Quarantine/Mars"; 
//    std::string kBase_fname = "ESP_018065_1975_RED_ESP_019133_1975_RED"; 
// 
// 
//    bool dump_color = false;
//    if (dump_color) 
//    {
//       std::string fname_mars_color  = kOut_path + "/ESP_018065_1975_RED_ESP_019133_1975_RED-DRG.tif"; 
//       //ClassGeoTiff tiff; 
//       CFileReader tiff; 
//       tiff.Open(fname_mars_color.c_str()); 
// 
//       int            num_overviews  = tiff.GetNumOverviews (); 
//       scc::data_type img_data_type  = tiff.GetDataType (); 
//       int            num_bands      = tiff.GetBands() ; 
//       int            src_sz_pxl     = tiff.GetDataTypeSize () ; 
//       int            src_wd         = tiff.GetWidth (); 
//       int            src_ht         = tiff.GetHeight(); 
//       double         dSrc_wd        = src_wd; 
//       double         dSrc_ht        = src_ht;
//       int            num_pxls       = src_wd * src_ht; 
// 
// 
//       std::vector<unsigned char> _img (src_wd * src_ht * src_sz_pxl * num_bands); 
//       ptru ptr = { _img.data()} ; 
// 
//       //std::vector<unsigned char> img_cv (src_wd * src_ht); 
// 
//       char** metadata      = tiff.GetMetaData (); 
// 
//       int      bands[]     = { 0 };
// 
//       tiff.GetView (0, 0, src_wd, src_ht, _img.data(), 0, 0, 1); 
//       //swap_endian_array(img_og.data(), img_og.data(), 4, num_pxls); 
// 
//       double _Eastings[4]  ;
//       double _Northings[4] ;
//       int _zone; 
// 
//       tiff.GetUTM_WGS84 (0          , 0         , _Eastings[0] , _Northings[0], _zone); 
//       tiff.GetUTM_WGS84 (src_wd-1   , 0         , _Eastings[1] , _Northings[1], _zone); 
//       tiff.GetUTM_WGS84 (src_wd-1   , src_ht-1  , _Eastings[2] , _Northings[2], _zone); 
//       tiff.GetUTM_WGS84 (0          , src_ht-1  , _Eastings[3] , _Northings[3], _zone); 
// 
//       double _lats[4] ;
//       double _lons[4] ;
//       tiff.GetLatLongCoordinates (0          , 0         , _lats+0, _lons+0); 
//       tiff.GetLatLongCoordinates (src_wd-1   , 0         , _lats+1, _lons+1); 
//       tiff.GetLatLongCoordinates (src_wd-1   , src_ht-1  , _lats+2, _lons+2); 
//       tiff.GetLatLongCoordinates (0          , src_ht-1  , _lats+3, _lons+3); 
// 
//       bool dump_IGM = true; 
//       if (dump_IGM ) 
//       {
//          std::string             fn = kOut_path + "/" + kBase_fname + "-IGM.dat"; 
//          std::shared_ptr<FILE>   f (fopen(fn.c_str(), "wb"), fclose); 
//          
//          double E_; 
//          double N_; 
//          int dumbzone_; 
//          for (int iH = 0; iH < src_ht; iH++)
//          {
//             for (int iW = 0; iW < src_wd; iW++)
//             {
//                //tiff.GetLatLongCoordinates (iW, iH, &lat_, &lon_); 
//                tiff.GetUTM_WGS84 (iW, iH, E_, N_, dumbzone_); 
//                fwrite (&E_, sizeof(double), 1, f.get()); 
//                fwrite (&N_, sizeof(double), 1, f.get()); 
//             }
//          }
// 
//          f.reset();             
//       }
// 
//       // color 
//       {
//          std::shared_ptr<FILE> f (fopen ("J:/Quarantine/Mars/ESP_018065_1975_RED_ESP_019133_1975_RED-DRG.6900x17177.dat", "wb"), fclose); 
//          fwrite (ptr.v, src_sz_pxl, num_pxls * num_bands, f.get()); 
//          f.reset(); 
//       }
// 
// 
//       tiff.Close (); 
//    }
// 
//    
//    // height
//    bool dump_height = true; 
//    if (dump_height)
//    {
//       std::string fname_marc_height = "J:/Quarantine/Mars/ESP_018065_1975_RED_ESP_019133_1975_RED-DEM.tif"; 
// 
//       CFileReader tiff; 
//       tiff.Open (fname_marc_height.c_str()); 
// 
//       int            num_overviews  = tiff.GetNumOverviews (); 
//       scc::data_type img_data_type  = tiff.GetDataType (); 
//       int            num_bands      = tiff.GetBands() ; 
//       int            src_sz_pxl     = tiff.GetDataTypeSize () ; 
//       int            src_wd         = tiff.GetWidth (); 
//       int            src_ht         = tiff.GetHeight(); 
// 
//       double         dSrc_wd        = static_cast<double> (src_wd); 
//       double         dSrc_ht        = static_cast<double> (src_ht);
//       int            num_pxls       = src_wd * src_ht; 
// 
//       std::shared_ptr<unsigned char>         _img (new unsigned char[src_wd * src_ht * num_bands * src_sz_pxl] ); 
//       ptru ptr = {_img.get() }; 
// 
//       char** metadata = tiff.GetMetaData (); 
// 
//       int      bands[]     = { 1 };
// 
//       tiff.GetView(0, 0, src_wd, src_ht,  _img.get(), 0, 0, 1); 
// 
//       std::shared_ptr<FILE> f (fopen ("J:/Quarantine/Mars/ESP_018065_1975_RED_ESP_019133_1975_RED-DEM.6900x17177.hgt", "wb"), fclose); 
//       fwrite (ptr.v, src_sz_pxl, num_pxls * num_bands, f.get()); 
//       f.reset(); 
//       tiff.Close (); 
//    }
// }
