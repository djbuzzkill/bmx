#ifndef SPUX_SYSTEM_INCLUDE
#define SPUX_SYSTEM_INCLUDE


#include "Shared.h"

//
// namespace xups
// namespace uxps
// namespace uxsp
// namespace puxs
// namespace psux
// namespace suxp
// namespace sxup
// namespace supx
//
namespace spux
{
	struct Window ;          
	struct DisplaySurface;    
	struct Runnable;    
	struct System;
   struct WindowListener;   
   struct GraphicsWindow;   

   //
   struct Mouse_state { 
      // 2d + scroll
      int xRel, yRel, zRel;  
      int xAbs, yAbs, zAbs;  

      // left, middle, right
      int butt[3]; 
      }; 

   //         
   struct Keyboard_state { 
      const unsigned char* scan_code; 
      bool                 mod_Ctrl;
      bool                 mod_Alt ;
      bool                 mod_Shift; 
      }; 

   //
   enum KB_scan_code {

      SC_A = 4,
      SC_B = 5,
      SC_C = 6,
      SC_D = 7,
      SC_E = 8,
      SC_F = 9,
      SC_G = 10,
      SC_H = 11,
      SC_I = 12,
      SC_J = 13,
      SC_K = 14,
      SC_L = 15,
      SC_M = 16,
      SC_N = 17,
      SC_O = 18,
      SC_P = 19,
      SC_Q = 20,
      SC_R = 21,
      SC_S = 22,
      SC_T = 23,
      SC_U = 24,
      SC_V = 25,
      SC_W = 26,
      SC_X = 27,
      SC_Y = 28,
      SC_Z = 29,
      SC_1 = 30,
      SC_2 = 31,
      SC_3 = 32,
      SC_4 = 33,
      SC_5 = 34,
      SC_6 = 35,
      SC_7 = 36,
      SC_8 = 37,
      SC_9 = 38,
      SC_0 = 39,
      SC_RETURN = 40,
      SC_ESCAPE = 41,
      SC_BACKSPACE = 42,
      SC_TAB = 43,
      SC_SPACE = 44,
      SC_MINUS = 45,
      SC_EQUALS = 46,
      SC_LEFTBRACKET = 47,
      SC_RIGHTBRACKET = 48,
      SC_BACKSLASH = 49, 
      SC_NONUSHASH = 50, 
      SC_SEMICOLON = 51,
      SC_APOSTROPHE = 52,
      SC_TILDE = 53, 
      SC_COMMA = 54,
      SC_PERIOD = 55,
      SC_SLASH = 56,
      SC_CAPSLOCK = 57,
      SC_F1 = 58,
      SC_F2 = 59,
      SC_F3 = 60,
      SC_F4 = 61,
      SC_F5 = 62,
      SC_F6 = 63,
      SC_F7 = 64,
      SC_F8 = 65,
      SC_F9 = 66,
      SC_F10 = 67,
      SC_F11 = 68,
      SC_F12 = 69,
      SC_PRINTSCREEN = 70,
      SC_SCROLLLOCK = 71,
      SC_PAUSE = 72,
      SC_INSERT = 73, 
      SC_HOME = 74,
      SC_PAGEUP = 75,
      SC_DELETE = 76,
      SC_END = 77,
      SC_PAGEDOWN = 78,
      SC_RIGHT = 79,
      SC_LEFT = 80,
      SC_DOWN = 81,
      SC_UP = 82,
      SC_NUMLOCKCLEAR = 83, 
      SC_KP_DIVIDE = 84,
      SC_KP_MULTIPLY = 85,
      SC_KP_MINUS = 86,
      SC_KP_PLUS = 87,
      SC_KP_ENTER = 88,
      SC_KP_1 = 89,
      SC_KP_2 = 90,
      SC_KP_3 = 91,
      SC_KP_4 = 92,
      SC_KP_5 = 93,
      SC_KP_6 = 94,
      SC_KP_7 = 95,
      SC_KP_8 = 96,
      SC_KP_9 = 97,
      SC_KP_0 = 98,
      SC_KP_PERIOD = 99,
      SC_NONUSBACKSLASH = 100, 
      SC_APPLICATION = 101,    
   }; 

   //
   //
   inline bool Is_keydown (KB_scan_code ky, const Keyboard_state& kb)
   {
      return kb.scan_code[ky ] ? true : false; 
   }


   // Time_tracker 
   struct TimeTracker {

      typedef unsigned long long u64;

      u64      start_ticks   ;
      u64      freq_ticks    ;
      u64      prev_ticks    ; 
      u64      acc_ticks; 
      double   freq_Hz; 

      //
      bool  Reset (System* sys);

      //
      double GetDt (System* sys);

   }; 

	//
	// WindowListener
	struct WindowListener {
	protected: 
		WindowListener () {
			}
	public: 

		virtual void OnWindowResize		(int wd, int ht) = 0; 
		virtual void OnWindowClose		() = 0; 
		virtual void OnWindowActivate	(bool activate) = 0; 
	}; 

	//
	// AppWindow 
	struct Window 
		{
	public: 

      enum { Default_X_Pos = 128 }; 
      enum { Default_Y_Pos = 128 }; 

      enum { Default_Width = 1024}; 
      enum { Default_Height= 768 }; 


		//
		// funcs
		virtual size_t	Get_WindowID () = 0; 
		virtual void	Show	 (bool show)		= 0; 
		virtual void	SetPos (int x, int y)	= 0; 
		virtual void	Resize (int x, int y)	= 0; 

	protected: 
		Window () { 
		}
	}; 

	//
	// DeviceSurface - wat u see
	struct DisplaySurface 
		{
	public: 

		virtual bool		SetupDisplay () = 0; 
		virtual void	Present () = 0; 

	protected:

		DisplaySurface () { 
			}
		}; 


   //
   // GraphicsWindow - like an HWND
   struct GraphicsWindow : public Window, public DisplaySurface
      {
         static const size_t FULLSCREEN_BIT = (0x1 << 0);  
         static const size_t OPENGL_BIT     = (0x1 << 1); 
         static const size_t VULKAN_BIT     = (0x1 << 2); 

   protected: 

      GraphicsWindow () {}
      
      }; 

   //
   //
	struct System 
		{ 
	protected: 

	   enum Request : size_t {
		   Exit = 0,  
		   }; 


		System () { 
			}

	public: 

      // deprecated by GraphicsWindow
      virtual Ref<GraphicsWindow>::Type CreateGraphicsWindow   (Ref<WindowListener>::Type, const std::string& title, int x, int y, int wd, int ht, size_t flags) = 0; 
      virtual void                           PollInput         (Mouse_state& m, Keyboard_state& k) = 0;  

		virtual bool	   DispatchEvents	() = 0; 

      // timer
      virtual  unsigned long long   GetTimerTicks        () = 0; 
      virtual  unsigned long long   GetTimerFrequencyHz  () = 0;

		// Request_system ();
		virtual void	   SysReq			(Request req) = 0; 
		}; 



   //
   //

   Ref<System>::Type SystemContext(); 
   //

   }
   
   #endif
