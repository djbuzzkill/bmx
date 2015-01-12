

#include "BG_renderer.h"

#include <FileReader.h>

GLenum GL_TEXTURE_stage (int stage)
{
   BOOST_ASSERT (stage >= 0); 
   BOOST_ASSERT (stage < 16); 

   return GL_TEXTURE0 + stage; 
}

// Temporary: for now we will only do this many
const int k_MaxDetailLevelToBuild = 5;


off_t SizeOf_file (const std::string& fname)
{
	if (FILE* f = fopen (fname.c_str(), "rb"))
	{
		fseek (f, 0, SEEK_END);
		long len = ftell (f); 
		fclose (f); 
		return len ; 
	}
	return 0;  

}	


std::string& From_file (std::string& out, const std::string& fname)
{
	size_t sizeOf_file = SizeOf_file (fname) ; 
	out.resize (sizeOf_file); 
	if (FILE* f = fopen (fname.c_str(), "rb"))
	{
		size_t read_Len = fread (&out[0], 1, sizeOf_file, f);
		fclose (f); 
		return out; 
	}
	 
	return out; 
}


// static PIXELFORMATDESCRIPTOR sg_pfd =				// pfd Tells Windows How We Want Things To Be
// {
// 	sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
// 	1,											// Version Number
// 	PFD_DRAW_TO_WINDOW |						// Format Must Support Window
// 	PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
// 	PFD_DOUBLEBUFFER,							// Must Support Double Buffering
// 	PFD_TYPE_RGBA,								// Request An RGBA Format
// 	32,											// Select Our Color Depth
// 	0, 0, 0, 0, 0, 0,							// Color Bits Ignored
// 	0,											// No Alpha Buffer
// 	0,											// Shift Bit Ignored
// 	0,											// No Accumulation Buffer
// 	0, 0, 0, 0,									// Accumulation Bits Ignored
// 	16,											// 16Bit Z-Buffer (Depth Buffer)
// 	1,											// No Stencil Buffer
// 	0,											// No Auxiliary Buffer
// 	PFD_MAIN_PLANE,								// Main Drawing Layer
// 	0,											// Reserved
// 	0, 0, 0										// Layer Masks Ignored
// };



/**
 *
 */
//LRESULT CALLBACK FB_WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	int wmId, wmEvent;
//	PAINTSTRUCT ps;
//	HDC hdc;
//
//
//	switch (message)
//	{
//	// WM_COMMAND
////	case WM_COMMAND:
////		break;
//
//	//case WM_CHAR: 
//	//	wat++;
//	//	break; 
//
//	//case WM_KEYDOWN: 
//	//	sg_keyState[wParam] = true; 
//	//	break;
//	//	
//	//case WM_KEYUP: 
//	//	sg_keyState[wParam] = false; 
//	//	break;
//
//	// WM_MOUSE
//	//case WM_MOUSEMOVE:
//		//sg_X_mouse = LOWORD (lParam); 
//		//sg_Y_mouse = HIWORD (lParam); 
//	//	break; 
//
//	// WM_MOUSELEAVE: 
//	case WM_MOUSELEAVE: 
//		break; 
//
//	// WM_MOUSEWHEEL
//	case WM_MOUSEWHEEL: 
//		break; 
//
//	// WM_SIZE
////	case WM_SIZE: 
////	   break;
//
//	// WM_CREATE
//	case WM_CREATE:
//		//PostQuitMessage(0);
//		break;
//
//	//case WM_PAINT:
//	//	hdc = BeginPaint(hWnd, &ps);
//	//	// TODO: Add any drawing code here...
//	//	EndPaint(hWnd, &ps);
//	//	break;
//
//	// WM_DESTROY
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//
//	// WM_ACTIVATE
//	case WM_ACTIVATE: 
//		//if (listener)
//		//	listener->OnWindowActivate (wParam ? true : false); 
//		break; 
//
//	// WM_CLOSE
//	case WM_CLOSE: 
//		//if (listener) 
//		//	listener->OnWindowClose (); 
//		PostQuitMessage (0); 
//		break; 
//	//
//	// default
//	default:
//		return DefWindowProc (hWnd, message, wParam, lParam);
//	}
//	return 0;
//}

//static TCHAR szWindowClass[64] = "FB_win_class"; 
//static TCHAR szTitle[64] = "FB window"; 

//ATOM Register_def_window_class (HINSTANCE hInstance)
//{
//	WNDCLASSEX wcex;
//
//	wcex.cbSize			   = sizeof(WNDCLASSEX);
//	wcex.style			   = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
//	wcex.lpfnWndProc	   = FB_WndProc;
//	wcex.cbClsExtra		= 0;
//	wcex.cbWndExtra		= 0;
//	wcex.hInstance		   = hInstance;
//	wcex.hIcon			   = NULL;
//	wcex.hCursor		   = LoadCursor(NULL, IDC_ARROW);
//	wcex.hbrBackground	= NULL; // (HBRUSH)(COLOR_WINDOW+1);
//	wcex.lpszMenuName	   = NULL; // MAKEINTRESOURCE(IDC_HORDEINTGRTEST);
//	wcex.lpszClassName	= szWindowClass;
//	wcex.hIconSm		   = NULL;
//	return RegisterClassEx (&wcex);
//}
//
//void Unregister_def_window_class (HINSTANCE hinst)
//{
//   ::UnregisterClass (szWindowClass, hinst); 
//}
//
//HWND Create_window (HINSTANCE hInstance, int nCmdShow)
//{
//	HWND hWnd = CreateWindow (
//		szWindowClass, 
//		szTitle,
//		WS_OVERLAPPEDWINDOW,
//		CW_USEDEFAULT,
//		0,
//		CW_USEDEFAULT,
//		0,
//		NULL, 
//		NULL, 
//		hInstance, 
//		NULL
//		);
//
//	//
//	if (!hWnd)
//	   return 0;
//
//	return hWnd;
//}



// return only works for ortho
// void FrustrumPoints_ortho (
//    glm::dvec3&    far_bott_left  , 
//    glm::dvec3&    far_bott_right ,
//    glm::dvec3&    far_top_right  ,
//    glm::dvec3&    far_top_left   ,
//    glm::dvec3&    near_bott_left ,
//    glm::dvec3&    near_bott_right,
//    glm::dvec3&    near_top_right ,
//    glm::dvec3&    near_top_left  ,
//    const PLANE*   frus           )
// {
//    near_top_left  = glm::dvec3 (-frus[1].d * frus[1].a, -frus[3].d * frus[3].b, -frus[5].d * frus[5].c); 
//    near_top_right = glm::dvec3 (-frus[0].d * frus[0].a, -frus[3].d * frus[3].b, -frus[5].d * frus[5].c); 
//    near_bott_left = glm::dvec3 (-frus[1].d * frus[1].a, -frus[2].d * frus[2].b, -frus[5].d * frus[5].c); 
//    near_bott_right= glm::dvec3 (-frus[0].d * frus[0].a, -frus[2].d * frus[2].b, -frus[5].d * frus[5].c); 
//    far_top_left   = glm::dvec3 (-frus[1].d * frus[1].a, -frus[3].d * frus[3].b, -frus[4].d * frus[4].c); 
//    far_top_right  = glm::dvec3 (-frus[0].d * frus[0].a, -frus[3].d * frus[3].b, -frus[4].d * frus[4].c); 
//    far_bott_left  = glm::dvec3 (-frus[1].d * frus[1].a, -frus[2].d * frus[2].b, -frus[4].d * frus[4].c); 
//    far_bott_right = glm::dvec3 (-frus[0].d * frus[0].a, -frus[2].d * frus[2].b, -frus[4].d * frus[4].c);
// } 

//bool PointInFrustrum (glm::dvec3& pt, const PLANE* frus)
//{
//   if (  (frus[0].PlaneDotCoord (pt.x, pt.y, pt.z) >= 0.0) 
//      && (frus[1].PlaneDotCoord (pt.x, pt.y, pt.z) >= 0.0) 
//      && (frus[2].PlaneDotCoord (pt.x, pt.y, pt.z) >= 0.0) 
//      && (frus[3].PlaneDotCoord (pt.x, pt.y, pt.z) >= 0.0) 
//      && (frus[4].PlaneDotCoord (pt.x, pt.y, pt.z) >= 0.0) 
//      && (frus[5].PlaneDotCoord (pt.x, pt.y, pt.z) >= 0.0) )
//      return true; 
//
//   return false; 
//}
//
//inline bool PointInFrustrum (double x, double y, double z, const PLANE* frus)
//{
//   return PointInFrustrum (glm::dvec3(x, y, z), frus);
//}

//
// Generate_tri_grid_indices - starts bottom left 
template<typename IntTy>
   std::vector<IntTy>& Generate_tri_grid_indices (std::vector<IntTy>& tri_inds, int num_u_tiles, int num_v_tiles) 
{
   int num_tiles = num_u_tiles * num_v_tiles; 
   tri_inds.resize (num_tiles * 2 * 3); 

   int num_x_verts = num_u_tiles + 1; 

   // generate tri indies for grid                   
   for (int iv  = 0; iv  < num_v_tiles ; iv++)
      for (int iu = 0; iu < num_u_tiles; iu++)
   {
      int base_ind = 6 * (iv * num_u_tiles + iu) ; 
      tri_inds[base_ind+0] = (iv * num_x_verts) + iu;       
      tri_inds[base_ind+1] = (iv * num_x_verts) + iu + 1;       
      tri_inds[base_ind+2] = ((1+iv) * num_x_verts) + iu + 1;       
      tri_inds[base_ind+3] = ((1+iv) * num_x_verts) + iu + 1;       
      tri_inds[base_ind+4] = ((1+iv) * num_x_verts) + iu;       
      tri_inds[base_ind+5] = (iv * num_x_verts) + iu;       
   }

   return tri_inds; 
}

template<typename Ty>
   Ty* Generate_tri_grid_indices (Ty* tri_inds, int num_u_tiles, int num_v_tiles) 
{
   int num_tiles = num_u_tiles * num_v_tiles; 

   int num_x_verts = num_u_tiles + 1; 

   // generate tri indies for grid                   
   for (int iv  = 0; iv  < num_v_tiles ; iv++)
      for (int iu = 0; iu < num_u_tiles; iu++)
   {
      int base_ind = 6 * (iv * num_u_tiles + iu) ; 
      tri_inds[base_ind+0] = (iv * num_x_verts) + iu;       
      tri_inds[base_ind+1] = (iv * num_x_verts) + iu + 1;       
      tri_inds[base_ind+2] = ((1+iv) * num_x_verts) + iu + 1;       
      tri_inds[base_ind+3] = ((1+iv) * num_x_verts) + iu + 1;       
      tri_inds[base_ind+4] = ((1+iv) * num_x_verts) + iu;       
      tri_inds[base_ind+5] = (iv * num_x_verts) + iu;       
   }

   return tri_inds; 
}


glm::dvec3* Generate_grid_vert3 (glm::dvec3* verts, int num_x_verts, int num_y_verts, double x_size, double y_size)
{
   double x_step = x_size / (num_x_verts - 1); 
   double y_step = y_size / (num_y_verts - 1); 

   for (int iy = 0; iy < num_y_verts; iy++) 
   {
      for (int ix = 0; ix < num_x_verts; ix++)
      {
      verts[(iy * num_x_verts) + ix].x = x_step * ix; 
      verts[(iy * num_x_verts) + ix].y = y_step * iy; 
      }
   }

   return verts; 
}

glm::dvec2* Generate_grid_vert2 (glm::dvec2* verts, int num_x_verts, int num_y_verts, double x_size, double y_size)
{
   double x_step = x_size / (num_x_verts - 1); 
   double y_step = y_size / (num_y_verts - 1); 

   for (int iy = 0; iy < num_y_verts; iy++) 
   {
      for (int ix = 0; ix < num_x_verts; ix++)
      {
      verts[(iy * num_x_verts) + ix].x = x_step * ix; 
      verts[(iy * num_x_verts) + ix].y = y_step * iy; 
      }
   }

   return verts; 
}

// 
// build tile background tile textures 
struct GL_TileAccumulator : public TileAccumulator
{
   union vec_u
   {
      void*          v; 
      unsigned char* u8;
      glm::u8vec2*   v2; 
      glm::u8vec3*   v3; 
      glm::u8vec4*   v4;
      unsigned int*  u32;
   }; 

   // 
   int         initialized ; 

#if 0
   HDC         hdc; 
   HWND        hwnd; 
   HGLRC       hglrc; 
   HINSTANCE   app_inst; 

   HDC         prev_DC; 
   HGLRC       prev_HGLRC; 
   std::shared_ptr<GLEWContext> m_glew_context; 

   scc::CDem& const dem; 

#endif

   // the frame buffer to draw in
   GLuint         fb_ID;
   GLuint         txr_IDs[2]; 
   BG_tile::Map*  zone_map_N; 
   BG_tile::Map*  zone_map_S; 

   int zone_used_N[60]; 
   int zone_used_S[60]; 
   



   static std::map<BG_tile::DataType, GLenum> GL_type_map;
   static GLenum kFmt_internal[3]; 
   static GLenum kFmt_source[3]; 

   // 
   // GL_TileAccumulator (scc::CDem& d) 
   //    : initialized (0)
   //    , dem (d)
   //    , hdc(0)
   //    , hwnd(0)
   //    , hglrc (0)
   //    , app_inst (0)
   //    , prev_DC (0)
   //    , prev_HGLRC(0)
   //    , zone_map_N (0)
   //    , zone_map_S (0)
   //    , m_glew_context ()
   // {
   //    Initialize ();  
   // }

   //          
   virtual ~GL_TileAccumulator () {
   }

#if defined (GLEW_MX)
   // defined for glew mx
   GLEWContext* glewGetContext ()
   {
      if (!m_glew_context.get())
         m_glew_context.reset (new GLEWContext); 

      return m_glew_context.get(); 
   }
#endif

   //
   //
   int Initialize () 
   {
      if (initialized)
         return 0; 
         
		GL_type_map[BG_tile::DT_uint8]    = GL_UNSIGNED_BYTE; 
		GL_type_map[BG_tile::DT_int16]   = GL_SHORT;
		GL_type_map[BG_tile::DT_uint16]  = GL_UNSIGNED_SHORT;
		GL_type_map[BG_tile::DT_int32]   = GL_INT;
		GL_type_map[BG_tile::DT_uint32]  = GL_UNSIGNED_INT; 
		GL_type_map[BG_tile::DT_real32]  = GL_FLOAT;
		GL_type_map[BG_tile::DT_real64]  = GL_DOUBLE; 

      fb_ID = 0; 

      txr_IDs[0] = 0; 
      txr_IDs[1] = 0; 

      initialized = 1; 
      return 0; 
   }; 


#if 0
   // Initialize_accumulator_context <- rename to this
   int Initialize_accumulator_context (HINSTANCE hInst, BG_tile::Map* zones_N, BG_tile::Map* zones_S)
   {
      DWORD error_ = 0;

      app_inst = hInst; 
	   BOOST_ASSERT (app_inst);

	   ATOM	atom = Register_def_window_class (hInst);
	   BOOST_ASSERT (atom);

	   HWND	hwnd = Create_window (hInst, SW_SHOW); 
	   BOOST_ASSERT (hwnd);

      this->hwnd  = hwnd; 
      this->hdc   = ::GetDC(hwnd); 
	   BOOST_ASSERT (this->hwnd);
	   BOOST_ASSERT (this->hdc);

      BOOST_ASSERT (zones_N); 
      BOOST_ASSERT (zones_S); 

      zone_map_N = zones_N;
      zone_map_S = zones_S;
      
      int iPixelFormat = ::ChoosePixelFormat (this->hdc, &sg_pfd); 
      ::SetPixelFormat (this->hdc, iPixelFormat , &sg_pfd); 

      this->hglrc = wglCreateContext (this->hdc); 

      if (this->hglrc == 0)
         {
      error_ = GetLastError (); 
      return -1; 
         }
      else
         {
         BOOL res_ = ::wglMakeCurrent (this->hdc, this->hglrc); 
         }

      // does this need to be called?
      GLenum init_res = glewInit(); 
      
      GLboolean frambuffer_supported = glewIsSupported ("GL_ARB_framebuffer_object"); 

      GLenum GL_error_        = GL_NO_ERROR ; 
      const GLubyte* error_s  = 0;

      if (!txr_IDs[0] && !txr_IDs[1])
      { 
         glGenTextures (2, txr_IDs); 
         GL_error_   = glGetError (); 

         if (GL_error_ != GL_NO_ERROR )
            {
         error_s  = glewGetErrorString (GL_error_);
            }
      }
      
      if (!fb_ID)
      {
         glGenFramebuffers(1, &fb_ID); 
         GL_error_   = glGetError (); 
         if (GL_error_ != GL_NO_ERROR )
            {
         error_s  = glewGetErrorString (GL_error_);
            }
      }


      return 0;
   }
    
   // Deinitialize_accumulator_context 
   int Deinitialize_accumulator_context ()
   {
      ::wglDeleteContext (this->hglrc); 
      this->hdc    = 0; 
      this->hglrc = 0; 
      Unregister_def_window_class (app_inst); 

      zone_map_N = 0; 
      zone_map_S = 0; 
      // ::wglMakeCurrent (this->prev_DC, this->prev_HGLRC); 
      return 0;
   }

#endif

   //
   //
   int Accumulate_image (double& center_easting, double& center_northing, int& zone, const std::string& fname)
   {
      GLenum         error_      = GL_NO_ERROR ; 
      const GLubyte* error_s_    = 0; 

      BOOST_ASSERT (zone_map_N); 
      BOOST_ASSERT (zone_map_S);  

      error_   = glGetError (); 
      if (error_ != GL_NO_ERROR )
         error_s_  = glewGetErrorString (error_ );

      const int grid_x_dim = 8; 
      const int grid_y_dim = 8; 
      const int num_x_verts = grid_x_dim + 1; 
      const int num_y_verts = grid_y_dim + 1; 

      // should be created during context initialization
      GLuint dst_ID = txr_IDs[0]; 
      GLuint src_ID = txr_IDs[1]; 

      // geotiff
      CFileReader geotiff; 
      geotiff.Open (fname.c_str()); 
// static bool DBG_inspect_tiff_metadata_ = false; 
//       if (DBG_inspect_tiff_metadata_) 
//       { 
// char** metadata_ = geotiff.GetMetaData (); 
// std::vector<std::string> meta_data; 
// int meatadata_count_ = 0; 
// while (metadata_[meatadata_count_++])
//    meta_data.push_back (metadata_[meatadata_count_]); 
// meta_data.size (); 
// 
//       }

      // tiff SampleFormat
      // 1 = unsigned integer data
      // 2 = two's complement signed integer data
      // 3 = IEEE floating point data
      // 4 = undefined data format
      std::map<int, BG_tile::DataType> TIF_2_BG_type;  
      {
         TIF_2_BG_type[1] = BG_tile::DT_uint16; 
         TIF_2_BG_type[2] = BG_tile::DT_int16 ; 
         TIF_2_BG_type[3] = BG_tile::DT_real32 ; 
      }


      BG_tile::DataType img_data_type  = TIF_2_BG_type[ geotiff.GetDataType () ]; 
      int            num_bands      = geotiff.GetBands() ; 
      int            src_sz_pxl     = geotiff.GetDataTypeSize () ; 
      int            src_wd         = geotiff.GetWidth (); 
      int            src_ht         = geotiff.GetHeight(); 
      double         dsrc_wd        = src_wd; 
      double         dsrc_ht        = src_ht;

      geotiff.GetUTM_WGS84 (src_wd / 2, src_ht / 2, center_easting, center_northing, zone);
              
      BOOST_ASSERT (img_data_type == scc::data_type::id::byte); 
      // for now
      int wd_pxl_step = src_wd / grid_x_dim; 
      int ht_pxl_step = src_ht / grid_y_dim; 

      int            zone_;
      // geotiff coords start from top
      // we store from bottom left 
      glm::dvec2 EN_corners[4];  
      glm::dvec2 min_EN, max_EN; 

      geotiff.GetUTM_WGS84 (0,            src_ht - 1,   EN_corners[0].x, EN_corners[0].y, zone_);
      geotiff.GetUTM_WGS84 (0,            0,            EN_corners[1].x, EN_corners[1].y, zone_);
      geotiff.GetUTM_WGS84 (src_wd - 1,   0,            EN_corners[2].x, EN_corners[2].y, zone_);
      geotiff.GetUTM_WGS84 (src_wd - 1,   src_ht - 1,   EN_corners[3].x, EN_corners[3].y, zone_);

      min_EN.x = EN_corners[0].x;
      max_EN.x = EN_corners[0].x;   
      min_EN.y = EN_corners[0].y;
      max_EN.y = EN_corners[0].y;   

      glm::dvec2 img_U = EN_corners[3] - EN_corners[0]; 
      glm::dvec2 img_V = EN_corners[1] - EN_corners[0]; 
      // not quite accurate this takes the axis aligned box and not true UV directions of the image dims
      double vU_len = glm::length (img_U); 
      double vV_len = glm::length (img_V); 

      double dGSD_U = vU_len / src_wd; 
      double dGSD_V = vV_len / src_ht; 

      // textures have to be multiples of 4? 
      int align4_wd = (src_wd % 4) ? (src_wd / 4 + 1) * 4 : src_wd;
      int align4_ht = (src_ht % 4) ? (src_ht / 4 + 1) * 4 : src_ht; 

      for (int i = 1; i < 4; i++) 
      {
         min_EN.x = std::min (EN_corners[i].x, min_EN.x);
         max_EN.x = std::max (EN_corners[i].x, max_EN.x);
         min_EN.y = std::min (EN_corners[i].y, min_EN.y);
         max_EN.y = std::max (EN_corners[i].y, max_EN.y);
      }

      // generate tri indies and geometry for grid
      std::vector<unsigned short>   tri_inds; 
      std::vector<glm::dvec3>       verts_ (num_x_verts * num_y_verts); 
      std::vector<glm::dvec2>       txcds_ (num_x_verts * num_y_verts); 
      // std::vector<glm::dvec3> verts_ (new glm::dvec3[num_x_verts * num_y_verts]); 
      // std::vector<glm::dvec2> txcds_ (new glm::dvec2[num_x_verts * num_y_verts]); 

      Generate_tri_grid_indices (tri_inds, grid_x_dim, grid_y_dim); 

      // prepare texture for accumulation
      // tex coords are 0-1
      // verts are taken for lat/lons
      glm::dvec3* verts = verts_.data();
      glm::dvec2* txcds = txcds_.data();

      // gl textures seem to like dimensions in multiples of 4.  the sampling area may not
      // stretch to the end of the texture, so the full sampling range is...
      double txcd_ratio_wd = static_cast<double> (src_wd) / static_cast<double>(align4_wd); 
      double txcd_ratio_ht = static_cast<double> (src_ht) / static_cast<double>(align4_ht); 

      for (int iy = 0; iy <= grid_y_dim; iy++) 
      {
         for (int ix =0; ix <= grid_x_dim; ix++) 
         {
            int ind_lin = num_x_verts * (grid_y_dim - iy) + ix; 

            int x_pxl = (ix * wd_pxl_step);  
            int y_pxl = (grid_y_dim - iy) * ht_pxl_step;  

            verts[ind_lin].z = -1.0; 

            if (iy == grid_y_dim)
            {
               if (ix == grid_x_dim)
               {
                  geotiff.GetUTM_WGS84 (src_wd - 1, 0, verts[ind_lin].x, verts[ind_lin].y, zone_);
                  txcds[ind_lin].x = txcd_ratio_wd;
                  txcds[ind_lin].y = txcd_ratio_ht; 
               }
               else
               {
                  geotiff.GetUTM_WGS84 (x_pxl, 0, verts[ind_lin].x, verts[ind_lin].y, zone_);
                  txcds[ind_lin].x = txcd_ratio_wd * double (ix * wd_pxl_step) /  dsrc_wd;   
                  txcds[ind_lin].y = txcd_ratio_ht; 
               }
            }
            else
            {
               if (ix == grid_x_dim)
               {
                  geotiff.GetUTM_WGS84 (src_wd - 1, y_pxl, verts[ind_lin].x, verts[ind_lin].y, zone_);
                  txcds[ind_lin].x = txcd_ratio_wd; 
                  txcds[ind_lin].y = txcd_ratio_ht * double (iy * ht_pxl_step) / dsrc_ht; 
               }
               else
               {
                  geotiff.GetUTM_WGS84 (x_pxl , y_pxl, verts[ind_lin].x, verts[ind_lin].y, zone_);
                  txcds[ind_lin].x = txcd_ratio_wd * double (ix * wd_pxl_step) / dsrc_wd; 
                  txcds[ind_lin].y = txcd_ratio_ht * double (iy * ht_pxl_step) / dsrc_ht; 
               }
            }

         }
      }
      
      BOOST_ASSERT (GL_type_map.count (img_data_type)); 

      glBindFramebuffer (GL_FRAMEBUFFER, fb_ID); 
      error_  = glGetError ();
      if (error_ != GL_NO_ERROR )
         error_s_  = glewGetErrorString (error_ );

      glFrontFace (GL_CCW); 
      glCullFace  (GL_BACK);
      error_  = glGetError ();
      if (error_ != GL_NO_ERROR )
         error_s_  = glewGetErrorString (error_ );

      glDisable (GL_CULL_FACE); 
      error_  = glGetError ();
      if (error_ != GL_NO_ERROR )
         error_s_  = glewGetErrorString (error_ );

      // glEnableClientState - input geom
      glEnableClientState  (GL_VERTEX_ARRAY); 
      glEnableClientState  (GL_TEXTURE_COORD_ARRAY); 

      glVertexPointer      (3, GL_DOUBLE, 0, verts);  
      glTexCoordPointer    (2, GL_DOUBLE, 0, txcds); 
      error_  = glGetError ();
      if (error_ != GL_NO_ERROR )
         error_s_  = glewGetErrorString (error_ );

      GLboolean prev_depth_enabled  ; 
      GLboolean prev_stencil_enabled; 
      GLboolean prev_blend_enabled  ; 

      glGetBooleanv (GL_DEPTH_TEST  , &prev_depth_enabled); 
      glGetBooleanv (GL_STENCIL_TEST, &prev_stencil_enabled); 
      glGetBooleanv (GL_BLEND       , &prev_blend_enabled  ); 

      glDisable (GL_DEPTH_TEST); 
      glDisable (GL_STENCIL_TEST); 
      glDisable (GL_BLEND); 

      // enable texturing 
      glEnable (GL_TEXTURE_2D);       

      // set our tiff to tex stage 0
      glActiveTexture (GL_TEXTURE0);

      glBindTexture  (GL_TEXTURE_2D, src_ID);
      error_   = glGetError (); 
      if (error_ != GL_NO_ERROR)
         error_s_  = glewGetErrorString (error_ );
      
      const unsigned char k_Default_pixel_value = 0x0; 

      // source data is from geotiff
      std::vector<unsigned char> src_bytes (src_wd * src_ht * num_bands * src_sz_pxl);
      geotiff.GetView (0, 0, geotiff.GetWidth () , geotiff.GetHeight (),  src_bytes.data()); 
      //geotiff.GetView (0, iy, geotiff.GetWidth () , 1,  src_bytes.data() + line_offs_); 

      std::vector<unsigned char> aligned_bytes (align4_wd * align4_ht * num_bands * src_sz_pxl, k_Default_pixel_value); 
      for (int iy = 0; iy < src_ht; iy++) 
      {
         int src_size_line = src_wd* num_bands * src_sz_pxl;
         int src_line_offs = (src_ht - iy - 1) * src_size_line; 

         int align4_size_line = align4_wd * num_bands * src_sz_pxl; 
         int align4_line_offs = iy * align4_size_line; 
         std::copy (src_bytes.begin() + src_line_offs , src_bytes.begin() + src_line_offs + src_size_line, aligned_bytes.begin () + align4_line_offs); 
      }
      
      glBindTexture (GL_TEXTURE_2D, src_ID);
      //glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, align4_wd, align4_ht, 0, kFmt_source[num_bands-1], GL_UNSIGNED_BYTE, src_bytes.data()); 
      glTexImage2D (GL_TEXTURE_2D, 0, kFmt_internal[num_bands-1], align4_wd, align4_ht, 0, kFmt_source[num_bands-1], GL_UNSIGNED_BYTE, aligned_bytes.data()); 
                                  
      error_   = glGetError (); 
      if (error_ != GL_NO_ERROR)
         error_s_  = glewGetErrorString (error_ );

// DBGDBGDBGDBGDBGDBGDBG
// static bool DBG_read_src_ = false; 
//       if (DBG_read_src_) 
//       {
// std::vector<unsigned char> read_src_ (4 * src_wd * src_ht, 0xA); 
// vec_u returned_dat_ = { read_src_.data() } ; 
// glGetTexImage (GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &read_src_[0]); 
//       }

      // build image tiles at each detail level 
      // 0 1 meter
      // 1 2 meters
      // 2 4 meters
      // 3 8 ..
      // 4 16 ...
      for (int iLevel = 0; iLevel < k_MaxDetailLevelToBuild; iLevel++) 
      { 
         // do not support sub meter detail yet
         BOOST_ASSERT (iLevel >= 0); 

         int level_ind = BG_tile::Level_index (iLevel); 

         int      start_Easting     = static_cast<int> (min_EN.x * 0.001); 
         int      stop_Easting      = static_cast<int> (max_EN.x * 0.001); 
         int      start_Northing    = static_cast<int> (min_EN.y * 0.001); 
         int      stop_Northing     = static_cast<int> (max_EN.y * 0.001); 

         int      level_Base1       = iLevel + 1; 
         int      iKilometer_step   = 0x1 << iLevel; 
         double   dKilometer_step   = static_cast<double> (iKilometer_step); 

         // ...every tile_step_dist kilometers
         start_Easting /= iKilometer_step; 
         start_Easting *= iKilometer_step; 
            
         stop_Easting /= iKilometer_step;         
         stop_Easting *= iKilometer_step;

         start_Northing /= iKilometer_step;
         start_Northing *= iKilometer_step;

         stop_Northing /= iKilometer_step;
         stop_Northing *= iKilometer_step;

         // std::exp (2, iLevel); 
         // 
         BG_tile::Map& curr_zone = zone_map_N[zone_]; 

         glColor4f (1.0f, 1.0f, 1.0f, 1.0f); 
// std::vector<int>                 DBG_numbands; 
// std::vector<glm::ivec2>          DBG_tilecoord; 
// std::vector<scc::data_type::id>  DBG_datatype; 
         for (int iN = start_Northing; iN <= stop_Northing; iN += iKilometer_step)
         {
            for (int iE = start_Easting; iE <= stop_Easting; iE += iKilometer_step) 
            {
               BG_tile::Struc&   curr_tile = curr_zone[glm::ivec2 (iE, iN)]; 

               // maybe do any setup if new 
               if (curr_tile.tile_level[level_ind].type == scc::data_type::none)
               {
                  // for inverse meter size, we increase texture size, not implemented yet
                  int num_tile_pxls = BG_tile::kTextureWidth * BG_tile::kTextureHeight; 

                  // just work with 1 level for now
                  curr_tile.tile_level[level_ind].type        = img_data_type; 
                  curr_tile.tile_level[level_ind].num_bands   = num_bands; 
                  curr_tile.tile_level[level_ind].color        = new unsigned char[3 * num_tile_pxls];
               
                  vec_u cols = { curr_tile.tile_level[level_ind].color };
                  std::fill (cols.v3, cols.v3 + num_tile_pxls, glm::u8vec3(128, 128, 128));

// DBG_numbands   .push_back(num_bands); 
// DBG_tilecoord  .push_back(glm::ivec2 (iE, iN)); 
// DBG_datatype   .push_back(img_data_type);  
               }

               // bind the render target (dst)            

//vec_u cols = { curr_tile.tile_level[0].dat }; 

               // dst_ID is the render target 
               glBindTexture (GL_TEXTURE_2D, dst_ID); 
               glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, BG_tile::kTextureWidth, BG_tile::kTextureHeight, 
                              0, GL_RGB, GL_UNSIGNED_BYTE, curr_tile.tile_level[level_ind].color); 
               error_  = glGetError ();
               if (error_ != GL_NO_ERROR)
                  error_s_  = glewGetErrorString (error_ );

               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
               error_  = glGetError ();
               if (error_ != GL_NO_ERROR)
                  error_s_  = glewGetErrorString (error_ );

               // glBindFramebuffer 
               glBindFramebuffer (GL_FRAMEBUFFER, fb_ID); 
               error_  = glGetError ();
               if (error_ != GL_NO_ERROR )
                  error_s_  = glewGetErrorString (error_ );

               // glFramebufferTexture2D 
               glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dst_ID, 0); 
               error_  = glGetError ();
               if (error_ != GL_NO_ERROR )
                  error_s_  = glewGetErrorString (error_ );

               // glActiveTexture (GL_TEXTURE0);
               // glBindTexture  (GL_TEXTURE_2D, src_ID);
               // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
               // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
               // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
               // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
               // 
               // error_   = glGetError (); 
               // if (error_ != GL_NO_ERROR)
               //    error_s_  = glewGetErrorString (error_ );

               // glDrawBuffers
               GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
               glDrawBuffers (1, drawBuffers);  
               error_  = glGetError ();
               if (error_ != GL_NO_ERROR )
                  error_s_  = glewGetErrorString (error_ );

               // set source texture (tiff)               
               glActiveTexture (GL_TEXTURE0); 
               glBindTexture (GL_TEXTURE_2D, src_ID); 
               error_  = glGetError ();
               if (error_ != GL_NO_ERROR )
                  error_s_  = glewGetErrorString (error_ );
               // sampling method
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
               error_  = glGetError ();
               if (error_ != GL_NO_ERROR )
                  error_s_  = glewGetErrorString (error_ );

               // glTexEnvi - application method
               glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); // GL_REPLACE
               error_  = glGetError ();
               if (error_ != GL_NO_ERROR )
                  error_s_  = glewGetErrorString (error_ );

               // glViewport 
               glViewport (0, 0, BG_tile::kTextureWidth, BG_tile::kTextureHeight); 

               // glMatrixMode 
               glMatrixMode (GL_MODELVIEW); 
               glLoadIdentity (); 

               glMatrixMode (GL_PROJECTION); 
               glm::dmat4 ortho_ = glm::ortho (
                  // left, right
                  iE * 1000.0, (iE + iKilometer_step) * 1000.0, 
                  // bottom top                     
                  iN * 1000.0, (iN + iKilometer_step) * 1000.0
                  ); 

               glLoadMatrixd (glm::value_ptr (ortho_)); 
               error_  = glGetError ();
               if (error_ != GL_NO_ERROR )
                  error_s_  = glewGetErrorString (error_ );
            
               // glDrawElements 
               glDrawElements (GL_TRIANGLES, tri_inds.size (), GL_UNSIGNED_SHORT , &tri_inds[0]); 
               error_  = glGetError ();
               if (error_ != GL_NO_ERROR )
                  error_s_  = glewGetErrorString (error_ );

               glFlush (); 

               glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0); 
               error_  = glGetError ();
               if (error_ != GL_NO_ERROR )
                  error_s_  = glewGetErrorString (error_ );

               glBindTexture (GL_TEXTURE_2D, dst_ID); 
               error_  = glGetError ();
               if (error_ != GL_NO_ERROR )
                  error_s_  = glewGetErrorString (error_ );

               // write it back out to store 
               glGetTexImage (GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, curr_tile.tile_level[level_ind].color); 

               error_  = glGetError ();
               if (error_ != GL_NO_ERROR )
                  error_s_  = glewGetErrorString (error_ );

               // we start at 32x32 at level 0 because that is the approx res of the DEM
               // lvl 0 => 32x32
               // lvl 1 => 16
               // lvl 2 => 8
               // lvl 3 => 4
               // lvl 4 => .. we may not use a height map anymore past this point
               // lvl 5 => 
               // lvl 6 => 
               // lvl 7 => 
               // lvl 8 => 


               short*      height_dat     = new short[BG_tile::kHeightMap_dim * BG_tile::kHeightMap_dim] ; 
               double      dHeight_step   = (dKilometer_step * 1000) / (BG_tile::kHeightMap_dim - 1); 

               double tile_E_start = iE * 1000.0; 
               double tile_N_start = iN * 1000.0; 

               for (int iy = 0; iy < BG_tile::kHeightMap_dim; iy++)
               {   
                  for (int ix = 0; ix < BG_tile::kHeightMap_dim; ix++) 
                  {
                     double dem_val = dem.GetElevation (tile_E_start + ix * dHeight_step, tile_N_start + iy * dHeight_step, zone); 
                     height_dat[BG_tile::kHeightMap_dim * iy + ix] = dem_val; 
                     //height_dat[BG_tile::kHeightMap_dim * (BG_tile::kHeightMap_dim - iy - 1) + ix] = dem_val; 
                  }
               }
               curr_tile.tile_level[level_ind].height = height_dat; 

// static bool DBG_tile_inspect_ = true; 
//             if (DBG_tile_inspect_) 
//             {
// std::vector<unsigned char> DBG_out (3 * BG_tile::kTextureWidth * BG_tile::kTextureHeight); 
// glGetTexImage (GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, DBG_out.data ()); 
// DBG_out.size (); 
//             }

//static bool DBG_dump_tile_ = false;
//            if (DBG_dump_tile_) 
//            { 
//std::vector<unsigned char> DBG_out (3 * BG_tile::kTextureWidth * BG_tile::kTextureHeight); 
//glGetTexImage (GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, DBG_out.data ()); 
//std::ostringstream oss; 
//oss << "C:/usr/accum_E" << iE << "_N" << iN << "_z" << zone << ".dat"; 
//std::shared_ptr<FILE> fout (fopen (oss.str ().c_str (), "wb"), fclose); 
//fwrite (DBG_out.data (), 1, 3 * BG_tile::kTextureWidth * BG_tile::kTextureHeight, fout.get()); 
//fout.reset(); 
//            }

            }
         } 
      } // for detail level

      if (prev_depth_enabled == GL_TRUE)
         glEnable (GL_DEPTH_TEST); 
      if (prev_stencil_enabled == GL_TRUE)
         glEnable (GL_STENCIL_TEST); 

      // 0 errors 
      return 0; 
   } 


    // GLboolean prev_clip_planes[6]; 
    // glGetBooleanv (GL_CLIP_PLANE0, prev_clip_planes + 0); 
    // glGetBooleanv (GL_CLIP_PLANE1, prev_clip_planes + 1); 
    // glGetBooleanv (GL_CLIP_PLANE2, prev_clip_planes + 2); 
    // glGetBooleanv (GL_CLIP_PLANE3, prev_clip_planes + 3); 
    // glGetBooleanv (GL_CLIP_PLANE4, prev_clip_planes + 4); 
    // glGetBooleanv (GL_CLIP_PLANE5, prev_clip_planes + 5); 
    // 
    // glDisable (GL_CLIP_PLANE0); 
    // glDisable (GL_CLIP_PLANE1); 
    // glDisable (GL_CLIP_PLANE2); 
    // glDisable (GL_CLIP_PLANE3); 
    // glDisable (GL_CLIP_PLANE4); 
    // glDisable (GL_CLIP_PLANE5); 


    //  GLint pack_align_ = 69; 
    //  GLint pack_row_len_ = 69; 
    //  GLint pack_skip_pxls_ = 69; 
    //  GLint pack_skip_rows_ = 69; 
    //  glGetIntegerv (GL_PACK_ALIGNMENT, &pack_align_ );
    //  glGetIntegerv (GL_PACK_ROW_LENGTH, &pack_row_len_);

    //  glGetIntegerv (GL_PACK_SKIP_PIXELS, &pack_skip_pxls_);
    //  glGetIntegerv (GL_PACK_SKIP_ROWS, &pack_skip_rows_);

    //  

    //  GLint unpack_align_ = 69; 
    //  GLint unpack_row_len_ = 69; 
    //  GLint unpack_skip_pxls_ = 69; 
    //  GLint unpack_skip_rows_ = 69; 

    //  glGetIntegerv (GL_UNPACK_ALIGNMENT  , &unpack_align_ );
    //  glGetIntegerv (GL_UNPACK_ROW_LENGTH , &unpack_row_len_);
    //  glGetIntegerv (GL_UNPACK_SKIP_PIXELS  , &unpack_skip_pxls_ );
    //  glGetIntegerv (GL_UNPACK_SKIP_ROWS    , &unpack_skip_rows_);

	   //glPixelStorei (GL_PACK_ALIGNMENT, 1);
	   //glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
    //  error_   = glGetError (); 
    //  if (error_ != GL_NO_ERROR)
    //     error_s_  = glewGetErrorString (error_ );



    //  GLfloat red_bias, red_scale, green_bias, green_scale, blue_bias, blue_scale, alpha_scale, alpha_bias;
    //  glGetFloatv(GL_RED_SCALE, &red_scale); 
    //  glGetFloatv(GL_RED_BIAS , &red_bias); 

    //  glGetFloatv(GL_GREEN_SCALE, &green_scale); 
    //  glGetFloatv(GL_GREEN_BIAS , &green_bias); 

    //  glGetFloatv(GL_BLUE_SCALE, &blue_scale); 
    //  glGetFloatv(GL_BLUE_BIAS , &blue_bias); 

    //  glGetFloatv(GL_ALPHA_SCALE, &alpha_scale); 
    //  glGetFloatv(GL_ALPHA_BIAS , &alpha_bias); 
}; 
   
GLenum GL_TileAccumulator::kFmt_internal[3] = {
   GL_RED,  GL_RG,  GL_RGB,  
   }; 
      
GLenum GL_TileAccumulator::kFmt_source[3] = { 
   GL_RED, GL_RG, GL_RGB
   }; 

    
std::map<BG_tile::DataType, GLenum> GL_TileAccumulator::GL_type_map; 

//TileAccumulator* TileAccumulator::Create (scc::CDem& dem)
//{
//   return new GL_TileAccumulator (dem); 
//}

const int k_Texture_budget = 128; 

//
//
// GL_BackgroundRenderer
struct GL_BackgroundRenderer : public BackgroundRenderer
{
   
   struct   tx_ID_pair { GLuint c, h; }; 

   typedef  std::map<BG_tile::Key, tx_ID_pair, BG_tile::less_than> TextureIDMap; 


   BG_tile::Map*           zone_map_N; 
   BG_tile::Map*           zone_map_S; 

   TextureIDMap            tex_ID_cache[BG_tile::kNumTextureLevels];
   GLuint                  texture_ID_budget[2 * k_Texture_budget]; 
   std::vector<tx_ID_pair> available_texture_IDs;

   std::shared_ptr<glm::dvec3>      grid_verts[BG_tile::kNumTextureLevels]; 
   std::shared_ptr<glm::dvec2>      grid_txcrds;
   std::shared_ptr<unsigned short>  grid_inds; 

   bool no_draw; 

   HDC               hdc; 
   HGLRC             glrc; 
   GLEWContext*      glew_Context; 
   
   std::map<std::string, GLuint> shader_Table; 
   std::map<std::string, GLint>  uniformLoc_map; 
   std::map<std::string, GLint>  attribLoc_map; 


   GLuint Create_shader          (const char* shaderSource, GLenum shaderType); 
   GLuint Build_shader_program   (const GLuint* shaders); 
   void Validate_GL_call         (); 
   //
   //
   GL_BackgroundRenderer () 
      : zone_map_N (0)
      , zone_map_S (0)      
      // , grid_verts (0) 
      , grid_txcrds(0)
      , grid_inds  (0) 
      , hdc (0)      
      , glrc(0)      
      , glew_Context(0) 
   {
      Initialize (); 
   }
   
   //
   //
   virtual ~GL_BackgroundRenderer () 
   {
      glDeleteTextures (k_Texture_budget , texture_ID_budget); 
      available_texture_IDs.clear (); 
   }

   //
   //
   int Initialize ()
   {
      // alloc geometry
      int num_verts = BG_tile::kVertexDim_x * BG_tile::kVertexDim_y;
      int num_cols = BG_tile::kVertexDim_x - 1; 
      int num_rows = BG_tile::kVertexDim_y - 1; 

      //rows * cols * 3inds/tri * 2tris/tile; 
      int num_inds = num_rows * num_cols * 2 * 3; 

      for (int ilvl = 0; ilvl < k_MaxDetailLevelToBuild; ilvl++)
      {
         int      baseStep = 0x1 << ilvl; 
         int      iKilometer_step   = baseStep; 
         double   dKilometer_step   = static_cast<double>(iKilometer_step); 
         int      level_ind         = BG_tile::Level_index (ilvl);

         grid_verts[level_ind].reset (new glm::dvec3[num_verts] ); 
         Generate_grid_vert3 (grid_verts[level_ind].get(), BG_tile::kVertexDim_x, BG_tile::kVertexDim_y, dKilometer_step * 1000.0, dKilometer_step * 1000.0); 
      }

      grid_txcrds.reset (new glm::dvec2[num_verts] ); 
      grid_inds.reset   (new unsigned short[num_inds]  ); 


      Generate_grid_vert2 (grid_txcrds.get(), BG_tile::kVertexDim_x, BG_tile::kVertexDim_y, 1.0, 1.0);
      Generate_tri_grid_indices (grid_inds.get(), num_cols, num_rows);
      return 0; 
   }

   //
   //
   GLEWContext* glewGetContext ()
   {
      BOOST_ASSERT (glew_Context); 
      return glew_Context; 
   }

   //
   //
   virtual int Initialize_draw_context (GLEWContext* gc, HDC hdc, HGLRC glrc, scc::CDem& dem, BG_tile::Map* zones_N, BG_tile::Map* zones_S)
   {
      glew_Context = gc; 


      GLboolean ver_avail_11 = GLEW_VERSION_1_1  ;
      GLboolean ver_avail_12 = GLEW_VERSION_1_2  ;
      GLboolean ver_avail_121 = GLEW_VERSION_1_2_1;
      GLboolean ver_avail_13 = GLEW_VERSION_1_3  ;
      GLboolean ver_avail_14 = GLEW_VERSION_1_4  ;
      GLboolean ver_avail_15 = GLEW_VERSION_1_5  ;
      GLboolean ver_avail_20 = GLEW_VERSION_2_0  ;
      GLboolean ver_avail_21 = GLEW_VERSION_2_1  ;
      GLboolean ver_avail_30 = GLEW_VERSION_3_0  ;
      GLboolean ver_avail_31 = GLEW_VERSION_3_1  ;
      GLboolean ver_avail_32 = GLEW_VERSION_3_2  ;
      GLboolean ver_avail_33 = GLEW_VERSION_3_3  ;
      GLboolean ver_avail_40 = GLEW_VERSION_4_0  ;
      GLboolean ver_avail_41 = GLEW_VERSION_4_1  ;
      GLboolean ver_avail_42 = GLEW_VERSION_4_2  ;
      GLboolean ver_avail_43 = GLEW_VERSION_4_3  ;
      GLboolean ver_avail_44 = GLEW_VERSION_4_4  ;

      BOOST_ASSERT (gc); 
      BOOST_ASSERT (0 == zone_map_N); 
      BOOST_ASSERT (0 == zone_map_S); 
      BOOST_ASSERT (zones_N); 
      BOOST_ASSERT (zones_S); 

      zone_map_N = zones_N;  
      zone_map_S = zones_S; 


       
      glGenTextures (2 * k_Texture_budget , texture_ID_budget); 

      for (int i = 0; i < k_Texture_budget; i++) 
      {
         tx_ID_pair txs = { texture_ID_budget[i], texture_ID_budget[k_Texture_budget + i] }; 
         // color texture
         glBindTexture  (GL_TEXTURE_2D, txs.c); 
         glTexImage2D   (GL_TEXTURE_2D, 0, GL_RGB, BG_tile::kTextureWidth, BG_tile::kTextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0); 
         Validate_GL_call (); 

         // height
         glBindTexture  (GL_TEXTURE_2D, txs.h); 
         glTexImage2D   (GL_TEXTURE_2D, 0, GL_RED, BG_tile::kHeightMap_dim, BG_tile::kHeightMap_dim, 0, GL_RED, GL_FLOAT, 0); 
         Validate_GL_call (); 
      
         available_texture_IDs.push_back (txs); 

      }

      
      //
      
      
      
      //static char vert_shader_source[] = {
      //   "  uniform mat4      mat_Model   ;                                               \n"
      //   "  uniform mat4      mat_View    ;                                               \n"
      //   "  uniform mat4      mat_Proj    ;                                               \n"
      //   "  uniform float     height_Scale;                                               \n"
      //   "  uniform sampler2D heightMap;                                                  \n"
      //   "                                                                                \n"
      //   "  in vec2      coord_EN;                                                        \n"
      //   "  in vec2      texCoord;                                                        \n"
      //   "                                                                                \n"
      //   "  varying vec2     frag_txcd;                                                   \n"
      //   "                                                                                \n"
      //   "  void main ()                                                                  \n"
      //   "  {                                                                             \n"
      //   "     float height_Coord   = height_Scale * texture2D (heightMap, texCoord).x;   \n"
      //   "     vec4  posi           = vec4 (coord_EN.x, coord_EN.y, height_Coord, 1.0);   \n"
      //   "     posi = mat_Model * posi;                                                   \n"
      //   "     posi = mat_View * posi;                                                    \n"
      //   "                                                                                \n"
      //   "     frag_txcd   = texCoord;                                                    \n"
      //   "     gl_Position = mat_Proj * posi;                                             \n"
      //   "  }                                                                             \n"
      //   "                                                                                \n"
      //   }; 

      // static GLchar frag_shader_source[] = {
      //    "                                                     \n"
      //    "  uniform sampler2D colorMap;                        \n"
      //    "                                                     \n"
      //    "  in vec2           frag_txcd;                       \n"
      //    "                                                     \n"
      //    "  // out vec4          frag_color                    \n"
      //    "  void main ()                                       \n"
      //    "  {                                                  \n"
      //    "     gl_FragColor = texture2D (colorMap, frag_txcd); \n"
      //    "  }                                                  \n"
      //    "                                                     \n"
      //    }; 

      std::string vert_shader_source; 
      From_file (vert_shader_source, "C:/usr/bg_shader.vp"); 
      shader_Table["BG_vs"] = Create_shader (vert_shader_source.c_str(), GL_VERTEX_SHADER);  

      std::string frag_shader_source; 
      From_file (frag_shader_source, "C:/usr/bg_shader.fp"); 
      shader_Table["BG_fs"] = Create_shader (frag_shader_source.c_str(), GL_FRAGMENT_SHADER); 

      GLuint shaders[] = 
      {
         shader_Table["BG_vs"] , 
         shader_Table["BG_fs"] , 
         0
      } ; 

      shader_Table["BG_prog"] = Build_shader_program (shaders); 

      // find input locations
      attribLoc_map["coord_EN"] = glGetAttribLocation (shader_Table["BG_prog"], "coord_EN");
      attribLoc_map["texCoord"] = glGetAttribLocation (shader_Table["BG_prog"], "texCoord");

      // contants
      uniformLoc_map["colorMap"]    = glGetUniformLocation (shader_Table["BG_prog"], "colorMap");
      uniformLoc_map["heightMap"]   = glGetUniformLocation (shader_Table["BG_prog"], "heightMap");
      uniformLoc_map["height_Scale"]= glGetUniformLocation (shader_Table["BG_prog"], "height_Scale");
      
      uniformLoc_map["mat_Model"]   = glGetUniformLocation (shader_Table["BG_prog"], "mat_Model");
      uniformLoc_map["mat_View"]    = glGetUniformLocation (shader_Table["BG_prog"], "mat_View");
      uniformLoc_map["mat_Proj"]    = glGetUniformLocation (shader_Table["BG_prog"], "mat_Proj");

      return 0; 
   } 


   //
   //
   virtual int Deinitialize_draw_context ()
   {
      BOOST_ASSERT (0); 
      return -1;
   } 

   //
   //
   virtual int Draw (
      const int*        dim_viewport, 
      const PLANE*      frustum, 
      const glm::dmat4& mat_Proj, 
      const glm::dmat4& mat_View, 
      const glm::dvec3& look_At, 
      int      zone, 
      double   zoom,
      double   scale_alt)
   {
      glm::ivec2 view_dim     (dim_viewport[0], dim_viewport[1]); 

      const PLANE* frus[6] = {
         frustum + 0, // right
         frustum + 1, // left
         frustum + 2, // bottom 
         frustum + 3, // top 
         frustum + 4, // far 
         frustum + 5, // near
         }; 
      
      
      // zoom - higher value : +detail, less area
      double inv_zoom = 1.0 / zoom ; 

      BG_tile::Map& zone_map = zone_map_N[zone]; 

      std::vector<BG_tile::Struc*>  DBG_frustum_q(69);  
      std::vector<glm::ivec2>       DBG_frustum_ID(69);  

      DBG_frustum_q.clear ();
      DBG_frustum_ID.clear (); 

      if (zone_map.size () == 0)
         return 0; 

      int detail_level = -static_cast<int> (std::log (zoom) / std::log (2.0)); 


      // temporary, until we support all levels
      if (detail_level < 0)
            detail_level = 0; 
      if (detail_level >= k_MaxDetailLevelToBuild)
         detail_level = k_MaxDetailLevelToBuild - 1; 
      // 

      int      iKilometer_step  = 0x1 << detail_level;  
      double   dKilometer_step  = static_cast<double> (iKilometer_step);  
      int      level_Base1      = detail_level + 1; 
      int      level_ind        = BG_tile::Level_index (detail_level); 

      
      //
      // find in view 
      glm::dvec3  view_radius       = inv_zoom * 0.5 * glm::dvec3 (static_cast<double>(view_dim.x), static_cast<double>(view_dim.y), 0.0);
      double      len_View_radius   = glm::length (view_radius) + dKilometer_step * 500.0; 

      std::vector<BG_tile::Struc*>  draw_q   (128);
      std::vector<glm::ivec2>       draw_ID  (128);
      draw_q.clear ();    
      draw_ID.clear (); 


      for (BG_tile::Map::iterator it = zone_map.begin ();  it != zone_map.end (); it++)
      {
         const glm::ivec2& posKM = it->first; 
         BG_tile::Struc&   tile  = it->second; 

         // skip records that are not on our detail level step
         if (it->second.tile_level[level_ind].type == scc::data_type::none) 
            continue; 

         if (it->first.x % iKilometer_step || it->first.y % iKilometer_step)
            continue; 

         int coord_KM_x = it->first.x; 
         int coord_KM_y = it->first.y; 

         // how far is this tile from the look_At 
         glm::dvec3 diff_pos (it->first.x * 1000.0, it->first.y * 1000.0, 0.0); 
         // center of the tile

         diff_pos.x += dKilometer_step * 500.0;
         diff_pos.y += dKilometer_step * 500.0;
         diff_pos = look_At - diff_pos ; 

         if (glm::length (diff_pos) < len_View_radius)
         {
            draw_q.push_back (&it->second); 
            draw_ID.push_back (it->first); 
         } 
      }

      if (draw_q.size () == 0)
         return 0; // nothing to draw



      glMatrixMode (GL_MODELVIEW); 
      glPushMatrix (); 

      glUseProgram (shader_Table["BG_prog"]); 
      Validate_GL_call (); 


static int DBG_polygon_mode = 2; 
GLenum polygon_Mode[] = { GL_POINT,  GL_LINE,  GL_FILL }; 


      glPolygonMode (GL_FRONT_AND_BACK, polygon_Mode[DBG_polygon_mode]); 
      Validate_GL_call (); 




      bool  use_height_texture   = true; 
      if (use_height_texture)
      {
         glUniform1f (uniformLoc_map["height_Scale"], static_cast<float> (scale_alt)); 
         Validate_GL_call (); 
      }

      glEnable (GL_TEXTURE_2D); 
      Validate_GL_call (); 

      //glDisable (GL_DEPTH_TEST); 
      //Validate_GL_call (); 
      glEnableVertexAttribArray  (attribLoc_map["coord_EN"]); 
      glVertexAttribPointer      (attribLoc_map["coord_EN"], 3, GL_DOUBLE, GL_FALSE, 0, grid_verts[level_ind].get()); 
      Validate_GL_call (); 

      glEnableVertexAttribArray  (attribLoc_map["texCoord"]); 
      glVertexAttribPointer      (attribLoc_map["texCoord"], 2, GL_DOUBLE, GL_FALSE, 0, grid_txcrds.get()); 
      Validate_GL_call (); 


      // draw tiles in view 
      for (size_t i = 0; i < draw_q.size (); i++) 
      {
         int texture_stage = 0; 

         glm::ivec2& tile_KM_Pos = draw_ID[i]; 
         glm::dvec3  tile_M_Pos  (static_cast<double>(tile_KM_Pos.x), static_cast<double>(tile_KM_Pos.y), 0.0); 
         
         tile_M_Pos.x *= 1000.0;
         tile_M_Pos.y *= 1000.0;

         if (tex_ID_cache[level_ind].count(tile_KM_Pos))
         {
            tx_ID_pair&  IDs = tex_ID_cache[level_ind][tile_KM_Pos]; 
             
            if (use_height_texture)
            {
               glActiveTexture (GL_TEXTURE_stage (texture_stage)); 
               glBindTexture (GL_TEXTURE_2D, IDs.h); 
               Validate_GL_call (); 
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

               glUniform1i (uniformLoc_map["heightMap"], texture_stage); // glBindSampler (0, uniformLoc_map["heightMap"]); 
               Validate_GL_call (); 
               texture_stage++; 
            }

            glActiveTexture (GL_TEXTURE_stage (texture_stage)); 
            glBindTexture (GL_TEXTURE_2D, IDs.c); 
            Validate_GL_call (); 
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            Validate_GL_call (); 
            
            glUniform1i (uniformLoc_map["colorMap"], texture_stage); // glBindSampler (1, uniformLoc_map["colorMap"]); 
            Validate_GL_call (); 
            texture_stage++; 
         }
         else 
         if (available_texture_IDs.size ())
         {
            tx_ID_pair&  IDs = available_texture_IDs.back(); 

            if (use_height_texture)
            {
               glActiveTexture (GL_TEXTURE_stage (texture_stage)); 
               glBindTexture (GL_TEXTURE_2D, IDs.h); 

               std::shared_ptr<float> fhghts (new float[BG_tile::kHeightMap_dim * BG_tile::kHeightMap_dim]);
               short* hts = static_cast<short*> (draw_q[i]->tile_level[level_ind].height); 
               std::copy (hts, hts + BG_tile::kHeightMap_dim * BG_tile::kHeightMap_dim, fhghts.get()); 
      
               glTexSubImage2D (
                  GL_TEXTURE_2D, 
                  0, 
                  0, 
                  0, 
                  BG_tile::kHeightMap_dim,  
                  BG_tile::kHeightMap_dim, 
                  GL_RED,
                  GL_FLOAT,   // GL_SHORT,
                  fhghts.get()      // draw_q[i]->tile_level[level_ind].height
                  ); 
               Validate_GL_call (); 

               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
               Validate_GL_call (); 

               glUniform1i (uniformLoc_map["heightMap"], texture_stage); 
               Validate_GL_call (); 
               texture_stage++; 
            }


            glActiveTexture (GL_TEXTURE_stage (texture_stage)); 
            glBindTexture (GL_TEXTURE_2D, IDs.c); 
            BOOST_ASSERT (draw_q[i]->tile_level[level_ind].type != scc::data_type::none); 
            glTexSubImage2D (
               GL_TEXTURE_2D, 
               0, 
               0, 
               0, 
               BG_tile::kTextureWidth, 
               BG_tile::kTextureHeight, 
               GL_RGB,
               GL_UNSIGNED_BYTE,
               draw_q[i]->tile_level[level_ind].color
               ); 
            Validate_GL_call (); 

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            Validate_GL_call (); 

            glUniform1i(uniformLoc_map["colorMap"], texture_stage); //glBindSampler (1, uniformLoc_map["colorMap"]); 
            Validate_GL_call (); 
            texture_stage++; 

            tex_ID_cache[level_ind][tile_KM_Pos] = IDs; 
            available_texture_IDs.pop_back (); 
         } 
         else
         {

            bool        found_available_ID   = false;
            tx_ID_pair  IDs                  = {}; 

            for (int iLvl = 0; iLvl < k_MaxDetailLevelToBuild; iLvl++) 
            {
               int curr_ind = BG_tile::Level_index (iLvl); 

               for (TextureIDMap::iterator it = tex_ID_cache[curr_ind].begin (); it != tex_ID_cache[curr_ind].end(); it++)
               {
                  std::vector<glm::ivec2>::iterator find_it = std::find (draw_ID.begin(), draw_ID.end(), it->first); 

                  if (draw_ID.end() == find_it && curr_ind != level_ind)
                  {
                     found_available_ID   = true; 
                     IDs                  = it->second;
                     // this location isnt being drawn, reuse it
                     tex_ID_cache[level_ind][tile_KM_Pos] = IDs; 
                     // erase old one
                     tex_ID_cache[curr_ind].erase (it); 
                     break; 
                  }
               }

               if (found_available_ID)
               {
                  BOOST_ASSERT (draw_q[i]->tile_level[level_ind].type != scc::data_type::none); 

                  if (use_height_texture)
                  {
                     glActiveTexture (GL_TEXTURE_stage (texture_stage)); 
                     glBindTexture (GL_TEXTURE_2D, IDs.h); 
                  
                     std::shared_ptr<float> fhghts (new float[BG_tile::kHeightMap_dim * BG_tile::kHeightMap_dim]);
                     short* hts = static_cast<short*> (draw_q[i]->tile_level[level_ind].height); 
                     std::copy (hts, hts + BG_tile::kHeightMap_dim * BG_tile::kHeightMap_dim, fhghts.get()); 

                     glTexSubImage2D (
                        GL_TEXTURE_2D, 
                        0, 
                        0, 
                        0, 
                        BG_tile::kHeightMap_dim,  
                        BG_tile::kHeightMap_dim, 
                        GL_RED,
                        GL_FLOAT,      // GL_SHORT,
                        fhghts.get()   // draw_q[i]->tile_level[level_ind].height
                        ); 
                     Validate_GL_call (); 
                  
                     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                     Validate_GL_call (); 
                     glUniform1i (uniformLoc_map["heightMap"], texture_stage);  //glBindSampler (0, uniformLoc_map["heightMap"]); 
                     Validate_GL_call (); 
                     texture_stage++; 
                  }

                  glActiveTexture (GL_TEXTURE_stage (texture_stage)); 
                  glBindTexture (GL_TEXTURE_2D, IDs.c); 
                  // map to new location
                  glTexSubImage2D (
                     GL_TEXTURE_2D, 
                     0, 
                     0, 
                     0, 
                     BG_tile::kTextureWidth, 
                     BG_tile::kTextureHeight, 
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     draw_q[i]->tile_level[level_ind].color
                     ); 
                  Validate_GL_call (); 
                  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                  Validate_GL_call (); 

                  glUniform1i (uniformLoc_map["colorMap"], texture_stage);  // glBindSampler (1, uniformLoc_map["colorMap"]); 
                  Validate_GL_call (); 
                  texture_stage++; 

                  break;
               }
               else
               {
                  int all_textures_are_used = 0; 
                  all_textures_are_used ++ ; 
                  break;
               }
            }
         }

         Validate_GL_call (); 

         glm::fvec3 model_pos; 
            model_pos.x = tile_M_Pos.x;
            model_pos.y = tile_M_Pos.y;
            model_pos.z = tile_M_Pos.z;

         glm::fmat4 mat_Model = glm::translate (model_pos); 
         glUniformMatrix4fv (uniformLoc_map["mat_Model"] , 1, GL_FALSE, glm::value_ptr (mat_Model)); 
         Validate_GL_call (); 
         
         float fMat_view[16]; // double precision (glUniformMatrix4dv) not available
         std::copy (glm::value_ptr (mat_View), glm::value_ptr (mat_View) + 16, fMat_view); 
         glUniformMatrix4fv (uniformLoc_map["mat_View"], 1, GL_FALSE, fMat_view); 
         Validate_GL_call (); 

         float fMat_proj[16];// double precision (glUniformMatrix4dv) not available
         std::copy (glm::value_ptr (mat_Proj), glm::value_ptr (mat_Proj) + 16, fMat_proj); 
         glUniformMatrix4fv (uniformLoc_map["mat_Proj"], 1, GL_FALSE, fMat_proj); 
         Validate_GL_call (); 

         int ind_count = 2 * 3 * (BG_tile::kVertexDim_x - 1) * (BG_tile::kVertexDim_y - 1); 

         glDrawElements (GL_TRIANGLES, ind_count, GL_UNSIGNED_SHORT, grid_inds.get()); 
         Validate_GL_call (); 
      }

glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); 

      glUseProgram (0); 

      glEnable (GL_DEPTH_TEST); 
      glDisableVertexAttribArray (attribLoc_map["coord_EN"]); 
      glDisableVertexAttribArray (attribLoc_map["texCoord"]); 
      glMatrixMode (GL_MODELVIEW); 
      glPopMatrix(); 
      return 0; 

   } 
}; 

//
//
BackgroundRenderer* BackgroundRenderer::Create ()
{ 
   return new GL_BackgroundRenderer; 
}



void GL_BackgroundRenderer::Validate_GL_call ()
{
   int wat = 0; 
   GLenum         err      = 0; 
   const GLubyte* error_s_ = 0; 
      
   err = glGetError ();
   if (err != GL_NO_ERROR )
   {
      error_s_  = glewGetErrorString (err);
      BOOST_ASSERT (0); 
   }


   wat++; 
}

GLuint GL_BackgroundRenderer::Create_shader (const GLchar* shaderSource, GLenum shaderType)
{
   BOOST_ASSERT (shaderSource); 
   BOOST_ASSERT (shaderType == GL_VERTEX_SHADER  || shaderType == GL_FRAGMENT_SHADER); 

   GLuint shaderID = glCreateShader (shaderType); 

   const GLchar* source[] = {
      shaderSource, 
      0   
      }; 

   GLint len_source[] = { 
      strlen (shaderSource) - 1, 
      0
      }; 
      


   glShaderSource (shaderID, 1, source, len_source); 

   glCompileShader(shaderID); 

   int compileStatus;
	glGetShaderiv (shaderID, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE) 
	{
		GLchar err_buf[1024];
		GLsizei infolen;
		glGetShaderInfoLog (shaderID, 1024, &infolen, err_buf);
		//Debug ("Shader Error:\n %s", output);
		//BreakAssert (0, "shader compile fail");
      glDeleteShader(shaderID); 
      BOOST_ASSERT (0); 
		return 0;
	}

   return shaderID; 

}


GLuint GL_BackgroundRenderer::Build_shader_program (const GLuint* shaders)
{
   GLuint progID = glCreateProgram (); 

   int count_shaders = 0; 
   while (shaders[count_shaders]) 
   {
      glAttachShader (progID, shaders[count_shaders]); 
      count_shaders++;
   }   

   // 
   // 
   glLinkProgram (progID);

	GLint    linkRes;
	GLsizei  bufflen;
	char     outputbuffer[1024];
	glGetProgramiv (progID, GL_LINK_STATUS, &linkRes);
	if (linkRes == GL_FALSE)
	{
		glGetProgramInfoLog (progID, 1024, &bufflen, outputbuffer);
		//Debug ( "\nGL Info Log :  \n\n %s\n", outputbuffer);
		//Assert (0, "ShaderProg::Link () - Failed");
      glDeleteProgram (progID); 
		return 0;
	}

   return progID; 
}

