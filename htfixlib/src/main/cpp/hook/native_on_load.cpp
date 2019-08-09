#include "native_on_load.h"
#include "../base/base.h"

#define CLASS_NAME "com/lody/whale/WhaleRuntime"

#ifndef WHALE_ANDROID_AUTO_LOAD
#define JNI_OnLoad Whale_OnLoad
#endif


extern "C" OPEN_API void WhaleRuntime_reserved0(JNI_START) {}

extern "C" OPEN_API void WhaleRuntime_reserved1(JNI_START) {}



static JNINativeMethod gMethods[] = {
        NATIVE_METHOD(WhaleRuntime, hookMethodNative,
                      "(Ljava/lang/Class;Ljava/lang/reflect/Member;Ljava/lang/Object;)J"),
        NATIVE_METHOD(WhaleRuntime, invokeOriginalMethodNative,
                      "(JLjava/lang/Object;[Ljava/lang/Object;)Ljava/lang/Object;"),
        NATIVE_METHOD(WhaleRuntime, getMethodSlot, "(Ljava/lang/reflect/Member;)J"),
        NATIVE_METHOD(WhaleRuntime, setObjectClassNative, "(Ljava/lang/Object;Ljava/lang/Class;)V"),
        NATIVE_METHOD(WhaleRuntime, cloneToSubclassNative,
                      "(Ljava/lang/Object;Ljava/lang/Class;)Ljava/lang/Object;"),
        NATIVE_METHOD(WhaleRuntime, removeFinalFlagNative,
                      "(Ljava/lang/Class;)V"),
        NATIVE_METHOD(WhaleRuntime, enforceDisableHiddenAPIPolicy, "()V"),
        NATIVE_METHOD(WhaleRuntime, reserved0, "()V"),
        NATIVE_METHOD(WhaleRuntime, reserved1, "()V")
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = nullptr;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }
    jclass cl = env->FindClass(CLASS_NAME);
    if (cl == nullptr) {
        LOGE("FindClass failed for %s",CLASS_NAME);
        return JNI_ERR;
    }
    if (env->RegisterNatives(cl, gMethods, NELEM(gMethods)) < 0) {
        LOGE("RegisterNatives failed for %s",CLASS_NAME);
        return JNI_ERR;
    }
    return JNI_VERSION_1_4;
}
