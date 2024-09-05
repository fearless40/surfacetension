#pragma once

namespace Util {



class CriticalSection
{
    public:
    static const int DefaultSpinCount = 400;
    
    protected:
    CRITICAL_SECTION mSection;
    
    public:
    CriticalSection( int spinCount ) {
		InitializeCriticalSectionAndSpinCount( &mSection, spinCount );
    }
    CriticalSection( ) {
		InitializeCriticalSectionAndSpinCount( &mSection, DefaultSpinCount );
		
    }

    ~CriticalSection() {
        DeleteCriticalSection( &mSection );
    }
    
    void setSpinCount( int spinCount ) {
        SetCriticalSectionSpinCount( &mSection, spinCount );
    }
    
    void lock() {
        EnterCriticalSection( &mSection );
    }
    
    void unlock() {
        LeaveCriticalSection( &mSection );
    }
    
    void tryLock() {
        TryEnterCriticalSection( &mSection );
    }
};

typedef CriticalSection LightMutex;

};