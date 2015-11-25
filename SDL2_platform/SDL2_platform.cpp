#include "SDL2_platform.h"

#include <map>
#include <string>

#include <SDL.h>
#include <SDL_opengl.h>


#include "Dx/System.h"
#include "Dx/VecMath.h"
#include "Dx/Render.h"

//   SDL_window (const std::string& title, sy::Window_listener* ls) 

const int k_GL_Major_version (4); 
const int k_GL_Minor_version (2);

// C:\usr\glm
unsigned int sy::Window::kDef_window_X_pos   (128); 
unsigned int sy::Window::kDef_window_Y_pos   (128); 

unsigned int sy::Window::kDef_windowed_width    (1024); 
unsigned int sy::Window::kDef_windowed_height   (768); 

unsigned int sy::Graphics_window::kDef_windowed_width    = sy::Window::kDef_windowed_width ; 
unsigned int sy::Graphics_window::kDef_windowed_height   = sy::Window::kDef_windowed_height; 

class SDL_window : public sy::Graphics_window, public cx::Destructor
{
   // | SDL_WINDOW_FULLSCREEN
   // | SDL_WINDOW_MINIMIZED 
   // | SDL_WINDOW_INPUT_GRABBED; 

public:    

   static const uint32_t 
   kSDL_default_flags   = SDL_WINDOW_OPENGL  
                        | SDL_WINDOW_RESIZABLE;

   std::shared_ptr<SDL_Window> win_; 
   SDL_GLContext        gl_;
   //
   //
   SDL_window (const std::string& title, sy::Window_listener* ls, int wd, int ht, uint32_t flags_) 
      : win_ (SDL_CreateWindow (title.c_str(), sy::Window::kDef_window_X_pos, sy::Window::kDef_window_Y_pos, wd, ht, flags_), SDL_DestroyWindow) 
   {
      //SDL_Rect rect; 
      //SDL_GetDisplayBounds (0, &rect); 
      SDL_assert (win_); 
      setup_display (); 
   }

   //
   virtual ~SDL_window () 
   {
      SDL_GL_DeleteContext (gl_); 
   }

	// funcs
	virtual size_t	Get_WindowID ()
   {
      return (size_t) win_.get(); 
   } 

   //
   virtual void Show	(bool show)
   {
   (show ? SDL_ShowWindow : SDL_HideWindow) (win_.get()); 
   }

	//
   virtual void SetPos (int x, int y)
   {
      SDL_SetWindowPosition (win_.get(), x, y); 
   }

   //
	virtual int setup_display ()
   {
      gl_ = ::SDL_GL_CreateContext(win_.get());

      ::SDL_GL_MakeCurrent (win_.get(), gl_); 
      ::SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, k_GL_Major_version);
      ::SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, k_GL_Minor_version);
      ::SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
      ::SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
      ::SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

      int supportec_extensions[] = { 

         ::SDL_GL_ExtensionSupported ("not_an_extension"), 
         ::SDL_GL_ExtensionSupported ("GL_ARB_tessellation_shader"), 
         ::SDL_GL_ExtensionSupported ("GL_EXT_direct_state_access"), 
         ::SDL_GL_ExtensionSupported ("GL_ARB_viewport_array"), 
         }; 

      if (gl_)
         return 0;       

      return -1; 
   }
	
   virtual void Swap_buffers ()
   {
      ::SDL_GL_SwapWindow (win_.get()); 
   }
}; 

// const uint32_t SDL_window::kSDL_window_flags = 0; 

//
//
class SDL_system 
   : public sy::System_context 
   , public cx::Destructor 
{ 
   
   static const Uint32 
   kSDL_init_flags   = SDL_INIT_AUDIO 
                     | SDL_INIT_VIDEO 
                     | SDL_INIT_EVENTS 
                     | SDL_INIT_TIMER;

   bool is_quit ;   // 
   std::map<SDL_Window*, sy::Window_listener*> map_window_2_listener; 

public: 
   
   // 
   SDL_system () : is_quit  (false) {

         SDL_Init (kSDL_init_flags); 

      }

   virtual ~SDL_system () {
      SDL_Quit (); 
   }
   // deprecated by GraphicsWindow
	virtual sy::Window* Create_window (sy::Window_listener* ls, const char* title)
   {
      return 0; 
   } 


   //
   // deprecated by GraphicsWindow
	virtual sy::Graphics_display* Create_display (sy::Window*)
      {
      return 0; 
      }

    
   // new hot
   virtual sy::Graphics_window* Create_GraphicsWindow (
      sy::Window_listener*  ls, 
      const char*          title,
      int                  wd, 
      int                  ht, 
      bool                 fullscreen)
   {

      std::string s_title = title; 

      Uint32 flags_ = SDL_window::kSDL_default_flags;
      if (fullscreen)
         flags_ |= SDL_WINDOW_FULLSCREEN;
      
      SDL_window* syswnd_ = new SDL_window (s_title, ls, wd, ht, flags_); 

      map_window_2_listener[syswnd_->win_.get()] = ls; 

      return syswnd_ ; 
   }

   // 
   // do want : Poll_input ^ Create_input_system 
   virtual void Poll_input (sy::Mouse_state& m, sy::Keyboard_state& k)
      {
         // mouse
         Uint32 butt_bits = SDL_GetMouseState (&m.xAbs, &m.yAbs); 
         SDL_GetRelativeMouseState (&m.xRel, &m.yRel); 

         m.butt[0] = butt_bits & SDL_BUTTON (1);
         m.butt[1] = butt_bits & SDL_BUTTON (2);
         m.butt[2] = butt_bits & SDL_BUTTON (3);

         // keyboard
         int            num_keyz; 
         const Uint8*   codez = SDL_GetKeyboardState (&num_keyz); 
         //SDL_Keycode
         k.scan_code = codez ;
         k.mod_Shift = (codez[SDL_SCANCODE_RSHIFT] | codez[SDL_SCANCODE_LSHIFT]) ? true : false;
         k.mod_Alt   = (codez[SDL_SCANCODE_LALT] | codez[SDL_SCANCODE_RALT]    ) ? true : false;
         k.mod_Ctrl  = (codez[SDL_SCANCODE_LCTRL] | codez[SDL_SCANCODE_RCTRL]  ) ? true : false;
         // SDL_assert (0); // WE'RE NOT DONE HERE
         //dez[SDL_SCANCODE_0]
      }  
   
	// should be called Dispatch_WindowEvents ()
	virtual bool DispatchEvents ()
   {
      SDL_Event event; 
      bool ret_res = !is_quit; 
      int i= 0; 
      while (SDL_TRUE == SDL_PollEvent (&event))
         switch (event.type) 
      {
      // SDL_QUIT
      case SDL_QUIT: 
         ret_res = false;
         break; 

      // SDL_WINDOWEVENT
      case SDL_WINDOWEVENT: 
         if (sy::Window_listener* ls = map_window_2_listener[SDL_GetWindowFromID (event.window.windowID)] )
            switch (event.window.event)
         {
         case SDL_WINDOWEVENT_CLOSE: 
            ls->OnWindowClose (); 
            break; 

         case SDL_WINDOWEVENT_RESIZED: 
            // OnWindowResize put in SDL_WINDOWEVENT_SIZE_CHANGED instead
            break; 

         case SDL_WINDOWEVENT_FOCUS_GAINED: 
               ls->OnWindowActivate (true); 
            break; 
         case SDL_WINDOWEVENT_FOCUS_LOST:
               ls->OnWindowActivate (false); 
            break; 
         case SDL_WINDOWEVENT_MINIMIZED:  
            i++; 
            break;
         case SDL_WINDOWEVENT_SIZE_CHANGED:  
            ls->OnWindowResize(event.window.data1, event.window.data2); 
            break;

         case SDL_WINDOWEVENT_SHOWN:  
            i++; 
            break;
         case SDL_WINDOWEVENT_HIDDEN:  
            i++; 
            break;
         case SDL_WINDOWEVENT_EXPOSED:  
            i++; 
            break;
         }
         break; 

      // SDL_USEREVENT
      case SDL_USEREVENT:
         // wat
         break;  
      }
         
      return ret_res ; 
   } 

   //
   //


   virtual unsigned long long GetTimerTicks        () { return ::SDL_GetPerformanceCounter (); }
   virtual unsigned long long GetTimerFrequencyHz  () { return ::SDL_GetPerformanceFrequency(); }

	// Request_system ();
	virtual void SysReq (sy::SystemRequest req)
   {
      SDL_Event event; 
		switch (req) 
      {
      case sy::SR_Exit:
            is_quit  = true; 
            event.type = SDL_QUIT; 
				SDL_PushEvent (&event); 
				break;
      default: 
		   break;
      }
   } 

}; 

 
sy::System_context* sy::Create_context ()
{
   return new SDL_system; 
}




//
// ProjectionState - load perspective GL projection 
void Rn::ProjectionState (float fFoV, float fAsp, float fNear, float fFar)
{
	Ma::Mat44f proj_Mat;

	Ma::PerspectiveFoV_RHS (
		proj_Mat, 
		fFoV, fAsp, 
		fNear, fFar
		); 

	// Ma::Transpose (proj_Mat); 

	glMatrixMode (GL_PROJECTION); 
	glLoadMatrixf (proj_Mat.ptr()); 
}

//
// ModelViewMatrix - build model view matrix
Ma::Mat44f& Rn::ModelViewMatrix (
	Ma::Mat44f& out, 
	const Ma::Vec3f& mod_Pos, 
	const Ma::Quatf& mod_Rot, 
	const Ma::Vec3f& view_Pos, 
	const Ma::Quatf& view_Rot)
{
	//
	Ma::Vec3f	mv_Pos, v_Tmp; 
	Ma::Mat33f	inv_Mat; 
	Ma::Quatf	mv_Rot, view_InvRot; 

	Ma::Sub (v_Tmp, mod_Pos, view_Pos); 
	
	
	Ma::QuatInverse (view_InvRot, view_Rot);
	Ma::SetMat33Quat (inv_Mat, view_InvRot); 

	Ma::Mult (mv_Pos, inv_Mat, v_Tmp); 
	Ma::Mult (mv_Rot, view_InvRot, mod_Rot); 

	Ma::SetMat44Quat (out, mv_Rot); 
	Ma::AddMat44Transl (out, mv_Pos); 
	return out; 
}

Ma::Mat44f& Rn::ModelViewMatrix (
	Ma::Mat44f& out, 
	const Ma::Vec3f& mod_Pos, 
	const Ma::Mat33f& mod_Rot, 
	const Ma::Vec3f& view_Pos, 
	const Ma::Quatf& view_Rot)
{

	//
	Ma::Vec3f	mv_Pos, v_Tmp; 
	Ma::Mat33f	inv_Mat, mv_Rot; 
	Ma::Quatf	view_InvRot; 

	Ma::Sub (v_Tmp, mod_Pos, view_Pos); 
	
	
	Ma::QuatInverse  (view_InvRot, view_Rot);
	Ma::SetMat33Quat (inv_Mat, view_InvRot); 

	Ma::Mult (mv_Pos, inv_Mat, v_Tmp); 
	//Ma::Mult (mv_Rot, view_InvRot, mod_Rot); 
	Ma::MultMat (mv_Rot, inv_Mat, mod_Rot); 


	Ma::SetMat44 (out, 
		mv_Rot[0][0], mv_Rot[0][1],	mv_Rot[0][2], 0.0f,
		mv_Rot[1][0], mv_Rot[1][1],	mv_Rot[1][2], 0.0f,
		mv_Rot[2][0], mv_Rot[2][1],	mv_Rot[2][2], 0.0f,
		0.0f,		0.0f,		0.0f,		1.0f
		); 

	Ma::AddMat44Transl (out, mv_Pos); 


	return out; 
}

//
// ModelViewState - load model GL model view matrix 
void Rn::ModelViewState (
	const Ma::Vec3f& mod_Pos, 
	const Ma::Quatf& mod_Rot, 
	const Ma::Vec3f& view_Pos, 
	const Ma::Quatf& view_Rot)
{
	
   Ma::Mat44f mv_Mat; 
	
	ModelViewMatrix (
		mv_Mat, 
		mod_Pos,
		mod_Rot,
		view_Pos,
		view_Rot
		); 

	Ma::Transpose (mv_Mat); 
	glMatrixMode (GL_MODELVIEW); 
	glLoadMatrixf (mv_Mat.ptr()); 
}

//
// ModelViewState - load model GL model view matrix 
void Rn::ModelViewState (
	const Ma::Vec3f& mod_Pos, 
	const Ma::Mat33f& mod_Rot, 
	const Ma::Vec3f& view_Pos, 
	const Ma::Quatf& view_Rot)
{
	Ma::Mat44f mv_Mat; 
	
	ModelViewMatrix (
		mv_Mat, 
		mod_Pos,
		mod_Rot,
		view_Pos,
		view_Rot
		); 

		
	Ma::Transpose (mv_Mat); 
	glMatrixMode (GL_MODELVIEW); 
	glLoadMatrixf (mv_Mat.ptr()); 
}


//
//
void Rn::Draw_AA_grid ( 
	float		size, 
	Ma::Quatf&	view_Rot, 
	Ma::Vec3f&	view_Pos)
{

	Ma::Quatf model_Rot; 
	Ma::Vec3f model_Pos; 

	Ma::SetIdent (model_Rot); 
	Ma::Set (model_Pos, 0.0f, 0.0f, 0.0f); 


	glDisable (GL_TEXTURE_2D); 
	glDisableClientState (GL_VERTEX_ARRAY); 
	glDisableClientState (GL_NORMAL_ARRAY); 
	glDisableClientState (GL_TEXTURE_COORD_ARRAY); 


	Rn::ModelViewState (
		model_Pos,
		model_Rot, 
		view_Pos, 
		view_Rot); 



	glBegin (GL_LINES); 



	glColor3f (0.1f, 0.1f, 0.2f); 
	for (size_t i = 1; i <= 32; i++)
	{

		float step = (i/32.0f); 

		glVertex3f (step * size, 0.0f, -size); 
		glVertex3f (step * size, 0.0f, size); 
	
		glVertex3f (-step * size, 0.0f, -size); 
		glVertex3f (-step * size, 0.0f, size); 


		glVertex3f (-size , 0.0f, step  * size); 
		glVertex3f (size  , 0.0f, step  * size); 

		glVertex3f (-size , 0.0f, -step * size); 
		glVertex3f (size  , 0.0f, -step * size); 

	}


	glColor3f (1.0f, 0.2f, 0.2f); 
	glVertex3f (-size, 0.0f, 0.0f); 
	glVertex3f (size, 0.0f, 0.0f); 

	glColor3f (0.2f, 1.0f, 0.2f); 
	glVertex3f (0.0, -size, 0.0f); 
	glVertex3f (0.0, size, 0.0f); 

	glColor3f (0.2f, 0.2f, 1.0f); 
	glVertex3f (0.0, 0.0f, -size); 
	glVertex3f (0.0, 0.0f, size); 



	glEnd (); 


}


//
//
void Rn::Draw_axes (
	float size, 
   Ma::Matrix<4, 4, float>& mv)

{

	glDisable (GL_TEXTURE_2D); 
	glDisableClientState (GL_VERTEX_ARRAY); 
	glDisableClientState (GL_NORMAL_ARRAY); 
	glDisableClientState (GL_TEXTURE_COORD_ARRAY); 

	glEnable (GL_LINE_WIDTH); 
	glLineWidth (1.0f); 


	glMatrixMode (GL_MODELVIEW); 
	glLoadMatrixf (mv.ptr()); 

	glBegin (GL_LINES); 

	glColor3f (1.0f, 0.2f, 0.2f); 
	glVertex3f (0.0, 0.0f, 0.0f); 
	glVertex3f (size, 0.0f, 0.0f); 


	glColor3f (0.2f, 1.0f, 0.2f); 
	glVertex3f (0.0, 0.0f, 0.0f); 
	glVertex3f (0.0, size, 0.0f); 

	glColor3f (0.2f, 0.2f, 1.0f); 
	glVertex3f (0.0, 0.0f, 0.0f); 
	glVertex3f (0.0, 0.0f, size); 

	glEnd (); 

	glLineWidth (1.0f); 
	glDisable (GL_LINE_WIDTH); 
}


//
//
void Rn::Draw_line (
	float             size, 
   const Ma::Vec3f&  col, 
   const Ma::Vec3f&  a, 
   const Ma::Vec3f&  b, 
	Ma::Quatf&        view_Rot, 
   Ma::Vec3f&        view_Pos)
{
   using namespace Ma;


   Ma::Vec3f model_Pos; 
   Ma::Quatf model_Rot;


	glDisable (GL_TEXTURE_2D); 
	glDisableClientState (GL_VERTEX_ARRAY); 
	glDisableClientState (GL_NORMAL_ARRAY); 
	glDisableClientState (GL_TEXTURE_COORD_ARRAY); 

	glEnable (GL_LINE_WIDTH); 
	glLineWidth (size); 

	Rn::ModelViewState (
		Set (model_Pos, 0.0f, 0.0f, 0.0f),
		SetIdent (model_Rot), 
		view_Pos, 
		view_Rot); 

	glBegin (GL_LINES); 

	glColor3fv (col.ptr()); 
	glVertex3fv (a.ptr()); 
	glVertex3fv (b.ptr()); 

	glEnd (); 

	glLineWidth (1.0f); 
	glDisable (GL_LINE_WIDTH); 
}


//
//
void Rn::Draw_sphere (
   float size, 
   Ma::Vec3f& col3f, 
   Ma::Vec3f& model_Pos, 
	Ma::Quatf& view_Rot, 
   Ma::Vec3f& view_Pos)
{

	glDisable (GL_TEXTURE_2D); 
	glDisableClientState (GL_VERTEX_ARRAY); 
	glDisableClientState (GL_NORMAL_ARRAY); 
	glDisableClientState (GL_TEXTURE_COORD_ARRAY); 

	glEnable (GL_LINE_WIDTH); 
	glLineWidth (1.0f); 


   Ma::Quatf model_Rot; 
   

	Rn::ModelViewState (
		model_Pos,
		Ma::SetIdent (model_Rot), 
		view_Pos, 
		view_Rot); 


   Ma::Vec3f p; 

   const float dPhi = Ma::Pi / 12; 
   const float dTheta = Ma::Pi / 12; 

	glColor3fv (col3f.ptr ()); 

   for (int i_phi = 0; i_phi < 6; i_phi++)
      {

	glBegin (GL_LINE_LOOP); 
      // top half
      for (int i_theta = 0; i_theta < 24; i_theta++ ) {
         Ma::Spherical  (p,  i_theta * dTheta, i_phi * dPhi);  
         Ma::Scale      (p, size); 
         glVertex3f     (Ma::X(p), Ma::Y(p), Ma::Z(p)); 
         }
	glEnd (); 

   glBegin (GL_LINE_LOOP); 
      // bottom
      for (int i_theta = 0; i_theta < 24; i_theta++ ) {
         Ma::Spherical  (p,  i_theta * dTheta, i_phi * dPhi);  
         Ma::Scale      (p, size); 
         glVertex3f     (Ma::X(p), -Ma::Y(p), Ma::Z(p)); 
         }

   glEnd (); 
      }
       

	glLineWidth (1.0f); 
	glDisable (GL_LINE_WIDTH); 


}





//
void Rn::Draw_axes (
	float size, 
	Ma::Quatf& model_Rot, Ma::Vec3f& model_Pos, 
	Ma::Quatf& view_Rot, Ma::Vec3f& view_Pos)
{


	glDisable (GL_TEXTURE_2D); 
	glDisableClientState (GL_VERTEX_ARRAY); 
	glDisableClientState (GL_NORMAL_ARRAY); 
	glDisableClientState (GL_TEXTURE_COORD_ARRAY); 

	glEnable (GL_LINE_WIDTH); 
	glLineWidth (1.0f); 

	Rn::ModelViewState (
		model_Pos,
		model_Rot, 
		view_Pos, 
		view_Rot); 

	glBegin (GL_LINES); 

	glColor3f (1.0f, 0.2f, 0.2f); 
	glVertex3f (0.0, 0.0f, 0.0f); 
	glVertex3f (size, 0.0f, 0.0f); 


	glColor3f (0.2f, 1.0f, 0.2f); 
	glVertex3f (0.0, 0.0f, 0.0f); 
	glVertex3f (0.0, size, 0.0f); 

	glColor3f (0.2f, 0.2f, 1.0f); 
	glVertex3f (0.0, 0.0f, 0.0f); 
	glVertex3f (0.0, 0.0f, size); 

	glEnd (); 

	glLineWidth (1.0f); 
	glDisable (GL_LINE_WIDTH); 


}

