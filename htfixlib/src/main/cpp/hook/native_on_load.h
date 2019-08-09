//
// Created by timothyhe on 2019/8/9.
//

#ifndef HTFIX_NATIVE_ON_LOAD_H
#define HTFIX_NATIVE_ON_LOAD_H
#include <jni.h>


/**
 * DO NOT rename the following function
 */
extern "C" {

void HTFixNative_htfixNativeOne(JNIEnv *env, jclass cl);

void HTFixNative_htfixNativeTwo(JNIEnv *env, jclass cl);

}

#ifndef WHALE_ANDROID_AUTO_LOAD
JNIEXPORT jint JNICALL Whale_OnLoad(JavaVM *vm, void *reserved);
#endif
#endif //HTFIX_NATIVE_ON_LOAD_H
