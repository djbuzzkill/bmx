
#include "System.h"
#include "Task.h"


using namespace spux;



//
bool TimeTracker::Reset (System* sys)
{
   if (freq_ticks = sys->GetTimerFrequencyHz ())
   {
      freq_Hz     = 1.0 / (freq_ticks); 
      prev_ticks  = sys->GetTimerTicks ();
      return true;
   }

   return false; 
}

      //
double TimeTracker::GetDt (System* sys)
{
   acc_ticks = sys->GetTimerTicks (); 
   u64 elapsed_ = acc_ticks - prev_ticks;
   prev_ticks  = acc_ticks; 
   return elapsed_ * freq_Hz; 
}

