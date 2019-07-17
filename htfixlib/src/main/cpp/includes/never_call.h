#ifndef HTFIX_NEVER_CALL_H
#define HTFIX_NEVER_CALL_H

#include <jni.h>

extern "C"
JNIEXPORT void JNICALL
Java_com_htfixlib_offset_NeverCallClass_neverCallNative(JNIEnv *env, jobject instance);

#endif
