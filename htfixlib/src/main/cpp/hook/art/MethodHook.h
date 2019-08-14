//
// Created by timothyhe on 2019/7/11.
//

#ifndef HTFIX_METHODHOOK_H
#define HTFIX_METHODHOOK_H


#include <jni.h>
#include <mutex>
#include <list>
#include "../../base/arch_base.h"
#include "../../base/arch.h"
#include "MethodTrampoline.h"
#include "constants.h"

namespace HTFix {
#define MMAP_PAGE_SIZE sysconf(_SC_PAGESIZE)
#define EXE_BLOCK_SIZE MMAP_PAGE_SIZE



    class MethodHook {
    public:
        void init(JNIEnv *jniEnv, int sdkVersion);
        int doHookMethod(void *targetMethod, void *hookMethod);
        bool checkNativeMethod();
        void clearHotnessCount(void *targetMethod);
    private:
        void setSdkVersioin(int sdkVersion);
        void setArtMethodSize(JNIEnv *jniEnv);
        void setAccessFlags() ;
        void setEntryPointQuickCompliedCode();
        void setEntryPointInterpreter();
        void setEntryPointFromJni();
        void setDexCacheResolvedMethods();
        void setAccCompileDontBother();
        void setHotnessCount();
        void setHTFixNative(JNIEnv *env);

        size_t getArtMethodSize();
        size_t getQuickCompliedOffset();
    public:
        void installReplacementTrampoline(void *hookMethod);
        Code allocExecuteSpace(Size size);
        void flushCache(void *method);
    private:
        void setNonCompilable(void *method);
        void disableInterpreterForO(void *method);
        void setNative(void *method);
        Code allocNewSpace();
        uint32_t getAccessFlags(void *method);
        void setAccessFlags(void *method, uint32_t flags);

    private:
        int artTramReplaceMethod(void *targetMethod, void *hookMethod);
    private:
        int sdkVersion;
        int kAccCompileDontBother = 0x01000000;
        size_t artMethodSize;
        size_t offset_access_flags_;
        size_t offset_hotness_count_;

        size_t offset_entry_point_from_quick_compiled_code_;
        size_t offset_dex_cache_resolved_methods_;
        size_t offset_entry_point_from_jni_;
        size_t offset_entry_point_from_interpreter_;

    private:
        size_t native_offset_access_flags_;
        size_t native_jni_code_offset_;

    private:
        std::list<Code> executeSpaceList = std::list<Code>();
        std::mutex allocSpaceLock;
        Size executePageOffset = 0;
        MethodReplacementTrampoline * methodReplacementTrampoline;
    };
}
#endif //HTFIX_METHODHOOK_H
