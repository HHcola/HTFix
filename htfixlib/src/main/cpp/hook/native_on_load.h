//
// Created by timothyhe on 2019/8/9.
//

#ifndef HTFIX_NATIVE_ON_LOAD_H
#define HTFIX_NATIVE_ON_LOAD_H
#include <jni.h>

constexpr const char *kMethodReserved0 = "reserved0";
constexpr const char *kMethodReserved1 = "reserved1";

/**
 * DO NOT rename the following function
 */
extern "C" {

void WhaleRuntime_reserved0(JNIEnv *env, jclass cl);

void WhaleRuntime_reserved1(JNIEnv *env, jclass cl);

}

#ifndef WHALE_ANDROID_AUTO_LOAD
JNIEXPORT jint JNICALL Whale_OnLoad(JavaVM *vm, void *reserved);
#endif
#endif //HTFIX_NATIVE_ON_LOAD_H
