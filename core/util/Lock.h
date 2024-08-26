
namespace Util {
    
    template< class LockAble >
    class Lock 
    {
        LockAble & mItem;
        Lock();
        
        public:
        Lock( LockAble & item ) : mItem(item) {
            mItem.lock();
        }
        ~Lock() {
            mItem.unlock();
        }
    };
    
    
    template < class TryLockAble >
    class TryLock 
    {
        TryLockAble & mItem;
        TryLock();
        
        public:
        TryLock(TryLockAble & item) : mItem( item ){
            mItem.tryLock();
        }
        
        ~TryLock( ) {
            mItem.unlock();
        }
    };
};