//
// Created by timothyhe on 2019/7/24.
//

#include "DalvikMethodReplace.h"
#include "dalvik.h"
#include "../../base/base.h"

namespace HTFix {
    jboolean DalvikMethodReplace::dalvik_setup(JNIEnv *env, int apilevel) {
        void* dvm_hand = dlopen("libdvm.so", RTLD_NOW);
        if (dvm_hand) {
            dvmDecodeIndirectRef_fnPtr = reinterpret_cast<dvmDecodeIndirectRef_func>((Object *) dvm_dlsym(
                    dvm_hand,
                    apilevel > 10 ?
                    "_Z20dvmDecodeIndirectRefP6ThreadP8_jobject" :
                    "dvmDecodeIndirectRef"));
            if (!dvmDecodeIndirectRef_fnPtr) {
                return JNI_FALSE;
            }
            dvmThreadSelf_fnPtr = reinterpret_cast<dvmThreadSelf_func>((Object *) dvm_dlsym(dvm_hand,
                                            apilevel > 10 ? "_Z13dvmThreadSelfv" : "dvmThreadSelf"));
            if (!dvmThreadSelf_fnPtr) {
                return JNI_FALSE;
            }
            jclass clazz = env->FindClass("java/lang/reflect/Method");
            jClassMethod = env->GetMethodID(clazz, "getDeclaringClass",
                                            "()Ljava/lang/Class;");

            return JNI_TRUE;
        } else {
            return JNI_FALSE;
        }
    }

    int DalvikMethodReplace::dalvik_replaceMethod(JNIEnv *env, jobject targetMethod, jobject hookMethod) {
        jobject clazz = env->CallObjectMethod(hookMethod, jClassMethod);
        ClassObject* clz = (ClassObject*) dvmDecodeIndirectRef_fnPtr(
                dvmThreadSelf_fnPtr(), clazz);
        clz->status = CLASS_INITIALIZED;

        Method* meth = (Method*) env->FromReflectedMethod(targetMethod);
        Method* hook = (Method*) env->FromReflectedMethod(hookMethod);
        LOGD("dalvikMethod: %s", meth->name);

//	meth->clazz = target->clazz;
        meth->accessFlags |= ACC_PUBLIC;
        meth->methodIndex = hook->methodIndex;
        meth->jniArgInfo = hook->jniArgInfo;
        meth->registersSize = hook->registersSize;
        meth->outsSize = hook->outsSize;
        meth->insSize = hook->insSize;

        meth->prototype = hook->prototype;
        meth->insns = hook->insns;
        meth->nativeFunc = hook->nativeFunc;
        return 1;
    }

    void *DalvikMethodReplace::dvm_dlsym(void *hand, const char *name) {
        void* ret = dlsym(hand, name);
        char msg[1024] = { 0 };
        snprintf(msg, sizeof(msg) - 1, "0x%x", ret);
        LOGD("%s = %s\n", name, msg);
        return ret;
    }

    void DalvikMethodReplace::dalvik_setFieldFlag(JNIEnv *env, jobject field) {
        Field* dalvikField = (Field*) env->FromReflectedField(field);
        dalvikField->accessFlags = dalvikField->accessFlags & (~ACC_PRIVATE)
                                   | ACC_PUBLIC;
        LOGD("dalvik_setFieldFlag: %d ", dalvikField->accessFlags);
    }
}

