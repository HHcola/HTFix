//
// Created by timothyhe on 2019/7/24.
//

#ifndef HTFIX_DALVIKMETHODREPLACE_H
#define HTFIX_DALVIKMETHODREPLACE_H

#include <jni.h>
#include <time.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

#include <stdbool.h>
#include <fcntl.h>
#include <dlfcn.h>

#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <utime.h>
#include <sys/types.h>
#include <sys/wait.h>

namespace HTFix {
    class DalvikMethodReplace {
    public:
        jboolean dalvik_setup(JNIEnv* env, int apilevel);
        int dalvik_replaceMethod(JNIEnv* env, jobject target, jobject hook);
    private:
        void* dvm_dlsym(void *hand, const char *name);
        void dalvik_setFieldFlag(JNIEnv* env, jobject field);
    };
}

#endif //HTFIX_DALVIKMETHODREPLACE_H
