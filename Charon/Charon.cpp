#include "Charon.h"

void wat()
{
   int i = 0; i++;
}

GLenum GL_TEXTURE_stage(int stage)
{
   BOOST_ASSERT(stage >= 0);
   BOOST_ASSERT(stage < 16);

   return GL_TEXTURE0 + stage;
}

std::string index_2_str (int Y, int X)
{
   std::stringstream oss;
   oss << Y << "_" << X ;
   return oss.str();
}

bool str_2_index (int &X, int& Y, const std::string& s)
{
   std::string::size_type it = s.find ('_');
   if (it != std::string::npos)
   {
      X = boost::lexical_cast<int> (std::string(s.begin(), s.begin() + it));
      Y = boost::lexical_cast<int> (std::string(s.begin() + it + 1, s.end()));
      return true; 
   }
   return false;
}

//
//
size_t Max_elems_in_buff (size_t sizeOf_buf, size_t sizeOf_elem)
{
   DX_ASSERT (sizeOf_elem, "sizeOf_elem is 0"); 

   if (!sizeOf_elem)
      return ~0x0; 

   return (sizeOf_buf / sizeOf_elem); 
} 

void lolwut()
{
   ptru mem { malloc (1024) };

   free (mem.v); 
}

//
void Validate_GL_call()
{
   int wat = 0;
   GLenum         err = 0;
   const GLubyte* error_s_ = 0;

   err = glGetError();
   if (err != GL_NO_ERROR)
   {
      error_s_ = glewGetErrorString(err);
      BOOST_ASSERT(0);
   }


   wat++;
}

//
//
void xp::Update_view_transform (
	Ma::Vec3f&				view_Pos, 
	Ma::Vec3f&				view_Rot, 
	float					   move_Rate,
	const sy::Keyboard_state& kb,    
	const sy::Mouse_state&    ms)
{
   float dYdx = 1.0f; 
   float dXdy = 1.0f; 

   Ma::Vec3f dir_Fwd, dir_Right, dir_Up; 

   Ma::Set (dir_Up, 0.0f, 1.0f, 0.0f); 

   {
   
      Ma::X (view_Rot) += -(ms.yRel* dXdy);
      Ma::Clamp (Ma::X (view_Rot), -60.0f, 60.0f); 

      Ma::Y (view_Rot) += ms.xRel* -dYdx;


      while (Ma::Y (view_Rot) < 0.0)
   	   Ma::Y (view_Rot) += 360.0f; 
   
      while (Ma::Y (view_Rot) > 360.0)
   	   Ma::Y (view_Rot) -= 360.0f; 
   
      const float kDeg2Pi = (float) Ma::Pi / 180.0f; 
      const float fHalfPi = (float) Ma::HalfPi;
   
      Ma::Vec3f v_t;
      // (\ spherical.theta(0).phi(0)) => <1, 0, 0>
      Ma::Spherical (v_t, 0.0f, 0.0f);
      Ma::Spherical (dir_Fwd,    kDeg2Pi * -Ma::Y (view_Rot) - fHalfPi, 0.0f);
      Ma::Spherical (dir_Right,  kDeg2Pi * -Ma::Y (view_Rot), 0.0f); 
      Ma::X (dir_Right);
   }
   
   
   
   // view movement
   {
      Ma::Vec3f v;


      if (sy::Is_keydown (sy::SC_F, kb)) {
   	   X (v) = X (Ma::Scale (v, dir_Fwd, move_Rate));
         Ma::Incr (view_Pos, v);
   	   }
      else 
      if (sy::Is_keydown (sy::SC_V, kb)) {
   	   X (v) = X (Ma::Scale (v, dir_Fwd, -move_Rate));
         Ma::Incr (view_Pos, v);
   	   }
   
      //
      if (sy::Is_keydown (sy::SC_D, kb)) {
   	   Z (v) = 	Z (Ma::Scale (v, dir_Right, -move_Rate)); 
         Ma::Incr (view_Pos, v);
   	   }
      else 
      if (sy::Is_keydown (sy::SC_G, kb)) {
   	   Z (v) = 	Z (Ma::Scale (v, dir_Right, move_Rate)); 
         Ma::Incr (view_Pos, v);
   	   }

      if (sy::Is_keydown (sy::SC_A, kb)) {
   	   Y (v) = 	Y (Ma::Scale (v, dir_Up, move_Rate)); 
         Ma::Incr (view_Pos, v);
   	   }
      else
      if (sy::Is_keydown (sy::SC_Z, kb)) {
   	   Y (v) = 	Y (Ma::Scale (v, dir_Up, -move_Rate)); 
         Ma::Incr (view_Pos, v);
   	   }


   }   

   // view orientation
   //Ma::Set (view_Rot, 1.0f, 0.0f, 0.0f, 0.0f);
   //if (sg_X_mouse > 0 && sg_Y_mouse > 0 && sg_X_mouse < sg_Wd_window && sg_Y_mouse < sg_Ht_window)
   //	Ma::Mult (view_Rot, Ma::RotYAxis (q_Y, view_Y_rot), Ma::RotXAxis (q_X, view_X_rot)); 
   
}


//
// FUNC Update_view_transform 
void Update_view_transform(
   glm::fvec3&				view_Pos,
   glm::fvec3&				view_Rot,
   float                move_Rate,
   const sy::Keyboard_state& kb,
   const sy::Mouse_state&    ms)
{

   const double kDeg2Pi = Ma::Pi / 180.0f;
   const double fHalfPi = Ma::HalfPi;


   static float dYdx = 0.015f;
   static float dXdy = 0.015f;

   // view movement
   view_Rot[0] -= (ms.yRel * dYdx);
   view_Rot[1] -= (ms.xRel * dXdy);
   view_Rot.z = 0.0;


   {
      if (ms.yRel || ms.yRel)
      {
         wat();
      }


      Ma::Vec3d dir_Fwd, dir_Right, dir_Up;
      Ma::Set(dir_Up, 0.0, 1.0, 0.0);

      Ma::Vec3f v_t;
      // (\ spherical.theta(0).phi(0)) => <1, 0, 0>
      Ma::Spherical(v_t, 0.0f, 0.0f);
      Ma::Spherical(dir_Fwd, double(view_Rot[1] - Ma::HalfPi), 0.0);
      Ma::Spherical(dir_Right, double(view_Rot[1]), 0.0);
      Ma::X(dir_Right);
   }

   // view movement
   {
      const glm::fvec3 kRgt(1.0, 0.0, 0.0);
      const glm::fvec3 kFwd(0.0, 0.0, 1.0);

      glm::fvec3 dirFwd = glm::rotateY(kFwd, float(view_Rot[1]));
      glm::fvec3 dirRgt = glm::rotateY(kRgt, float(view_Rot[1]));

      Ma::Vec3d v;

      //
      if (sy::Is_keydown(sy::SC_F, kb))
      {
         view_Pos -= move_Rate * dirFwd;
      }
      else
      if (sy::Is_keydown(sy::SC_V, kb))
      {
         view_Pos += move_Rate * dirFwd;
      }

      //
      if (sy::Is_keydown(sy::SC_D, kb)) {
         view_Pos -= move_Rate * dirRgt;
      }
      else
      if (sy::Is_keydown(sy::SC_G, kb)) {
         view_Pos += move_Rate * dirRgt;
      }

      //
      if (sy::Is_keydown(sy::SC_A, kb)) {
         view_Pos.y += move_Rate;
      }
      else
      if (sy::Is_keydown(sy::SC_Z, kb)) {
         view_Pos.y -= move_Rate;
      }


   }

}






namespace mars_terr
{
   using namespace glm;

   //
   const dvec3 kPatch_verts[] = {
      dvec3(0.0, 0.0, 0.0),
      dvec3(1000.0, 0.0, 0.0),
      dvec3(1000.0, 1000.0, 0.0),
      dvec3(0.0, 1000.0, 0.0),
   };

   const dvec2 kPatch_txcrd[] = {
      dvec2(0.0, 0.0),
      dvec2(1.0, 0.0),
      dvec2(1.0, 1.0),
      dvec2(0.0, 1.0),
   };


   const char*    kFiles[] = {
      MARS_DRIVE "Quarantine/Mars/ESP_018065_1975_RED_ESP_019133_1975_RED-DRG.tif",
      MARS_DRIVE "Quarantine/Mars/ESP_018065_1975_RED_ESP_019133_1975_RED-DEM.tif",
      MARS_DRIVE "Quarantine/Mars/ESP_018065_1975_RED_ESP_019133_1975_RED-IGM.dat"
   };
   const size_t   kNum_files = El_count(kFiles);

   const std::string kType[] = {
      "mars_col_",
      "mars_hgt_",
      "mars_igm_",
   };

   const FREE_IMAGE_FORMAT kFIF_fmt[] = {
      FIF_TIFF,
      FIF_TIFF,
      FIF_UNKNOWN,
   };


   const char     kBase_path[] = MARS_DRIVE "Quarantine/";
   const char     kTile_subp[] = "Mars/tiled/";
   const char     kShader_subp[] = "Hardcore/shader/";

   extern const char kShader_name[] = "mars_terr";

   extern const char* kShader_ext[] = {
      "_vp.glsl",
      "_tc.glsl",
      "_te.glsl",
      "_fp.glsl",
   };

   const GLuint kShader_types[] = {
      GL_VERTEX_SHADER,
      GL_TESS_CONTROL_SHADER,
      GL_TESS_EVALUATION_SHADER,
      GL_FRAGMENT_SHADER,
   };

   const int      kTexture_dim = 1024;
   const int      kNum_X_tiles = (6900 / kTexture_dim) + (6900 % kTexture_dim ? 1 : 0);
   const int      kNum_Y_tiles = (17177 / kTexture_dim) + (17177 % kTexture_dim ? 1 : 0);

   const unsigned kWd = 6900;
   const unsigned kHt = 17177;
}


const char* kFI_color_type_str[] = {
   "FIC_MINISWHITE", // = 0,		//! min value is white
   "FIC_MINISBLACK", // = 1,		//! min value is black
   "FIC_RGB", // = 2,		//! RGB color model
   "FIC_PALETTE", // = 3,		//! color map indexed
   "FIC_RGBALPHA", // = 4,		//! RGB color model with alpha channel
   "FIC_CMYK", // = 5		//! CMYK color model
   };

//
void generate_height_tiles(
   std::string fname, 
   std::string basename, 
   int         in_width, 
   int         in_height,
   std::string outpath,
   int         numxtiles,
   int         numytiles, 
   double      scale) 
{

   int numpixels = in_width * in_height; 

   const int x_tile_dim = in_width / numxtiles;
   const int y_tile_dim = in_height / numytiles;

   std::vector<float> heightmap(in_width * in_height);


   if(FILE* f = fopen(fname.c_str(), "rb"))
   {
      std::shared_ptr<FILE> rs(f, fclose);
      
      std::vector<unsigned char> dat(numpixels);

      fread(dat.data(), sizeof(unsigned char), numpixels, rs.get()); 
      for (int i = 0; i < numpixels; i++)
      {
         heightmap[i] = scale * dat[i];
      }

   }

   heightmap.size();
   const std::string tile_fname = outpath + "/" + basename;

   for (int ity = 0; ity < numytiles; ity++)
   {
      for (int itx = 0; itx < numxtiles; itx++)
      {
         std::string currfname = tile_fname + index_2_str(ity, itx) + "_1f.hgt";
         std::shared_ptr<FILE> ws(fopen(currfname.c_str(), "wb"), fclose);
         for (int ilny = 0; ilny < y_tile_dim; ilny++)
         {
            fwrite(
               &heightmap[in_width * (ity * y_tile_dim + ilny) + (itx * x_tile_dim)],
               sizeof(float),
               x_tile_dim,
               ws.get()
               );
         }

      }
   }


}

void generate_color_tiles(
   std::string fname, 
   std::string basename, 
   int         in_width,
   int         in_height,
   std::string outpath,
   int         numxtiles,
   int         numytiles)
{
   FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(fname.c_str());

//   if (FIBITMAP* bitmap = FreeImage_Load(FIF_PNG, fname.c_str()))
//   {
//      unsigned int            bpp = FreeImage_GetBPP(bitmap);
//      BYTE*                   dat = FreeImage_GetBits(bitmap);
//      FREE_IMAGE_COLOR_TYPE   coltype = FreeImage_GetColorType(bitmap);
//      int                     wd = FreeImage_GetWidth(bitmap);
//      int                     ht = FreeImage_GetHeight(bitmap);
//      unsigned int            colorsused  = FreeImage_GetColorsUsed(bitmap);
//      RGBQUAD*                palette     = FreeImage_GetPalette  (bitmap);
//      int                     haspixels   = FreeImage_HasPixels   (bitmap);
//
//      BITMAPINFO*             bitmapinfo  = FreeImage_GetInfo (bitmap);
//      BITMAPINFOHEADER*       bitmapinfoheader = FreeImage_GetInfoHeader(bitmap);

   if (std::shared_ptr<FILE> rs = std::shared_ptr<FILE> (fopen(fname.c_str(), "rb"), fclose))
   {
      const int numpixels = in_width * in_height; 
      std::vector<glm::u8vec3> colormap (numpixels); 
      
      fread (colormap.data(), sizeof(glm::u8vec3), numpixels, rs.get());

      const int x_tile_dim = in_width  / numxtiles;
      const int y_tile_dim = in_height / numytiles;

      const std::string tile_fname = outpath + "/" + basename;

      for (int ity = 0; ity < numytiles; ity++)
      {
         for (int itx = 0; itx < numxtiles; itx++)
         {
            std::string currfname = tile_fname + index_2_str(ity, itx) + "_3uc.col";
            std::shared_ptr<FILE> ws(fopen(currfname.c_str(), "wb"), fclose);
            for (int ilny = 0; ilny < y_tile_dim; ilny++)
            {
               fwrite(
                  &colormap[in_width * (ity * y_tile_dim + ilny) + (itx * x_tile_dim)],
                  sizeof(glm::u8vec3),
                  x_tile_dim,
                  ws.get()
                  );
            }

         }
      }



   }
   else
   {
      printf("\nERROR: %s failed to open %s\n", __FUNCTIONW__, fname.c_str());

   }
}

template<typename Ty>
std::vector<glm::fvec3>& make_normal_from_height (
   std::vector<glm::fvec3>&   out, 
   std::string                fname,
   float                      x_scale,
   float                      y_scale,
   float                      height_scale,
   int                        in_width,
   int                        in_height)
{
   using namespace glm;
   out.clear();
   int numpixels = in_width * in_height;
   std::vector<Ty> buf(numpixels);
   if (std::shared_ptr<FILE> rs = std::shared_ptr<FILE>(fopen(fname.c_str(), "rb"), fclose))
   {
      fread(buf.data(), sizeof(Ty), numpixels, rs.get());
   }
   else
   {
      return out;
   }

   // set the unit distance between pixels; 
   float pixel_X_step = x_scale;
   float pixel_Y_step = y_scale;

   out.resize(numpixels);

   fvec3 vNrm;
   const int img_X_dim = in_width;
   const int img_Y_dim = in_height;

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

            float fHt = height_scale * buf[iY  * img_X_dim + iX];

            if (iX == 0) // first column
            {
               fvec3 vDn(0.0f, -pixel_Y_step, height_scale * buf[Ynx * img_X_dim + iX] - fHt);
               fvec3 vRt(pixel_X_step, 0.0f,  height_scale * buf[iY * img_X_dim + Xnx] - fHt);

               vNrm = cross(-vRt, vDn);
            }
            else
            if (Xnx == img_X_dim) // last col.
            {
               fvec3 vDn(0.0f, -pixel_Y_step, height_scale * buf[Ynx * img_X_dim + iX] - fHt);
               fvec3 vLt(pixel_X_step, 0.0f,  height_scale * buf[iY * img_X_dim + Xpr] - fHt);

               vNrm = cross(vLt, vDn);
            }
            else
            {
               fvec3 vDn(0.0f, -pixel_Y_step, height_scale * buf[Ynx * img_X_dim + iX] - fHt);
               fvec3 vLt(-pixel_X_step, 0.0f, height_scale * buf[iY * img_X_dim + Xpr] - fHt);
               fvec3 vRt(pixel_X_step, 0.0f,  height_scale * buf[iY * img_X_dim + Xnx] - fHt);

               vNrm = cross(-vDn, vLt) + cross(-vRt, vDn);
            }
            out[iY * in_width + iX] = normalize(vNrm);
         }
      }
      else // last row, no pxls below
      if (Ynx == img_Y_dim)
      {
         for (int iX = 0; iX < img_X_dim; iX++)
         {
            int Xnx = iX + 1;
            int Xpr = iX - 1;

            float fHt = height_scale * buf[iY  * img_X_dim + iX];

            if (iX == 0)
            {
               fvec3 vUp(0.0f, pixel_Y_step, height_scale * buf[Ypr * img_X_dim + iX] - fHt);
               fvec3 vRt(pixel_X_step, 0.0f, height_scale * buf[iY * img_X_dim + Xnx] - fHt);
               vNrm = cross(vUp, vRt);
            }
            else
            if (Xnx == img_X_dim)
            {
               fvec3 vUp(0.0f, pixel_Y_step,  height_scale * buf[Ypr * img_X_dim + iX] - fHt);
               fvec3 vLt(-pixel_X_step, 0.0f, height_scale * buf[iY * img_X_dim + Xpr] - fHt);

               vNrm = cross(vUp, vLt);
            }
            else
            {
               fvec3 vUp(0.0f, pixel_Y_step,  height_scale * buf[Ypr * img_X_dim + iX] - fHt);
               fvec3 vLt(-pixel_X_step, 0.0f, height_scale * buf[iY * img_X_dim + Xpr] - fHt);
               fvec3 vRt(pixel_X_step, 0.0f,  height_scale * buf[iY * img_X_dim + Xnx] - fHt);

               vNrm = cross(vUp, vLt) + cross(vRt, vUp);
            }

            out[iY * in_width + iX] = normalize(vNrm);
         }

      }
      else // inner rows
      {
         for (int iX = 0; iX < img_X_dim; iX++)
         {
            int Xnx = iX + 1;
            int Xpr = iX - 1;

            float fHt = height_scale * buf[iY  * img_X_dim + iX];

            if (iX == 0)
            {
               fvec3 vUp(0.0f, pixel_Y_step,  height_scale * buf[Ypr * img_X_dim + iX] - fHt);
               fvec3 vDn(0.0f, -pixel_Y_step, height_scale * buf[Ynx * img_X_dim + iX] - fHt);
               fvec3 vRt(pixel_X_step, 0.0f,  height_scale * buf[iY * img_X_dim + Xnx] - fHt);

               vNrm = cross(vRt, vUp) + cross(-vRt, vDn);
            }
            else
            if (Xnx == img_X_dim)
            {
               fvec3 vUp(0.0f, pixel_Y_step,  height_scale * buf[Ypr * img_X_dim + iX] - fHt);
               fvec3 vDn(0.0f, -pixel_Y_step, height_scale * buf[Ynx * img_X_dim + iX] - fHt);
               fvec3 vLt(-pixel_X_step, 0.0f, height_scale * buf[iY * img_X_dim + Xpr] - fHt);

               vNrm = cross(vUp, vLt) + cross(-vDn, vLt);
            }
            else
            {
               fvec3 vUp(0.0f, pixel_Y_step,  height_scale * buf[Ypr * img_X_dim + iX] - fHt);
               fvec3 vDn(0.0f, -pixel_Y_step, height_scale * buf[Ynx * img_X_dim + iX] - fHt);
               fvec3 vLt(-pixel_X_step, 0.0f, height_scale * buf[iY * img_X_dim + Xpr] - fHt);
               fvec3 vRt(pixel_X_step, 0.0f,  height_scale * buf[iY * img_X_dim + Xnx] - fHt);

               vNrm = cross(vUp, vLt) + cross(vLt, vDn) +
                  cross(-vRt, vDn) + cross(vRt, vUp);
            }

            out[iY * in_width + iX] = normalize(vNrm);
         }
      }

   }
   return out;
}

void generate_normal_tiles (
   std::string fname, 
   std::string basename, 
   int         in_width,
   int         in_height,
   std::string outpath,
   int         numxtiles,
   int         numytiles)
{
   using namespace glm;

   // make a big normal map
   std::vector<fvec3> normalmap; 
   make_normal_from_height<unsigned char> (
      normalmap,
      fname,
      1.0f, 
      1.0f, 
      100.0f/256.0,
      in_width,
      in_height
      );

   // now write out tiles
   // width x height of a tile
   const int x_tile_dim = in_width / numxtiles;
   const int y_tile_dim = in_height / numytiles;
   
   const std::string tile_fname = outpath + "/" + basename; 
   // todo
   for (int ity = 0; ity < numytiles; ity++)
   {
      for (int itx = 0; itx < numxtiles; itx++)
      {
         std::string currfname = tile_fname + index_2_str(ity, itx) + "_3f.nrm";
         std::shared_ptr<FILE> ws (fopen(currfname.c_str(), "wb"), fclose);
         for (int ilny = 0; ilny < y_tile_dim; ilny++)
         {
            fwrite (
               &normalmap[in_width * (ity * y_tile_dim + ilny) + (itx * x_tile_dim)],
               sizeof(fvec3), 
               x_tile_dim, 
               ws.get()
               );
         }
                  
      }
   }

}



//fname 
//basename 
//outpath 
//xsubiv_factor 
//ysubiv_factor
//FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(fname.c_str ());
//
//if (FIBITMAP* bitmap = FreeImage_Load (FIF_PNG, fname.c_str()))
//{
//   unsigned int            bpp         = FreeImage_GetBPP      (bitmap);
//   BYTE*                   dat         = FreeImage_GetBits     (bitmap); 
//   FREE_IMAGE_COLOR_TYPE   coltype     = FreeImage_GetColorType(bitmap);
//   int                     wd          = FreeImage_GetWidth    (bitmap);
//   int                     ht          = FreeImage_GetHeight   (bitmap);
//   unsigned int            colorsused  = FreeImage_GetColorsUsed(bitmap);
//   RGBQUAD*                palette     = FreeImage_GetPalette  (bitmap);
//   int                     haspixels   = FreeImage_HasPixels   (bitmap);
//   RGBQUAD                 rgbs[256];
//   std::vector<BYTE>       indices(wd); 
//   BITMAPINFO* bitmapinfo = FreeImage_GetInfo (bitmap);
//   BITMAPINFOHEADER* bitmapinfoheader = FreeImage_GetInfoHeader(bitmap);
//
//   std::vector<RGBQUAD>    vpalette(palette, palette + 256);
//   memset(rgbs, 0, 256 * sizeof(RGBQUAD));
//   std::vector<int> imgline(wd);
//   std::vector<int>     pix_res(wd);
//   FREE_IMAGE_TYPE fitype = FreeImage_GetImageType(bitmap);
//   for (int ipx =0; ipx < wd; ipx++)
//   { 
//      imgline[ipx] = dat[ipx];
//      pix_res[ipx] = FreeImage_GetPixelIndex(bitmap, ipx, ipx, &indices[ipx]);
//      //FreeImage_GetPixelColor (bitmap, ipx, ipx, &imgline[ipx]);
//   }      
//   
//   std::shared_ptr<FILE> ws(fopen("C:/Quarantine/awsum/rando/height_main.bin", "wb"), fclose); 
//
//   fwrite(dat, 1, wd * ht, ws.get()); 
//   ws.reset(); 
//   int minval = *std::min_element(indices.begin(), indices.end());
//   int maxval = *std::max_element(indices.begin(), indices.end());
//   
//
//   printf ("\nFI color type: %s, %s\n", __FUNCTIONW__, kFI_color_type_str[coltype]);
//   FreeImage_Unload(bitmap);
//}
//else
//{
//   printf("\nERROR: %s failed to open %s\n", __FUNCTIONW__, fname.c_str());
//}
