#ifndef _base_Lock_h_
#define _base_Lock_h_

   template <typename _Mutex_Traits, typename _Lock_Traits>
   class BaseLock
   {
      protected:
         _Mutex_Traits& mMutex;
      public:    
         BaseLock(_Mutex_Traits& mtx):
            mMutex(mtx)
         {
            _Lock_Traits::lock(mMutex);
         }

         ~BaseLock()
         {
            _Lock_Traits::unLock(mMutex);
         }
      public:
         inline void lock()
         {
            _Lock_Traits::lock(mMutex);
         }
    
         inline void unLock()
         {
            _Lock_Traits::unLock(mMutex);
         }
   };
   

#endif // _base_Lock_h_
