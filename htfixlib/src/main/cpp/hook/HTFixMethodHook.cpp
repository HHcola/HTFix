//
// Created by Administrator on 2019/7/21.
//
#include "jni.h"
#include "MethodHook.h"

HTFix::MethodHook methodHook;

extern "C"
JNIEXPORT void JNICALL
Java_com_htfixlib_HTFixNative_htfixInitNative(JNIEnv *env, jclass type, jint sdk) {
    methodHook.init(env, sdk);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_htfixlib_HTFixNative_htfixHookMethod(JNIEnv *env, jclass clazz, jobject target_method,
                                              jobject hook_method) {
    void* target = reinterpret_cast<void *>(env->FromReflectedMethod(target_method));
    void*  hook = reinterpret_cast<void *>(env->FromReflectedMethod(hook_method));
    LOGD("htfixHookMethod target = 0x%x hook = 0x%x", target, hook);
    return methodHook.doHookMethod(target , hook);
}