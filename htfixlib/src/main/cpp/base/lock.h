#ifndef SANDHOOK_LOCK_H
#define SANDHOOK_LOCK_H

#include "mutex"
namespace HTFix {

    class AutoLock {
    public:
        inline AutoLock(std::mutex& mutex) : mLock(mutex)  { mLock.lock(); }
        inline AutoLock(std::mutex* mutex) : mLock(*mutex) { mLock.lock(); }
        inline ~AutoLock() { mLock.unlock(); }
    private:
        std::mutex& mLock;
    };
}

#endif //SANDHOOK_LOCK_H
