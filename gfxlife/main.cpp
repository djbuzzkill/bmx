
#include "gfxlife.h"
#include "spux.h"

#define MULTI_IMAGE_NITF


using namespace spux;

//
//
//typedef std::function<int(const std::vector<std::string>&)> TestFunc;
typedef std::function<Ref<Runnable>::Type(const std::vector<std::string>&)> TestFunc;

   
struct runnablestruc {
   bool              enable; 
   TestFunc          fn; 
   std::string       txt;    
   };



//namespace gfx00 { int run (const std::vector<std::string>& args); }
//namespace vkx00 { int run (const std::vector<std::string>& args); }


Ref<Runnable>::Type create_vkx00 (const std::vector<std::string>& args); 

//
//
const std::vector<runnablestruc> tests = 
{
   { true, create_vkx00 , "" },
};


//
//
int main (int argc, const char** argv)
{

   int res = 0;
   //
   std::vector<std::string> args (argv, argv + argc);


   Ref<System>::Type sys; // = spux::SystemContext (); 


   for (size_t i = 0; i < tests.size(); ++i)
   {
      if (tests[i].enable)
      {
         if (Ref<Runnable>::Type task = tests[i].fn (args))
         {
            task->Initialize (sys); 
            task->Run  (sys, 0.0); 
            task->Deinitialize (sys); 
         }
      }
      else
      {
         tests[i].txt; 
      }

   }

  return res; 
}

