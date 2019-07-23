#ifndef SANDHOOK_LOCK_H
#define SANDHOOK_LOCK_H

#include "mutex"
#include "../fake/HideApi.h"

namespace HTFix {

    class AutoLock {
    public:
        inline AutoLock(std::mutex& mutex) : mLock(mutex)  { mLock.lock(); }
        inline AutoLock(std::mutex* mutex) : mLock(*mutex) { mLock.lock(); }
        inline ~AutoLock() { mLock.unlock(); }
    private:
        std::mutex& mLock;
    };

    class StopTheWorld {
    public:
        inline StopTheWorld()  { suspendVM(); }
        inline ~StopTheWorld() { resumeVM(); }
    };

}

#endif //SANDHOOK_LOCK_H
