#ifndef _INTERFACE_H_
#define _INTERFACE_H_


namespace cx
{
   
   class Destructor 
      {
   public: 
      virtual ~Destructor () {} 
   protected: 
      Destructor  (){}
      }; 

   //
   void Destruct (Destructor* d);


}

#endif
