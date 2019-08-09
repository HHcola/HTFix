#include "art/MethodHook.h"
#include "dalvik/DalvikMethodReplace.h"
#include "HTFixMethodHook.h"
HTFix::MethodHook methodHook;
HTFix::DalvikMethodReplace dalvikMethodReplace;
static bool isArt = true;

extern "C"
JNIEXPORT jint JNICALL
Java_com_htfixlib_HTFixNative_htfixHookMethod(JNIEnv *env, jclass clazz, jobject target_method,
                                              jobject hook_method) {

    if (isArt) {
        void* target = reinterpret_cast<void *>(env->FromReflectedMethod(target_method));
        void*  hook = reinterpret_cast<void *>(env->FromReflectedMethod(hook_method));
        return methodHook.doHookMethod(target , hook);
    } else {
        return dalvikMethodReplace.dalvik_replaceMethod(env, target_method , hook_method);
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_htfixlib_HTFixNative_setup(JNIEnv *env, jclass clazz, jboolean is_art, jint apilevel) {
    isArt = is_art;
    if (is_art) {
        methodHook.init(env, apilevel);
    } else {
        dalvikMethodReplace.dalvik_setup(env, apilevel);
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_htfixlib_HTFixNative_checkHookMethod(JNIEnv *env, jclass clazz) {
    return methodHook.checkNativeMethod();
}


extern "C"
JNIEXPORT void JNICALL
Java_com_htfixlib_HTFixNative_htfixNativeOne(JNIEnv *env, jclass clazz) {
}

extern "C"
JNIEXPORT void JNICALL
Java_com_htfixlib_HTFixNative_htfixNativeTwo(JNIEnv *env, jclass clazz) {
}


//extern "C" OPEN_API void Java_com_htfixlib_HTFixNative_htfixNativeOne(JNI_START) {}