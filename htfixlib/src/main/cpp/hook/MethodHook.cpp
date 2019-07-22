//
// Created by timothyhe on 2019/7/11.
//

#include <malloc.h>
#include "MethodHook.h"
#include "../base/utils.h"
#include "MethodTrampoline.h"
#include "../includes/lock.h"
#include "../trampoline/TrampolineManager.h"

namespace HTFix {
    void MethodHook::init(JNIEnv *jniEnv, int sdkVersion) {
        setSdkVersioin(sdkVersion);
        setArtMethodSize(jniEnv);
        setAccessFlags();
        setEntryPointQuickCompliedCode();
        setEntryPointInterpreter();
        setEntryPointFromJni();
        setDexCacheResolvedMethods();
        setAccCompileDontBother();
    }


    int MethodHook::doHookMethod(void *targetMethod, void *hookMethod) {
        LOGD("doHookMethod");
        if (sdkVersion >= ANDROID_N) {
            setNonCompilable(targetMethod);
            setNonCompilable(hookMethod);
        }
        flushCache(hookMethod);
        // set the target method to native so that Android O wouldn't invoke it with interpreter

        disableInterpreterForO(targetMethod);
        installReplacementTrampoline(hookMethod);
        unsigned char * code = methodReplacementTrampoline->getCode();
//        for (int i = 0; i < methodReplacementTrampoline->getCodeLen(); i ++) {
//            LOGD("methodReplacementTrampoline code value i = %d, value = %04x", i, *(code + i));
//        }
        size_t address = reinterpret_cast<size_t>(methodReplacementTrampoline->getCode());
        if (methodReplacementTrampoline != nullptr) {
//            memcpy((char *) targetMethod + offset_entry_point_from_quick_compiled_code_,
//                   methodReplacementTrampoline->getCode(),
//                   pointer_size);

            memcpy((char *) targetMethod + offset_entry_point_from_quick_compiled_code_,
                   &address,
                   pointer_size);
        } else {
            LOGE("failed to allocate space for trampoline of target method");
            return 1;
        }

        if (offset_entry_point_from_interpreter_ != 0) {
            LOGE("doHookMethod set offset_entry_point_from_interpreter_ ");
            memcpy((char *) targetMethod + offset_entry_point_from_interpreter_,
                   (char *) hookMethod + offset_entry_point_from_interpreter_,
                   pointer_size);
        }

        flushCache(targetMethod);
        return 0;
    }

    void MethodHook::setArtMethodSize(JNIEnv *jniEnv) {
        jclass sizeTestClass = jniEnv->FindClass("com/htfixlib/ArtMethodSize");
        Size artMethod1 = (Size) jniEnv->GetStaticMethodID(sizeTestClass, "method1", "()V");
        Size artMethod2 = (Size) jniEnv->GetStaticMethodID(sizeTestClass, "method2", "()V");
        artMethodSize = artMethod2 - artMethod1;
        LOGD("setArtMethodSize size = %d", artMethodSize);
    }

    void MethodHook::setAccessFlags() {
        switch (sdkVersion) {
            case ANDROID_L:
                offset_access_flags_ = 56;
                break;
            case ANDROID_L2:
                offset_access_flags_ = 20;
                break;
            case ANDROID_M:
            case ANDROID_N:
            case ANDROID_N2:
            case ANDROID_O:
            case ANDROID_O2:
            case ANDROID_P:
            case ANDROID_Q:
                offset_access_flags_ = 4;
                break;
            default:
                offset_access_flags_ = 4;
                break;
        }
        LOGD("setAccessFlags offset = %d", offset_access_flags_);
    }

    void MethodHook::setEntryPointQuickCompliedCode() {
        if (sdkVersion >= ANDROID_M) {
            offset_entry_point_from_quick_compiled_code_ =  getArtMethodSize() - BYTE_POINT;
        } else if (sdkVersion <= ANDROID_L) {
            offset_entry_point_from_quick_compiled_code_ = getArtMethodSize() - 4 - 2 * BYTE_POINT;
        } else {
            // get error address of ANDROID_P and ANDROID_Q
            offset_entry_point_from_quick_compiled_code_ = getArtMethodSize() - 4 - 2 * BYTE_POINT;
        }
        LOGD("setEntryPointQuickCompliedCode offset = %d", offset_entry_point_from_quick_compiled_code_);
    }

    void MethodHook::setEntryPointInterpreter() {
        if (sdkVersion >= ANDROID_L2 && sdkVersion <= ANDROID_M)
            offset_entry_point_from_interpreter_ = getArtMethodSize() - 3 * BYTE_POINT;
        else if (sdkVersion <= ANDROID_L) {
            offset_entry_point_from_interpreter_ = getArtMethodSize() - 4 * 8 - 4 * 4;
        } else {
            offset_entry_point_from_interpreter_ = 0;
        }
        LOGD("setEntryPointInterpreter offset = %d", offset_entry_point_from_interpreter_);
    }

    void MethodHook::setDexCacheResolvedMethods() {
        if (sdkVersion >= ANDROID_P) {
            offset_dex_cache_resolved_methods_ = getArtMethodSize() + 1;
        }else if (sdkVersion == ANDROID_M) {
            offset_dex_cache_resolved_methods_ = 4;
        } else if (sdkVersion >= ANDROID_L && sdkVersion <= ANDROID_L2) {
            offset_dex_cache_resolved_methods_ = 4 * 3;
        } else {
            offset_dex_cache_resolved_methods_ = getArtMethodSize() + 1;
        }
        LOGD("setDexCacheResolvedMethods offset = %d", offset_dex_cache_resolved_methods_);
    }

    void MethodHook::setAccCompileDontBother() {
        if (sdkVersion == __ANDROID_API_P__ || sdkVersion == __ANDROID_API_O_MR1__) {
            kAccCompileDontBother = 0x02000000;
        }
    }

    void MethodHook::setEntryPointFromJni() {
        if (sdkVersion >= ANDROID_L2 && sdkVersion <= ANDROID_N) {
            offset_entry_point_from_jni_ = getArtMethodSize() - 2 * BYTE_POINT;
        } else {
            offset_entry_point_from_jni_ = getArtMethodSize() - 8 * 2 - 4 * 4;
        }
    }
    void MethodHook::setSdkVersioin(int sdkVersion) {
        this->sdkVersion = sdkVersion;
    }

    size_t MethodHook::getArtMethodSize() {
        return artMethodSize;
    }
    size_t MethodHook::getQuickCompliedOffset() {
        return offset_entry_point_from_quick_compiled_code_;
    }
    void MethodHook::setNonCompilable(void *method) {
        if (sdkVersion < ANDROID_N) {
            return;
        }
        int access_flags = read32((char *) method + offset_access_flags_);
        LOGI("setNonCompilable: access flags is 0x%x, offset_access_flags_ = %d", access_flags, offset_access_flags_);
//        access_flags |= kAccCompileDontBother;
        access_flags |= kAccCompileDontBother;
        access_flags |= 0x00800000;
        LOGI("setNonCompilable: access flags and kAccCompileDontBother is 0x%x", access_flags);
        memcpy(
                (char *) method + offset_access_flags_,
                &access_flags,
                4
        );
    }
    void MethodHook::disableInterpreterForO(void *method) {
//        if (SDK_INT >= ANDROID_O && DEBUG) {
           if (sdkVersion >= ANDROID_O) {
                setNative(method);
        }
    }
    void MethodHook::setNative(void *method) {
        uint32_t accessFlag = getAccessFlags(method);
        LOGD("setNative accessFlag = 0x%x", accessFlag);
        accessFlag |= 0x0100;
        setAccessFlags(method, accessFlag);
        LOGD("setNative after accessFlag = 0x%x", accessFlag);
    }

    uint32_t MethodHook::getAccessFlags(void *method) {
        return read32((char *) method + offset_access_flags_);
    }
    void MethodHook::setAccessFlags(void *method, uint32_t access_flags) {
        memcpy(
                (char *) method + offset_access_flags_,
                &access_flags,
                4
        );
    }


    void MethodHook::installReplacementTrampoline(void *hookMethod) {
        LOGD("installReplacementTrampoline");
        methodReplacementTrampoline = new MethodReplacementTrampoline();
        methodReplacementTrampoline->initTrampoline();
        methodReplacementTrampoline->init();
        Code replacementHookTrampolineSpace = allocExecuteSpace(methodReplacementTrampoline->getCodeLen());
        if (replacementHookTrampolineSpace == 0) {
            LOGE("hook error due to can not alloc execute space!");
            return;
        }
        methodReplacementTrampoline->setExecuteSpace(replacementHookTrampolineSpace);
        methodReplacementTrampoline->setEntryCodeOffset(getQuickCompliedOffset());
        methodReplacementTrampoline->setHookMethod(reinterpret_cast<Code>(hookMethod));
    }

    unsigned char *MethodHook::allocExecuteSpace(Size size) {
        if (size > EXE_BLOCK_SIZE) {
            return 0;
        }
        LOGD("allocExecuteSpace size = %d, executePageOffset = %d", size, executePageOffset);
        AutoLock autoLock(allocSpaceLock);
        if (executeSpaceList.size() == 0 || executePageOffset + size > EXE_BLOCK_SIZE) {
            return allocNewSpace();
        } else {
            LOGD("allocExecuteSpace use exist add");
            Code exeSpace = 0;
            exeSpace = executeSpaceList.back();
            Code retSpace = exeSpace + executePageOffset;
            executePageOffset += size;
            LOGD("allocExecuteSpace retSpace 0x%x", retSpace);
            return retSpace;
        }
    }

    void MethodHook::flushCache(void *method) {
        flushCacheExt(reinterpret_cast<Size>(method), artMethodSize);
    }

    Code MethodHook::allocNewSpace() {
        LOGD("allocNewSpace");
        void* mmapRes;
        Code exeSpace = 0;
        mmapRes = mmap(NULL, EXE_BLOCK_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC,
                       MAP_ANON | MAP_PRIVATE, -1, 0);
        if (mmapRes == MAP_FAILED) {
            LOGE("allocNewSpace fail mmapRes == MAP_FAILED");
            return 0;
        }
        memset(mmapRes, 0, EXE_BLOCK_SIZE);
        exeSpace = static_cast<Code>(mmapRes);
        LOGD("allocNewSpace 0x%x", exeSpace);
        return exeSpace;
    }



}