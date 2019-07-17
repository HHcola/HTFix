//
// Created by timothyhe on 2019/7/8.
//
#include <jni.h>
#include "base/base.h"
#include "hook/MethodHook.h"
#include "art/ArtMethodOffSet.h"
#include "includes/never_call.h"
#include "fake/HideApi.h"
#include "includes/cast_compiler_options.h"
#include "trampoline/TrampolineManager.h"

extern "C" int SDK_INT = 0;
extern "C" bool DEBUG = false;

HTFix::TrampolineManager trampolineManager;

enum HookMode {
    AUTO = 0,
    INLINE = 1,
    REPLACE = 2
};

HookMode gHookMode = AUTO;

void ensureMethodCached(art::mirror::ArtMethod *hookMethod, art::mirror::ArtMethod *backupMethod) {
    if (SDK_INT >= ANDROID_P)
        return;

    HTFix::StopTheWorld stopTheWorld;

    uint32_t index = backupMethod->getDexMethodIndex();
    if (SDK_INT < ANDROID_O2) {
        hookMethod->setDexCacheResolveItem(index, backupMethod);
    } else {
        int cacheSize = 1024;
        Size slotIndex = index % cacheSize;
        Size newCachedMethodsArray = reinterpret_cast<Size>(calloc(cacheSize, BYTE_POINT * 2));
        unsigned int one = 1;
        memcpy(reinterpret_cast<void *>(newCachedMethodsArray + BYTE_POINT), &one, 4);
        memcpy(reinterpret_cast<void *>(newCachedMethodsArray + BYTE_POINT * 2 * slotIndex),
               (&backupMethod),
               BYTE_POINT
        );
        memcpy(reinterpret_cast<void *>(newCachedMethodsArray + BYTE_POINT * 2 * slotIndex + BYTE_POINT),
               &index,
               4
        );
        hookMethod->setDexCacheResolveList(&newCachedMethodsArray);
    }
}


bool doHookWithReplacement(JNIEnv* env,
                           art::mirror::ArtMethod *originMethod,
                           art::mirror::ArtMethod *hookMethod,
                           art::mirror::ArtMethod *backupMethod) {

    if (!hookMethod->compile(env)) {
        hookMethod->disableCompilable();
    }

    if (SDK_INT > ANDROID_N) {
        forceProcessProfiles();
    }
    if ((SDK_INT >= ANDROID_N && SDK_INT <= ANDROID_P)
        || (SDK_INT >= ANDROID_Q && !originMethod->isAbstract())) {
        originMethod->setHotnessCount(0);
    }

    if (backupMethod != nullptr) {
        originMethod->backup(backupMethod);
        backupMethod->disableCompilable();
        if (!backupMethod->isStatic()) {
            backupMethod->setPrivate();
        }
        backupMethod->flushCache();
    }

    originMethod->disableCompilable();
    hookMethod->disableCompilable();
    hookMethod->flushCache();

    originMethod->disableInterpreterForO();

    HTFix::HookTrampoline* hookTrampoline = trampolineManager.installReplacementTrampoline(originMethod, hookMethod, backupMethod);
    if (hookTrampoline != nullptr) {
        originMethod->setQuickCodeEntry(hookTrampoline->replacement->getCode());
        void* entryPointFormInterpreter = hookMethod->getInterpreterCodeEntry();
        if (entryPointFormInterpreter != NULL) {
            originMethod->setInterpreterCodeEntry(entryPointFormInterpreter);
        }
        if (hookTrampoline->callOrigin != nullptr) {
            backupMethod->setQuickCodeEntry(hookTrampoline->callOrigin->getCode());
            backupMethod->flushCache();
        }
        originMethod->flushCache();
        return true;
    } else {
        return false;
    }
}


bool doHookWithInline(JNIEnv* env,
                      art::mirror::ArtMethod *originMethod,
                      art::mirror::ArtMethod *hookMethod,
                      art::mirror::ArtMethod *backupMethod) {

    //fix >= 8.1
    if (!hookMethod->compile(env)) {
        hookMethod->disableCompilable();
    }

    originMethod->disableCompilable();
    if (SDK_INT > ANDROID_N) {
        forceProcessProfiles();
    }
    if ((SDK_INT >= ANDROID_N && SDK_INT <= ANDROID_P)
        || (SDK_INT >= ANDROID_Q && !originMethod->isAbstract())) {
        originMethod->setHotnessCount(0);
    }
    originMethod->flushCache();

    HTFix::HookTrampoline* hookTrampoline = trampolineManager.installInlineTrampoline(originMethod, hookMethod, backupMethod);

    if (hookTrampoline == nullptr)
        return false;

    hookMethod->flushCache();
    if (hookTrampoline->callOrigin != nullptr) {
        //backup
        originMethod->backup(backupMethod);
        backupMethod->setQuickCodeEntry(hookTrampoline->callOrigin->getCode());
        backupMethod->disableCompilable();
        if (!backupMethod->isStatic()) {
            backupMethod->setPrivate();
        }
        backupMethod->flushCache();
    }
    return true;
}



extern "C"
JNIEXPORT jint JNICALL
Java_com_htfixlib_HTFixNative_hookMethod(JNIEnv *env, jclass type, jobject originMethod,
                                         jobject hookMethod, jobject backupMethod, jint hookMode) {
    art::mirror::ArtMethod* origin = reinterpret_cast<art::mirror::ArtMethod *>(env->FromReflectedMethod(originMethod));
    art::mirror::ArtMethod* hook = reinterpret_cast<art::mirror::ArtMethod *>(env->FromReflectedMethod(hookMethod));
    art::mirror::ArtMethod* backup = backupMethod == NULL ? nullptr : reinterpret_cast<art::mirror::ArtMethod *>(env->FromReflectedMethod(backupMethod));

    bool isInlineHook = false;

    int mode = reinterpret_cast<int>(hookMode);

    //suspend other threads
    HTFix::StopTheWorld stopTheWorld;

    if (mode == INLINE) {
        if (!origin->isCompiled()) {
            if (SDK_INT >= ANDROID_N) {
                isInlineHook = origin->compile(env);
            }
        } else {
            isInlineHook = true;
        }
        goto label_hook;
    } else if (mode == REPLACE) {
        isInlineHook = false;
        goto label_hook;
    }

    if (origin->isAbstract()) {
        isInlineHook = false;
    } else if (gHookMode != AUTO) {
        if (gHookMode == INLINE) {
            isInlineHook = origin->compile(env);
        } else {
            isInlineHook = false;
        }
    } else if (SDK_INT >= ANDROID_O) {
        isInlineHook = false;
    } else if (!origin->isCompiled()) {
        if (SDK_INT >= ANDROID_N) {
            isInlineHook = origin->compile(env);
        } else {
            isInlineHook = false;
        }
    } else {
        isInlineHook = true;
    }


    label_hook:
    if (isInlineHook && trampolineManager.canSafeInline(origin)) {
        return doHookWithInline(env, origin, hook, backup) ? INLINE : -1;
    } else {
        return doHookWithReplacement(env, origin, hook, backup) ? REPLACE : -1;
    }
}



extern "C"
JNIEXPORT jboolean JNICALL
Java_com_htfixlib_HTFixNative_initNative(JNIEnv *env, jclass type, jint sdk, jboolean debug) {
    SDK_INT = sdk;
    DEBUG = debug;
    initHideApi(env);
    HTFix::CastArtMethod::init(env);
    HTFix::CastCompilerOptions::init(env);
    trampolineManager.init(HTFix::CastArtMethod::entryPointQuickCompiled->getOffset());
    return JNI_TRUE;
}



extern "C"
JNIEXPORT void JNICALL
Java_com_htfixlib_HTFixNative_ensureMethodCached(JNIEnv *env, jclass clazz, jobject hook,
                                                 jobject backup) {
    art::mirror::ArtMethod* hookeMethod = reinterpret_cast<art::mirror::ArtMethod *>(env->FromReflectedMethod(hook));
    art::mirror::ArtMethod* backupMethod = backup == NULL ? nullptr : reinterpret_cast<art::mirror::ArtMethod *>(env->FromReflectedMethod(backup));
    ensureMethodCached(hookeMethod, backupMethod);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_htfixlib_HTFixNative_ensureDeclareClass(JNIEnv *env, jclass clazz, jobject originMethod,
                                                 jobject backupMethod) {
    if (originMethod == NULL || backupMethod == NULL)
        return;
    art::mirror::ArtMethod* origin = reinterpret_cast<art::mirror::ArtMethod *>(env->FromReflectedMethod(originMethod));
    art::mirror::ArtMethod* backup = reinterpret_cast<art::mirror::ArtMethod *>(env->FromReflectedMethod(backupMethod));
    if (origin->getDeclaringClass() != backup->getDeclaringClass()) {
        LOGW("declaring class has been moved!");
        backup->setDeclaringClass(origin->getDeclaringClass());
    }
}


extern "C"
JNIEXPORT jboolean JNICALL
Java_com_htfixlib_HTFixNative_compileMethod(JNIEnv *env, jclass clazz, jobject member) {
    if (member == NULL)
        return JNI_FALSE;
    art::mirror::ArtMethod* method = reinterpret_cast<art::mirror::ArtMethod *>(env->FromReflectedMethod(member));

    if (method == nullptr)
        return JNI_FALSE;

    if (!method->isCompiled()) {
        HTFix::StopTheWorld stopTheWorld;
        if (!method->compile(env)) {
            if (SDK_INT >= ANDROID_N) {
                method->disableCompilable();
                method->flushCache();
            }
            return JNI_FALSE;
        } else {
            return JNI_TRUE;
        }
    } else {
        return JNI_TRUE;
    }
}


extern "C"
JNIEXPORT jboolean JNICALL
Java_com_htfixlib_HTFixNative_canGetObject(JNIEnv *env, jclass clazz) {
    return static_cast<jboolean>(canGetObject());
}


extern "C"
JNIEXPORT jobject JNICALL
Java_com_htfixlib_HTFixNative_getObjectNative(JNIEnv *env, jclass clazz, jlong thread,
                                              jlong address) {
    return getJavaObject(env, reinterpret_cast<void *>(thread), reinterpret_cast<void *>(address));
}


extern "C"
JNIEXPORT void JNICALL
Java_com_htfixlib_offset_NeverCallClass_neverCallNative(JNIEnv *env, jobject instance) {
    int a = 1 + 1;
    int b = a + 1;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_htfixlib_offset_NeverCallClass_neverCallNative2(JNIEnv *env, jobject instance) {
    int a = 4 + 3;
    int b = 9 + 6;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_htfixlib_HTFixNative_disableVMInline(JNIEnv *env, jclass clazz) {
    if (SDK_INT < ANDROID_N)
        return JNI_FALSE;
    replaceUpdateCompilerOptionsQ();
    art::CompilerOptions* compilerOptions = getGlobalCompilerOptions();
    if (compilerOptions == nullptr)
        return JNI_FALSE;
    return static_cast<jboolean>(disableJitInline(compilerOptions));
}
