#ifndef SPUX_SHARED_INCLUDE
#define SPUX_SHARED_INCLUDE

#include<string>
#include<memory>
#include<cassert>


namespace spux 
{

   //
   //
   template<typename Ty>
   inline bool range_check_incl (Ty mn, Ty mx, Ty val)
      {
   return (mn <= val && val <= mx); 
      }

   //
   template<typename Ty>
   inline bool NoProb (Ty res)
      {
   return ((Ty)0 == res); 
      }


   //
   template<typename Ty>
   inline void DontCare (const Ty& x) {
      (void) x; 
      }

   //
   template<typename Ty1, typename Ty2>
   inline bool Eq (const Ty1& a, const Ty2& b)
      {
      return a == b; 
      }

   //
   //
   struct Destructor 
      {
   public: 

      virtual ~Destructor () = 0 {}; 

   protected: 

      Destructor  (){}
      }; 


   template<typename Ty> struct Ref
      {
      typedef std::shared_ptr<Ty> Type;
      }; 



   void Destroy (Destructor* destructible);

}


#endif
