//
// Created by timothyhe on 2019/7/8.
//

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
#define roundUpToPtrSize(v) (v + pointer_size - 1 - ((v + pointer_size - 1) & (pointer_size - 1)))
static inline uint32_t read32(void *addr) {
    return *((uint32_t *) addr);
}

static inline uint64_t read64(void *addr) {
    return *((uint64_t *) addr);
}

static inline void *readAddr(void *addr) {
    return *((void **) addr);
}

#endif //HTFIX_BASE_H
