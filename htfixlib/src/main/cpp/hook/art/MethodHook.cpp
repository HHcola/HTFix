#include <malloc.h>
#include "MethodHook.h"
#include "../../base/utils.h"
#include "MethodTrampoline.h"
#include "../../base/lock.h"
#include "../HTFixMethodHook.h"

#define CLASS_NAME "com/htfixlib/HTFixNative"

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
        setHTFixNative(jniEnv);
    }


    int MethodHook::doHookMethod(void *targetMethod, void *hookMethod) {
        LOGD("doHookMethod");
        if (sdkVersion < __ANDROID_API_K__) {
          // don't support
        } else if (sdkVersion >= __ANDROID_API_K__ || sdkVersion <= __ANDROID_API_N_MR1__) {
            return artReplaceMethod(targetMethod, hookMethod);
        } else {
            return artTramReplaceMethod(targetMethod, hookMethod);
        }
    }


    /**
     * check native_offset_access_flags_ and native_jni_code_offset_
     */
    bool MethodHook::checkNativeMethod() {
        checkField(native_offset_access_flags_, INT32_MAX);
        checkField(native_jni_code_offset_, INT32_MAX);

        checkField(native_offset_access_flags_, offset_access_flags_);
        checkField(native_jni_code_offset_, offset_entry_point_from_jni_);
        LOGD("checkNativeMethod true");
        return true;
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
            case __ANDROID_API_L__:
                offset_access_flags_ = 56;
                break;
            case __ANDROID_API_L_MR1__:
                offset_access_flags_ = 20;
                break;
            case __ANDROID_API_M__:
            case __ANDROID_API_N__:
            case __ANDROID_API_N_MR1__:
            case __ANDROID_API_O__:
            case __ANDROID_API_O_MR1__:
            case __ANDROID_API_P__:
            case __ANDROID_API_Q__:
                offset_access_flags_ = 4;
                break;
            default:
                offset_access_flags_ = 4;
                break;
        }
        LOGD("setAccessFlags offset = %d", offset_access_flags_);
    }

    void MethodHook::setEntryPointQuickCompliedCode() {
        if (sdkVersion >= __ANDROID_API_M__) {
            offset_entry_point_from_quick_compiled_code_ =  getArtMethodSize() - BYTE_POINT;
        } else if (sdkVersion <= __ANDROID_API_L__) {
            offset_entry_point_from_quick_compiled_code_ = getArtMethodSize() - 4 - 2 * BYTE_POINT;
        } else {
            // get error address of ANDROID_P and ANDROID_Q
            offset_entry_point_from_quick_compiled_code_ = getArtMethodSize() - 4 - 2 * BYTE_POINT;
        }
        LOGD("setEntryPointQuickCompliedCode offset = %d", offset_entry_point_from_quick_compiled_code_);
    }

    void MethodHook::setEntryPointInterpreter() {
        if (sdkVersion >= __ANDROID_API_L_MR1__ && sdkVersion <= __ANDROID_API_M__)
            offset_entry_point_from_interpreter_ = getArtMethodSize() - 3 * BYTE_POINT;
        else if (sdkVersion <= __ANDROID_API_L__) {
            offset_entry_point_from_interpreter_ = getArtMethodSize() - 4 * 8 - 4 * 4;
        } else {
            offset_entry_point_from_interpreter_ = 0;
        }
        LOGD("setEntryPointInterpreter offset = %d", offset_entry_point_from_interpreter_);
    }

    void MethodHook::setDexCacheResolvedMethods() {
        if (sdkVersion >= __ANDROID_API_P__) {
            offset_dex_cache_resolved_methods_ = getArtMethodSize() + 1;
        }else if (sdkVersion == __ANDROID_API_M__) {
            offset_dex_cache_resolved_methods_ = 4;
        } else if (sdkVersion >= __ANDROID_API_L__ && sdkVersion <= __ANDROID_API_L_MR1__) {
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

    void MethodHook::setHTFixNative(JNIEnv *env) {
        size_t expected_access_flags = Constants::kAccPrivate | Constants::kAccStatic | Constants::kAccNative;
        jclass java_class = env->FindClass(CLASS_NAME);
        if (java_class == nullptr) {
            LOGE("FindClass failed for = %s", CLASS_NAME);
            return;
        }

        jmethodID methodOne = env->GetStaticMethodID(java_class, Constants::kMethodHTFixOne, "()V");
        jmethodID methodTwo = env->GetStaticMethodID(java_class, Constants::kMethodHTFixTwo, "()V");

        for (size_t offset = 0; offset != sizeof(size_t) * 24; offset += sizeof(size_t)) {
            if (MemberOf<size_t>(methodOne, offset) == expected_access_flags) {
                native_offset_access_flags_ = offset;
                break;
            }
        }
        void *native_function = reinterpret_cast<void *>(Java_com_htfixlib_HTFixNative_htfixNativeOne);

        for (size_t offset = 0; offset != sizeof(size_t) * 24; offset += sizeof(size_t)) {
            if (MemberOf<ptr_t>(methodOne, offset) == native_function) {
                native_jni_code_offset_ = offset;
                break;
            }
        }

        LOGD("setHTFixNative native_offset_access_flags_ = %d, native_jni_code_offset_ = %d", native_offset_access_flags_, native_jni_code_offset_);
        LOGD("setHTFixNative offset_access_flags_ = %d, offset_entry_point_from_jni_ = %d", offset_access_flags_, offset_entry_point_from_jni_);
    }



    void MethodHook::setEntryPointFromJni() {
        if (sdkVersion >= __ANDROID_API_L_MR1__ && sdkVersion <= __ANDROID_API_N__) {
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
        if (sdkVersion < __ANDROID_API_N__) {
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
           if (sdkVersion >= __ANDROID_API_O__) {
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

    int MethodHook::artTramReplaceMethod(void *targetMethod, void *hookMethod) {
        if (sdkVersion >= __ANDROID_API_N__) {
            setNonCompilable(targetMethod);
            setNonCompilable(hookMethod);
        }
        flushCache(hookMethod);
        // set the target method to native so that Android O wouldn't invoke it with interpreter

        disableInterpreterForO(targetMethod);
        installReplacementTrampoline(hookMethod);
        unsigned char * code = methodReplacementTrampoline->getCode();
        size_t address = reinterpret_cast<size_t>(methodReplacementTrampoline->getCode());
        if (methodReplacementTrampoline != nullptr) {

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
        return 1;
    }

    int MethodHook::artReplaceMethod(void *targetMethod, void *hookMethod) {
        if (artMethodSize <= 0) {
            LOGD("artReplaceMethod error artMethodSize is zero");
            return 0;
        }

        for (int i = 0; i < artMethodSize; i ++) {
            LOGD("artReplaceMethod befor targetMethod i = %d, value = 0x%x", i, *((char *)targetMethod + i));
        }

        for (int i = 0; i < artMethodSize; i ++) {
            LOGD("artReplaceMethod befor hookMethod i = %d, value = 0x%x", i, *((char *)hookMethod + i));
        }
        memcpy(targetMethod,
                hookMethod,
                artMethodSize
        );

        for (int i = 0; i < artMethodSize; i ++) {
            LOGD("artReplaceMethod after targetMethod i = %d, value = 0x%x", i, *((char *)targetMethod + i));
        }

        return 1;
    }



}