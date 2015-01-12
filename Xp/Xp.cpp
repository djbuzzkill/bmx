#include "Xp.h"


static TCHAR sg_DefaultWindowClass[] = "XpDefaultWindowClass"; 			// the main window class name

//
//
LRESULT CALLBACK Win32_WndProc (
	HWND hWnd, 
	UINT message, 
	WPARAM wParam, 
	LPARAM lParam
	); 

//
//
HWND Create_HWND_default (
	HINSTANCE hInstance, 
	const char* title, 
	int nCmdShow
	);

//
// Conv_2_uint 
ui_t str_2_uint (const std::string& str)
{
	ui_t accum = 0;

	for (size_t i = 0; i < str.size (); i++)
	{
		ui_t digi = str[str.size () - i - 1] - '0'; 
		accum += digi * pow (10, i) ;
	}
	
	return accum; 
}

//
// str_2_int 
si_t str_2_int (const std::string& str)
{
	if (str[0] == '-')
		return -(si_t) str_2_uint (str.c_str () + 1); 

	if (str[0] == '+')
		return str_2_uint (str.c_str () + 1); 

	return str_2_uint (str); 
}

//
//
size_t Max_elems_in_buff (size_t sizeOf_buf, size_t sizeOf_elem)
{
   Assert (sizeOf_elem, "sizeOf_elem is 0"); 

   if (!sizeOf_elem)
      return ~0x0; 

   return (sizeOf_buf / sizeOf_elem); 
} 

//
// sg_Default_PixelFormatDescriptor
static PIXELFORMATDESCRIPTOR sg_Default_PIXELFORMATDESCRIPTOR = {

	sizeof (PIXELFORMATDESCRIPTOR), // WORD  nSize;
	0x0001,  // WORD  nVersion;
	PFD_DRAW_TO_WINDOW|PFD_DOUBLEBUFFER|PFD_SUPPORT_OPENGL, // DWORD dwFlags;
	PFD_TYPE_RGBA,                                              // BYTE  iPixelType;
	32,      // BYTE  cColorBits;

	0,			// BYTE  cRedBits;
	0,			// BYTE  cRedShift;
	0,			// BYTE  cGreenBits;
	0,			// BYTE  cGreenShift;
	0,			// BYTE  cBlueBits;
	0,			// BYTE  cBlueShift;
				   
	0,			// BYTE  cAlphaBits;
	0,			// BYTE  cAlphaShift;
	0,			// BYTE  cAccumBits;
	0,			// BYTE  cAccumRedBits;
	0,			// BYTE  cAccumGreenBits;
	0,			// BYTE  cAccumBlueBits;
	0,			// BYTE  cAccumAlphaBits;
				   
	24,			// BYTE  cDepthBits;
	8,			// BYTE  cStencilBits;
	0,			// BYTE  cAuxBuffers;
				   
	0,			// BYTE  iLayerType;
	0,			// BYTE  bReserved;
	0,			// DWORD dwLayerMask;
	0,			// DWORD dwVisibleMask;
	0,			// DWORD dwDamageMask;	
	}; 




namespace Fs 
{
   //
	enum Type {
		Invalid_FS_Type = -1, 
		RegFile, 
		DirPath , 
		}; 

	// Filesystem Node 
	struct Node {
		Type		   type; 
		std::string label;
		HANDLE		handl; 
		}; 

   //
   // Find_dir 
	bool Find_dir (Node& node, const std::string& cur_Path)
	{
		WIN32_FIND_DATA   findFile, 
                        nextFind; 

		std::string       list_Path   = cur_Path + "/*"; 
		HANDLE            handl       = ::FindFirstFile (list_Path.c_str (), &findFile); 

		if (handl != INVALID_HANDLE_VALUE)
		{
			node.handl = handl; 
			node.label = findFile.cFileName; 
			
			if (findFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				node.type =	DirPath;  
			}
			else 
			if (findFile.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)
			{
				node.type = RegFile;  
			}
			else
			{
				node.type = Invalid_FS_Type; 
			}

			return true; 
		}
		return false; 

	}

	bool Close_dir (Node& node)
	{
		return (TRUE == ::FindClose (node.handl));  
	}

	bool Next_entry (Node& next, const Node& cur_Node)
	{
		WIN32_FIND_DATA findFile; 

		if (::FindNextFile (cur_Node.handl, &findFile))
		{
			next.label = findFile.cFileName; 
			next.handl = cur_Node.handl; 

			if (findFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				next.type =	DirPath;  
			}
			else 
			if (findFile.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
			{
				// FILE_ATTRIBUTE_NORMAL is not correct 
				next.type = RegFile;  
			}
			else
			{
				next.type = Invalid_FS_Type; 
			}

			return true; 
		}

		DWORD error_Code = ::GetLastError (); 

		return false; 
	}

   //
   //
   bool Collect_exclusion_list (const std::string& txt)
   {
      // nothing for now
      return false; 
   }

   bool File_exclusion_list (const std::string& txt)
   {
      return false; 
   }

   //
   // Build_Dir_tree - build tree from archive path
   Dir_node* Build_Dir_tree (
	   Dir_node*			   dir_Node, 
	   Dir_node::Map&		   dir_Map, 
	   const std::string&	accum_Path,	
	   const std::string&	disk_Path) 
   {
	   Node fs_Node, fs_Next; 

	   const std::string archive_Path =  disk_Path + "/" + accum_Path; 

	   if ( Find_dir (fs_Node, archive_Path)) 
	   {
		   if (!dir_Node)
		   {
            // this better be the root
			   size_t node_Index = dir_Map.size () + 1; 
			   dir_Node		= &dir_Map[node_Index]; // node_Bucket.Get (); 
			   dir_Node->name	= accum_Path; 
			   dir_Node->index = node_Index; 
		   }

		   while (Next_entry (fs_Next, fs_Node)) //for (bfs::directory_iterator iter (cur_Path); iter != bfs::directory_iterator (); iter++)
		   {
			   if (fs_Next.label == "." || fs_Next.label == "..")
				   continue; 

			   if (fs_Next.type == DirPath) //if (bfs::is_directory (iter->status ()))
			   {
				   size_t dir_Index = dir_Map.size () + 1; 
				   Dir_node* new_Node = &dir_Map[dir_Index];// 
				   new_Node->index = dir_Index; 

				   Add_child (dir_Node, new_Node);			// dir_Node->children.push_back (new_Node); 
				   new_Node->name = fs_Next.label;			//new_Node->name		= iter->leaf (); 
				   Build_Dir_tree (new_Node, dir_Map, accum_Path + "/" + new_Node->name, disk_Path); 
			   }
		   }

		   Close_dir (fs_Node); 
	   }

	   return dir_Node;
   }


   //
   //
   // IndexedMap& Build_Dir_table (
   // 	IndexedMap&			out, 
   // 	Dir_node::Map&		dir_Map, 
   // 	const std::string&	archive_ID,
   // 	const std::string&	disk_Path
   // 	)
   // {
   // 	std::string archive_Path = disk_Path + "/" + archive_ID; 
   // 	
   // 	//Dir_node::Bucket node_Bucket (1024); 
   // 	//
   // 	Dir_node* dir_Root = Build_Dir_tree (
   // 		0, 
   // 		dir_Map, // node_Bucket, 
   // 		archive_ID,
   // 		disk_Path 
   // 		); 
   // 	
   // 	//
   // 	// FQ names - relavtive path from base (ar_path). 
   // 	for (Dir_node::Map::iterator iter = dir_Map.begin (); iter != dir_Map.end(); iter++)
   // 		out[iter->first] = FQ_path_name (iter->second, "/");
   // 	
   // 	return out; 
   // }


    File_entry::Map& Collect_files (
	   File_entry::Map&		out, 
	   //Ar_file_entry::Bucket&	bucket, 
	   const IndexedMap&		fq_Paths, 
	   const std::string&		disk_Path, 
	   const size_t			disk_Index)
   {
      std::map<std::string, std::vector<int> > uniq_map;    
      for (File_entry::Map::const_iterator it = out.begin (); it != out.end (); it++)
         uniq_map[it->second.name].push_back (it->second.parent_Index); 

	   size_t file_Counter = out.size ();
	   for (IndexedMap::const_iterator it = fq_Paths.begin (); it != fq_Paths.end (); it++)
	   {
		   Node dirNode, curNode; 
		   if ( Find_dir (dirNode, disk_Path + "/" + it->second))
		   {
			   while ( Next_entry (curNode, dirNode))
			   {
               // exclude 
               if (File_exclusion_list (curNode.label))
                  continue; 

               // check the initial contents
               if (uniq_map.count (curNode.label) && std::count (uniq_map[curNode.label].begin (), uniq_map[curNode.label].end (), it->first))
                  continue; 

				   if (curNode.type == RegFile)
				   {
					   file_Counter++; // so we can be non-zero
					   //out[file_Counter] = bucket.Get (); 
					   out[file_Counter].name			= curNode.label; 
					   out[file_Counter].parent_Index	= it->first ; 
					   out[file_Counter].disk_Index	= disk_Index;
				   }
			   }

			   Close_dir (dirNode); 
		   }
	   }
	   return out; 
   }

}


    
/**
 * MyRegisterClass
 */
// void xp::H3D::Obj::Set_pos (const Ma::Vec3f& p)
// {
//    Ma::Copy (pos, p); 
//    update_xf();
// }
// 
// void xp::H3D::Obj::Set_rot (const Ma::Vec3f& r)
// {
//    Ma::Copy (rot, r); 
//    update_xf();
// }
// 
// void xp::H3D::Obj::Set_scl (const Ma::Vec3f& s)
// {
// 	Ma::Copy (scl, s); 
// 	update_xf();
// }
// 
// xp::H3D::Obj::H3D::Obj ()
// 	: node (0)
// {}
// 
// xp::H3D::Obj::H3D::Obj (H3DNode n)
// 	:node (n)
// {
// 	if (node)
// 		Bind (node); 
// }
// 
// //
// void xp::H3D::Obj::Bind (H3DNode n)
// {
// 	node = n; 
// 	if (!node)
// 		return;
// 
// 	h3dGetNodeTransform (
//       node, 
// 		&Ma::X (pos), &Ma::Y (pos), &Ma::Z (pos), 
// 		&Ma::X (rot), &Ma::Y (rot), &Ma::Z (rot), 
// 		&Ma::X (scl), &Ma::Y (scl), &Ma::Z (scl));
// }
// 
// //
// //
// Ma::Vec3f& xp::H3D::Obj::Get_pos (Ma::Vec3f& p) 
// {
// 
//    //h3dSetNode
//    //h3dGetNodeTransMats (
// 
// 
// 	h3dGetNodeTransform (
//       node, 
// 		&Ma::X (pos), &Ma::Y (pos), &Ma::Z (pos), 
// 		&Ma::X (rot), &Ma::Y (rot), &Ma::Z (rot), 
// 		&Ma::X (scl), &Ma::Y (scl), &Ma::Z (scl)
//       );
// 
// 
// 
// 
// 	return Ma::Copy (p, this->pos);
// } 
// 
// // IID1 for IS 1
// // RESRC ENGRDA for {XHD|IXSHD|TRE_OVERFLOW[n]} m  
// //
// Ma::Vec3f& xp::H3D::Obj::Get_rot (Ma::Vec3f& r) {
// 
// 	h3dGetNodeTransform (node, 
// 		&Ma::X (pos), &Ma::Y (pos), &Ma::Z (pos), 
// 		&Ma::X (rot), &Ma::Y (rot), &Ma::Z (rot), 
// 		&Ma::X (scl), &Ma::Y (scl), &Ma::Z (scl));
// 
// 	return Ma::Copy (r, this->rot);
// 	} 
// 
// //
// //
// Ma::Vec3f& xp::H3D::Obj::Get_scl (Ma::Vec3f& s) {
// 
// 	h3dGetNodeTransform (node, 
// 		&Ma::X (pos), &Ma::Y (pos), &Ma::Z (pos), 
// 		&Ma::X (rot), &Ma::Y (rot), &Ma::Z (rot), 
// 		&Ma::X (scl), &Ma::Y (scl), &Ma::Z (scl));
// 
// 	return Ma::Copy (s, this->scl);
// 	} 
// 
// //
// //
// void xp::H3D::Obj::update_xf () {
// 
// 	if (!node)
// 		return;
// 
// 	h3dSetNodeTransform (
// 		node, 
// 		Ma::X (pos), Ma::Y (pos), Ma::Z (pos), 
// 		Ma::X (rot), Ma::Y (rot), Ma::Z (rot), 
// 		Ma::X (scl), Ma::Y (scl), Ma::Z (scl)
// 		);
// 	}

//
//
//void xp::Update_view_transform (
//	Ma::Vec3f&				view_Pos, 
//	Ma::Vec3f&				view_Rot, 
//	float					   move_Rate,
//	const OIS::Keyboard*	kb,    
//	const OIS::Mouse*		ms)
//{
//   float dYdx = 1.0f; 
//   float dXdy = 1.0f; 
//   
//   const OIS::MouseState& mouse = ms->getMouseState(); 
//   
//   Ma::Vec3f dir_Fwd, dir_Right; 
//   
//   
//   {
//      const OIS::Axis& ax_X = mouse.X ; 
//      const OIS::Axis& ax_Y = mouse.Y ; 
//   
//      Ma::X (view_Rot) += -(ax_Y.rel * dXdy);
//      Ma::Clamp (Ma::X (view_Rot), -60.0f, 60.0f); 
//   
//      Ma::Y (view_Rot) += ax_X.rel * -dYdx;
//   
//
//      if (ax_X.abs< 0)
//         Ma::Y (view_Rot);    
//
//      if (ax_X.rel < 0)
//         Ma::Y (view_Rot);    
//                  
//
//      while (Ma::Y (view_Rot) < 0.0)
//   	   Ma::Y (view_Rot) += 360.0f; 
//   
//      while (Ma::Y (view_Rot) > 360.0)
//   	   Ma::Y (view_Rot) -= 360.0f; 
//   
//      const float kDeg2Pi = (float) Ma::Pi / 180.0f; 
//      const float fHalfPi = (float) Ma::HalfPi;
//   
//      Ma::Spherical (dir_Fwd,    kDeg2Pi * Ma::Y (view_Rot), 0.0f);
//      Ma::Spherical (dir_Right,  kDeg2Pi * Ma::Y (view_Rot) - fHalfPi, 0.0f); 
//
//   }
//   
//   
//   
//   // view movement
//   {
//      Ma::Vec3f v;
//   
//      if (kb->isKeyDown (OIS::KC_F)) {
//   	   X (v) = X (Ma::Scale (v, dir_Fwd, move_Rate));
//         Ma::Incr (view_Pos, v);
//   	   }
//   
//      else if (kb->isKeyDown (OIS::KC_V)) {
//   	   X (v) = X (Ma::Scale (v, dir_Fwd, -move_Rate));
//         Ma::Incr (view_Pos, v);
//   	   }
//   
//      //
//      if (kb->isKeyDown (OIS::KC_D)) {
//   	   Z (v) = 	Z (Ma::Scale (v, dir_Right, -move_Rate)); 
//         Ma::Incr (view_Pos, v);
//   	   }
//   
//      if (kb->isKeyDown (OIS::KC_G)) {
//   	   Z (v) = 	Z (Ma::Scale (v, dir_Right, move_Rate)); 
//         Ma::Incr (view_Pos, v);
//   	   }
//   }   
//
//   // view orientation
//   //Ma::Set (view_Rot, 1.0f, 0.0f, 0.0f, 0.0f);
//   //if (sg_X_mouse > 0 && sg_Y_mouse > 0 && sg_X_mouse < sg_Wd_window && sg_Y_mouse < sg_Ht_window)
//   //	Ma::Mult (view_Rot, Ma::RotYAxis (q_Y, view_Y_rot), Ma::RotXAxis (q_X, view_X_rot)); 
//   
//}

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
   
   //const OIS::MouseState& mouse = ms->getMouseState(); 
   

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


// Init_Bullet
//xp::BlPhy_Comp& xp::Init_Bullet (
//   xp::BlPhy_Comp& obj)
//{
//
//   // Init_Physics 
//
//   return obj; 
//} 

//
//
// xp::OIS_Comp& xp::Init_input (
// 	OIS_Comp& out, 
// 	sy::System_context* sys, 
// 	sy::Window* win)
// {
// 	
// 	if (out.myInputMgr = sys->Create_input_system (win))
// 	{
// 		out.myKeyboard	= reinterpret_cast<OIS::Keyboard*>	(out.myInputMgr->createInputObject(OIS::OISKeyboard, true));
// 		out.myMouse		= reinterpret_cast<OIS::Mouse*>		(out.myInputMgr->createInputObject(OIS::OISMouse, true));
// 		out.myKeyboard->setBuffered (false); 
// 		out.myMouse->setBuffered	(false); 
// 		out.myKeyboard->capture (); 
// 		out.myMouse->capture (); 
// 
// 		const OIS::MouseState& mouse = 
// 				out.myMouse->getMouseState (); 
// 
// 		mouse.width = 640; 
// 		mouse.height = 480; 
// 	}
// 	return out; 
// }



//
//
class Win32_GL_window : public sy::Graphics_window
{
	HWND	m_hwnd; 
	HDC   m_DC; 
	HGLRC m_hglrc;
public: 
	//
	//
   Win32_GL_window (
      HWND hwnd, 
      HDC hdc, 
      const std::string& title) 
   : m_hwnd (hwnd)
   , m_DC (hdc)
   , m_hglrc (0) {
	   //
	   int iPxlFmt = ::ChoosePixelFormat (
		   m_DC,
		   &sg_Default_PIXELFORMATDESCRIPTOR
		   ); 

	   if (iPxlFmt) {
		   BOOL res = ::SetPixelFormat (
			   m_DC, 
			   iPxlFmt, 
			   &sg_Default_PIXELFORMATDESCRIPTOR
			   ); 

		   m_hglrc = ::wglCreateContext (m_DC); 
		   res = ::wglMakeCurrent (m_DC, m_hglrc); 
		   }

   }

	//
	//
   virtual ~Win32_GL_window () 
   {

   }

	//
	//
	void Swap_buffers () {
		::SwapBuffers (m_DC); 
		}

	//
	//
	int setup_display () {
		return 0; 
		}

	//
   //
   HWND Get_HWND () { 
	   return m_hwnd; 
	   }

   HDC Get_HDC () { 
	   return m_DC; 
	   }
   //
   //
   virtual size_t Get_WindowID () { 
	   return (size_t) m_hwnd; 
	   }; 
   //
   //
   virtual void Show (bool show) {
	   ::ShowWindow(m_hwnd, show ? SW_SHOW : SW_HIDE); 
	   ::UpdateWindow (m_hwnd);
	   }
   //
   //
   virtual void SetPos (int x, int y) {
	   //::SetWindowPos (m_hwnd, 0, 
	   }
}; 

//
//
class GL_DisplBuff : public sy::Graphics_display
{
	HDC	  m_HDC; 
	HGLRC m_hglrc;

public: 

	//
	GL_DisplBuff (HDC hdc) : m_HDC (hdc) , m_hglrc (0) {
		//
		int iPxlFmt = ::ChoosePixelFormat (
			m_HDC,
			&sg_Default_PIXELFORMATDESCRIPTOR
			); 

		if (iPxlFmt) {
			BOOL res = ::SetPixelFormat (
				m_HDC, 
				iPxlFmt, 
				&sg_Default_PIXELFORMATDESCRIPTOR
				); 

			m_hglrc = ::wglCreateContext (m_HDC); 
			res = ::wglMakeCurrent (m_HDC, m_hglrc); 
			}
		}
	
	//
	~GL_DisplBuff () {
		}

	//
	void Swap_buffers () {
		::SwapBuffers (m_HDC); 
		}

	//
	int	setup_display() {
		return 0; 
		}

}; 



// Win32Window 
class Win32_window : public sy::Window  
{
	HWND				m_hwnd; 
	HDC					m_DC; 

public: 

	//
	// Win32Window 
	Win32_window (HWND hwnd, HDC hdc, const char* title) 
		: m_hwnd (hwnd), m_DC (0) {
		}

	//
	~Win32_window () {
		// HINSTANCE hInst, const char* title	
		}

	//
	HWND Get_HWND () { 
		return m_hwnd; 
		}

	HDC Get_HDC () { 
		return m_DC; 
		}
	//
	//
	virtual size_t Get_WindowID () { 
		return (size_t) m_hwnd; 
		}; 


	//
	virtual void Show (bool show) {
		::ShowWindow(m_hwnd, show ? SW_SHOW : SW_HIDE); 
		::UpdateWindow (m_hwnd);
		}


	//
	virtual void SetPos (int x, int y) {
		//::SetWindowPos (m_hwnd, 0, 
		}

	}; 



ATOM Default_WindowClass (HINSTANCE hInstance) {
	WNDCLASSEX wcex;


	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc	= Win32_WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, 0);		//MAKEINTRESOURCE(IDI_HORDEINTGRTEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);	//
	wcex.hbrBackground	= NULL;							// (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;							// MAKEINTRESOURCE(IDC_HORDEINTGRTEST);
	wcex.lpszClassName	= sg_DefaultWindowClass;				//
	wcex.hIconSm		= LoadIcon(wcex.hInstance, 0);	// MAKEINTRESOURCE(IDI_SMALL));
	return RegisterClassEx (&wcex);
	}


//
//
static std::map<HWND, sy::Window_listener*>	sg_HWND_2_Listener; 
static std::map<sy::Window*, HDC>			sg_Window_HDC; 
static std::map<sy::Window*, HWND>			sg_Window_2_HWND; 


// 
// Win32SystemContext  
class Win32_system : public sy::System_context 
{
	HINSTANCE			appInstance; 
	ATOM					wndClassAtom; 

	double					dTimeStep; 
	LARGE_INTEGER			perf_freq; 
	LARGE_INTEGER			start_time; 
	LARGE_INTEGER			prev_time; 


public: 
	// Win32SystemContext 
	Win32_system (HINSTANCE hInstance) 
		: appInstance (hInstance) 
		, wndClassAtom (0) 
   {
		wndClassAtom = Default_WindowClass (hInstance); 


      size_t sizeOf_LARGE_INTEGER = sizeof (LARGE_INTEGER); 

		::QueryPerformanceFrequency (&perf_freq); 

		dTimeStep = 1.0 / (double)perf_freq.QuadPart; 

		ResetTime (); 
	}

	// Win32SystemContext 
	~Win32_system () {
      
		}



	// ResetTime 
	virtual	void ResetTime () {
		::QueryPerformanceCounter (&start_time); 
		::QueryPerformanceCounter (&prev_time); 
		} 

	// GetTimeElapsed
	virtual	double GetTimeElapsed () {
		LARGE_INTEGER curr_time; 

		::QueryPerformanceCounter (&curr_time); 

		return (curr_time.QuadPart * dTimeStep); 
		}
	// GetDt 
	virtual	double  GetDt () {
		LARGE_INTEGER curr_time; 

		::QueryPerformanceCounter (&curr_time); 

		long long ticks = curr_time.QuadPart - prev_time.QuadPart; 
		prev_time = curr_time; 

		return (ticks * dTimeStep); 
		}

   virtual  unsigned long long   GetTimerTicks      () { return 0; } 
   virtual  unsigned long long   GetTimerFrequencyHz     () { return 0; }


	// Request something
	virtual void SysReq (sy::System_context::Request req) 
   {

		switch (req) {
	
   		case sy::System_context::Req_Exit:
				PostQuitMessage (0); 
				break;

			default: 
				break;

			}
	}

	// Create_GL_Display
	virtual sy::Graphics_display* Create_display (sy::Window* w)
	{
		if (sg_Window_HDC.count (w))
			return new GL_DisplBuff (sg_Window_HDC[w]); 

		return 0; 
	}	
	
	// CreateOpengGLWindow 
	virtual sy::Window*	Create_window (
		sy::Window_listener*  lstnr, 
		const char*			   title)
	{
		HWND	hwnd	= Create_HWND_default (appInstance, title, SW_SHOW); 
		HDC		hdc		= ::GetDC (hwnd); 

		Win32_window* winWin = new Win32_window (hwnd, hdc, title); 

		sg_HWND_2_Listener[hwnd]	= lstnr; 
		sg_Window_HDC[winWin]		= hdc; 
		sg_Window_2_HWND[winWin]	= hwnd; 

		return winWin; 
	}


   // supercede GL_display + Window
   virtual sy::Graphics_window* Create_GraphicsWindow (
      sy::Window_listener*  listening, 
      const char*          title, 
      int wd, 
      int ht, 
      bool fullscreen
      )
   {
		HWND  hwnd  = Create_HWND_default (appInstance, title, SW_SHOW); 
		HDC	hdc   = ::GetDC (hwnd);
  
  	   sy::Graphics_window* ret_win   = new Win32_GL_window (hwnd, hdc, title); 
		sg_HWND_2_Listener[hwnd]	   = listening; 
      sg_Window_2_HWND[ret_win]     = hwnd; 

		return ret_win; 
   } 


   virtual void Poll_input (sy::Mouse_state&, sy::Keyboard_state&) 
   {
   
   }

	//
	//
	OIS::InputManager* Create_input_system	(sy::Window* win) 
	{
		//if (sg_Window_2_HWND.count (win))
		//	return OIS::InputManager::createInputSystem ( (size_t) sg_Window_2_HWND[win]); 

		return 0; 
	}

	// DispatchEvents 
	bool DispatchEvents () {
		MSG msg; 
		
		if (::PeekMessage (&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage (&msg); 

			::DispatchMessage (&msg); 

			return msg.message != WM_QUIT; 
		}

		return TRUE;
		}

}; 


sy::System_context* xp::Create_Win32_system (HINSTANCE inst) 
{
	return new Win32_system (inst); 
}



//
//
HWND Create_HWND_default (HINSTANCE hInstance, const char* title, int nCmdShow) {

	HWND hWnd = ::CreateWindow (
		sg_DefaultWindowClass, 
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		NULL, 
		NULL, 
		hInstance, 
		NULL
		);

	//
	if (!hWnd)
	   return 0;

	return hWnd;
	}

//
//
LRESULT CALLBACK Win32_WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	sy::Window_listener* listener = 0; 
	
	if (sg_HWND_2_Listener.count (hWnd))
		listener = sg_HWND_2_Listener[hWnd]; 


	switch (message)
	{
	// WM_COMMAND
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
//		case IDM_EXIT:
//			DestroyWindow(hWnd);
//			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	// WM_SIZE
	case WM_SIZE: 
		if (listener) 
			listener->OnWindowResize (LOWORD (lParam), HIWORD (lParam)); 
	break;

	// WM_CREATE
	case WM_CREATE:
		//PostQuitMessage(0);
		break;

	//case WM_PAINT:
	//	hdc = BeginPaint(hWnd, &ps);
	//	// TODO: Add any drawing code here...
	//	EndPaint(hWnd, &ps);
	//	break;

	// WM_DESTROY
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	// WM_ACTIVATE
	case WM_ACTIVATE: 
		if (listener)
			listener->OnWindowActivate (wParam ? true : false); 
		break; 

	// WM_CLOSE
	case WM_CLOSE: 

		if (listener) 
			listener->OnWindowClose (); 

		PostQuitMessage (0); 
		break; 
	//
	// default
	default:
		return DefWindowProc (hWnd, message, wParam, lParam);
	}
	return 0;
}






// 
void xp::Update_stand_idle (); 

