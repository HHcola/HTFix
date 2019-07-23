#ifndef HTFIX_UTILS_H
#define HTFIX_UTILS_H
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/mman.h>
#include "arch.h"

extern "C" {

Size getAddressFromJava(JNIEnv *env, const char *className, const char *fieldName);

Size getAddressFromJavaByCallMethod(JNIEnv *env, const char *className, const char *methodName);

jint getIntFromJava(JNIEnv *env, const char *className, const char *fieldName);

bool getBooleanFromJava(JNIEnv *env, const char *className, const char *fieldName);

bool munprotect(size_t addr, size_t len);

bool flushCacheExt(Size addr, Size len);

}

#endif //HTFIX_UTILS_H
