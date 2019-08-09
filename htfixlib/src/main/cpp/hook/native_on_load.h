//
// Created by timothyhe on 2019/8/9.
//

#ifndef HTFIX_NATIVE_ON_LOAD_H
#define HTFIX_NATIVE_ON_LOAD_H
#include <jni.h>

#define CLASS_NAME "com/htfixlib/HTFixNative"

/**
 * DO NOT rename the following function
 */
extern "C" {

void HTFixNative_htfixNativeOne(JNIEnv *env, jclass cl);

void HTFixNative_htfixNativeTwo(JNIEnv *env, jclass cl);

}


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved);
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved);


#endif //HTFIX_NATIVE_ON_LOAD_H
