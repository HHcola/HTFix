//
// Created by timothyhe on 2019/7/16.
//

#ifndef HTFIX_TRAMPOLINEMANAGER_H
#define HTFIX_TRAMPOLINEMANAGER_H

#include "map"
#include "list"
#include "mutex"
#include "../base/arch.h"
#include "../art/ArtMethod.h"
#include "../includes/lock.h"
#include "trampoline.h"
#include <unistd.h>
#include <sys/mman.h>
#include "trampoline.h"

namespace HTFix {
#define MMAP_PAGE_SIZE sysconf(_SC_PAGESIZE)
#define EXE_BLOCK_SIZE MMAP_PAGE_SIZE

    using namespace art;


    class HookTrampoline {
    public:

        HookTrampoline() = default;

        Trampoline* replacement = nullptr;
        Trampoline* inlineJump = nullptr;
        Trampoline* inlineSecondory = nullptr;
        Trampoline* callOrigin = nullptr;
        Trampoline* hookNative = nullptr;

        Code originCode = nullptr;
    };

    class TrampolineManager {
    public:
        TrampolineManager() = default;

        void init(Size quickCompileOffset) {
            this->quickCompileOffset = quickCompileOffset;
        }

        Code allocExecuteSpace(Size size);

        //java hook
        HookTrampoline* installReplacementTrampoline(mirror::ArtMethod* originMethod, mirror::ArtMethod* hookMethod, mirror::ArtMethod* backupMethod);
        HookTrampoline* installInlineTrampoline(mirror::ArtMethod* originMethod, mirror::ArtMethod* hookMethod, mirror::ArtMethod* backupMethod);

        //native hook
        HookTrampoline* installNativeHookTrampolineNoBackup(void* origin, void* hook);

        bool canSafeInline(mirror::ArtMethod* method);

        uint32_t sizeOfEntryCode(mirror::ArtMethod* method);

        HookTrampoline* getHookTrampoline(mirror::ArtMethod* method) {
            return trampolines[method];
        }

        bool memUnprotect(Size addr, Size len) {
            long pagesize = sysconf(_SC_PAGESIZE);
            unsigned alignment = (unsigned)((unsigned long long)addr % pagesize);
            int i = mprotect((void *) (addr - alignment), (size_t) (alignment + len),
                             PROT_READ | PROT_WRITE | PROT_EXEC);
            if (i == -1) {
                return false;
            }
            return true;
        }

        Code getEntryCode(void* method) {
            Code entryCode = *reinterpret_cast<Code*>((Size)method + quickCompileOffset);
            return entryCode;
        }

        static bool isThumbCode(Size codeAddr) {
            return (codeAddr & 0x1) == 0x1;
        }

        static void checkThumbCode(Trampoline* trampoline, Code code) {
#if defined(__arm__)
            trampoline->setThumb(isThumbCode(reinterpret_cast<Size>(code)));
#endif
        }

        static Code getThumbCodeAddress(Code code) {
            Size addr = reinterpret_cast<Size>(code) & (~0x00000001);
            return reinterpret_cast<Code>(addr);
        }

        bool inlineSecurityCheck = true;
        bool skipAllCheck = false;
    private:

        Size quickCompileOffset;
        std::map<mirror::ArtMethod*,HookTrampoline*> trampolines;
        std::list<Code> executeSpaceList = std::list<Code>();
        std::mutex allocSpaceLock;
        std::mutex installLock;
        Size executePageOffset = 0;
    };
}
#endif //HTFIX_TRAMPOLINEMANAGER_H
