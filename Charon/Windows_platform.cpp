#include "Windows_platform.h"

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
   Win32_GL_window(
      HWND hwnd,
      HDC hdc,
      const std::string& title)
      : m_hwnd(hwnd)
      , m_DC(hdc)
      , m_hglrc(0) {
      //
      int iPxlFmt = ::ChoosePixelFormat(
         m_DC,
         &sg_Default_PIXELFORMATDESCRIPTOR
         );

      if (iPxlFmt) {
         BOOL res = ::SetPixelFormat(
            m_DC,
            iPxlFmt,
            &sg_Default_PIXELFORMATDESCRIPTOR
            );

         m_hglrc = ::wglCreateContext(m_DC);
         res = ::wglMakeCurrent(m_DC, m_hglrc);
      }

   }

   //
   //
   virtual ~Win32_GL_window()
   {

   }

   //
   //
   void Swap_buffers() {
      ::SwapBuffers(m_DC);
   }

   //
   //
   int setup_display() {
      return 0;
   }

   //
   //
   HWND Get_HWND() {
      return m_hwnd;
   }

   HDC Get_HDC() {
      return m_DC;
   }
   //
   //
   virtual size_t Get_WindowID() {
      return (size_t)m_hwnd;
   };
   //
   //
   virtual void Show(bool show) {
      ::ShowWindow(m_hwnd, show ? SW_SHOW : SW_HIDE);
      ::UpdateWindow(m_hwnd);
   }
   //
   //
   virtual void SetPos(int x, int y) {
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
   GL_DisplBuff(HDC hdc) : m_HDC(hdc), m_hglrc(0) {
      //
      int iPxlFmt = ::ChoosePixelFormat(
         m_HDC,
         &sg_Default_PIXELFORMATDESCRIPTOR
         );

      if (iPxlFmt) {
         BOOL res = ::SetPixelFormat(
            m_HDC,
            iPxlFmt,
            &sg_Default_PIXELFORMATDESCRIPTOR
            );

         m_hglrc = ::wglCreateContext(m_HDC);
         res = ::wglMakeCurrent(m_HDC, m_hglrc);
      }
   }

   //
   ~GL_DisplBuff() {
   }

   //
   void Swap_buffers() {
      ::SwapBuffers(m_HDC);
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
   Win32_window(HWND hwnd, HDC hdc, const char* title)
      : m_hwnd(hwnd), m_DC(0) {
   }

   //
   ~Win32_window() {
      // HINSTANCE hInst, const char* title	
   }

   //
   HWND Get_HWND() {
      return m_hwnd;
   }

   HDC Get_HDC() {
      return m_DC;
   }
   //
   //
   virtual size_t Get_WindowID() {
      return (size_t)m_hwnd;
   };


   //
   virtual void Show(bool show) {
      ::ShowWindow(m_hwnd, show ? SW_SHOW : SW_HIDE);
      ::UpdateWindow(m_hwnd);
   }


   //
   virtual void SetPos(int x, int y) {
      //::SetWindowPos (m_hwnd, 0, 
   }

};



ATOM Default_WindowClass(HINSTANCE hInstance) {
   WNDCLASSEX wcex;


   wcex.cbSize = sizeof(WNDCLASSEX);
   wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
   wcex.lpfnWndProc = Win32_WndProc;
   wcex.cbClsExtra = 0;
   wcex.cbWndExtra = 0;
   wcex.hInstance = hInstance;
   wcex.hIcon = LoadIcon(hInstance, 0);		//MAKEINTRESOURCE(IDI_HORDEINTGRTEST));
   wcex.hCursor = LoadCursor(NULL, IDC_ARROW);	//
   wcex.hbrBackground = NULL;							// (HBRUSH)(COLOR_WINDOW+1);
   wcex.lpszMenuName = NULL;							// MAKEINTRESOURCE(IDC_HORDEINTGRTEST);
   wcex.lpszClassName = sg_DefaultWindowClass;				//
   wcex.hIconSm = LoadIcon(wcex.hInstance, 0);	// MAKEINTRESOURCE(IDI_SMALL));
   return RegisterClassEx(&wcex);
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
   Win32_system(HINSTANCE hInstance)
      : appInstance(hInstance)
      , wndClassAtom(0)
   {
      wndClassAtom = Default_WindowClass(hInstance);


      size_t sizeOf_LARGE_INTEGER = sizeof(LARGE_INTEGER);

      ::QueryPerformanceFrequency(&perf_freq);

      dTimeStep = 1.0 / (double)perf_freq.QuadPart;

      ResetTime();
   }

   // Win32SystemContext 
   ~Win32_system() {

   }



   // ResetTime 
   virtual	void ResetTime() {
      ::QueryPerformanceCounter(&start_time);
      ::QueryPerformanceCounter(&prev_time);
   }

   // GetTimeElapsed
   virtual	double GetTimeElapsed() {
      LARGE_INTEGER curr_time;

      ::QueryPerformanceCounter(&curr_time);

      return (curr_time.QuadPart * dTimeStep);
   }
   // GetDt 
   virtual	double  GetDt() {
      LARGE_INTEGER curr_time;

      ::QueryPerformanceCounter(&curr_time);

      long long ticks = curr_time.QuadPart - prev_time.QuadPart;
      prev_time = curr_time;

      return (ticks * dTimeStep);
   }

   virtual  unsigned long long   GetTimerTicks() { return 0; }
   virtual  unsigned long long   GetTimerFrequencyHz() { return 0; }


   // Request something
   virtual void SysReq(sy::System_context::Request req)
   {

      switch (req) {

      case sy::System_context::Req_Exit:
         PostQuitMessage(0);
         break;

      default:
         break;

      }
   }

   // Create_GL_Display
   virtual sy::Graphics_display* Create_display(sy::Window* w)
   {
      if (sg_Window_HDC.count(w))
         return new GL_DisplBuff(sg_Window_HDC[w]);

      return 0;
   }

   // CreateOpengGLWindow 
   virtual sy::Window*	Create_window(
      sy::Window_listener*  lstnr,
      const char*			   title)
   {
      HWND	hwnd = Create_HWND_default(appInstance, title, SW_SHOW);
      HDC		hdc = ::GetDC(hwnd);

      Win32_window* winWin = new Win32_window(hwnd, hdc, title);

      sg_HWND_2_Listener[hwnd] = lstnr;
      sg_Window_HDC[winWin] = hdc;
      sg_Window_2_HWND[winWin] = hwnd;

      return winWin;
   }


   // supercede GL_display + Window
   virtual sy::Graphics_window* Create_GraphicsWindow(
      sy::Window_listener*  listening,
      const char*          title,
      int wd,
      int ht,
      bool fullscreen
      )
   {
      HWND  hwnd = Create_HWND_default(appInstance, title, SW_SHOW);
      HDC	hdc = ::GetDC(hwnd);

      sy::Graphics_window* ret_win = new Win32_GL_window(hwnd, hdc, title);
      sg_HWND_2_Listener[hwnd] = listening;
      sg_Window_2_HWND[ret_win] = hwnd;

      return ret_win;
   }


   virtual void Poll_input(sy::Mouse_state&, sy::Keyboard_state&)
   {

   }

   //
   //
   OIS::InputManager* Create_input_system(sy::Window* win)
   {
      //if (sg_Window_2_HWND.count (win))
      //	return OIS::InputManager::createInputSystem ( (size_t) sg_Window_2_HWND[win]); 

      return 0;
   }

   // DispatchEvents 
   bool DispatchEvents() {
      MSG msg;

      if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
      {
         ::TranslateMessage(&msg);

         ::DispatchMessage(&msg);

         return msg.message != WM_QUIT;
      }

      return TRUE;
   }

};


sy::System_context* xp::Create_Win32_system(HINSTANCE inst)
{
   return new Win32_system(inst);
}

