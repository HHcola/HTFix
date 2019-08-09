//
// Created by timothyhe on 2019/8/9.
//

#ifndef HTFIX_HTFIXMETHODHOOK_H
#define HTFIX_HTFIXMETHODHOOK_H
#include "jni.h"

extern "C" {

void Java_com_htfixlib_HTFixNative_htfixNativeOne(JNIEnv *env, jclass cl);

void Java_com_htfixlib_HTFixNative_htfixNativeTwo(JNIEnv *env, jclass cl);
}

#endif //HTFIX_HTFIXMETHODHOOK_H
