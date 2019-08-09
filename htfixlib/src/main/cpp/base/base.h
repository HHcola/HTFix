#ifndef HTFIX_BASE_H
#define HTFIX_BASE_H

#include <jni.h>
#include <android/log.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#define LOG_TAG "HTFIX"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define pointer_size sizeof(void*)
typedef void* ptr_t;
#define OPEN_API __attribute__((visibility("default")))
#define JNI_START JNIEnv *env, jclass cl
#define roundUpToPtrSize(v) (v + pointer_size - 1 - ((v + pointer_size - 1) & (pointer_size - 1)))

#define NATIVE_METHOD(className, functionName, signature) \
    { #functionName, signature, reinterpret_cast<void*>(className ## _ ## functionName) }

#define NELEM(x) (sizeof(x)/sizeof((x)[0]))


#define CHECK_EQUAL(field, value)  \
    if ((field) != (value)) {  \
            LOGD("CHECK_EQUAL true field = %d", field); \
        return false;  \
    }

static inline uint32_t read32(void *addr) {
    return *((uint32_t *) addr);
}

static inline uint64_t read64(void *addr) {
    return *((uint64_t *) addr);
}

static inline void *readAddr(void *addr) {
    return *((void **) addr);
}




template<typename R>
static inline R OffsetOf(uintptr_t ptr, size_t offset) {
    return reinterpret_cast<R>(ptr + offset);
}

template<typename R>
static inline R OffsetOf(intptr_t ptr, size_t offset) {
    return reinterpret_cast<R>(ptr + offset);
}

template<typename R>
static inline R OffsetOf(ptr_t ptr, size_t offset) {
    return (R) (reinterpret_cast<intptr_t>(ptr) + offset);
}

template<typename T>
static inline T MemberOf(ptr_t ptr, size_t offset) {
    return *OffsetOf<T *>(ptr, offset);
}
#endif //HTFIX_BASE_H
