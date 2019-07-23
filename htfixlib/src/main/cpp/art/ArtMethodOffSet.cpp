#include "../base/base.h"
#include "ArtMethodOffSet.h"
#include "../base/utils.h"
#include "../includes/never_call.h"
#include "../fake/HideApi.h"

extern int SDK_INT;

namespace HTFix {

    class CastDexCacheResolvedMethods : public ArrayMember<art::mirror::ArtMethod, void *> {
    protected:
        Size calOffset(JNIEnv *jniEnv, art::mirror::ArtMethod *p) override {
            if (SDK_INT >= ANDROID_P)
                return getParentSize() + 1;
            int offset = 0;
            Size addr = getAddressFromJava(jniEnv, "com/htfixlib/offset/HTFixMethodResolver",
                                           "resolvedMethodsAddress");
            if (addr != 0) {
                offset = findOffset(p, getParentSize(), 2, addr);
                if (offset >= 0) {
                    return static_cast<Size>(offset);
                }
            }
            if (SDK_INT == ANDROID_M) {
                return 4;
            } else if (SDK_INT >= ANDROID_L && SDK_INT <= ANDROID_L2) {
                return 4 * 3;
            }
            // get error address of ANDROID_P and ANDROID_Q
            return getParentSize() + 1;
        }

    public:
        Size arrayStart(art::mirror::ArtMethod *parent) override {
            void *p = IMember<art::mirror::ArtMethod, void *>::get(parent);
            if (SDK_INT <= ANDROID_M) {
                return reinterpret_cast<Size>(p) + 4 * 3;
            } else {
                return reinterpret_cast<Size>(p);
            }
        }

    };

    class CastEntryPointFormInterpreter : public IMember<art::mirror::ArtMethod, void *> {
    protected:
        Size calOffset(JNIEnv *jniEnv, art::mirror::ArtMethod *p) override {
            if (SDK_INT >= ANDROID_L2 && SDK_INT <= ANDROID_M)
                return getParentSize() - 3 * BYTE_POINT;
            else if (SDK_INT <= ANDROID_L) {
                Size addr = getAddressFromJava(jniEnv, "com/htfixlib/offset/HTFixMethodResolver",
                                               "entryPointFromInterpreter");
                int offset = 0;
                if (addr != 0) {
                    offset = findOffset(p, getParentSize(), 2, addr);
                    if (offset >= 0) {
                        return static_cast<Size>(offset);
                    }
                }
                return getParentSize() - 4 * 8 - 4 * 4;
            } else {
                return getParentSize() + 1;
            }
        }
    };

    class CastEntryPointQuickCompiled : public IMember<art::mirror::ArtMethod, void *> {
    protected:
        Size calOffset(JNIEnv *jniEnv, art::mirror::ArtMethod *p) override {
            if (SDK_INT >= ANDROID_M) {
                return getParentSize() - BYTE_POINT;
            } else if (SDK_INT <= ANDROID_L) {
                Size addr = getAddressFromJava(jniEnv, "com/htfixlib/offset/HTFixMethodResolver",
                                               "entryPointFromCompiledCode");
                int offset = 0;
                if (addr != 0) {
                    offset = findOffset(p, getParentSize(), 2, addr);
                    if (offset >= 0) {
                        return static_cast<Size>(offset);
                    }
                }
                return getParentSize() - 4 - 2 * BYTE_POINT;
            } else {
                // get error address of ANDROID_P and ANDROID_Q
                return getParentSize() - 4 - 2 * BYTE_POINT;
            }
        }
    };

    class CastEntryPointFromJni : public IMember<art::mirror::ArtMethod, void *> {
    protected:
        Size calOffset(JNIEnv *jniEnv, art::mirror::ArtMethod *p) override {
            Size jniAddr = reinterpret_cast<Size>(Java_com_htfixlib_offset_NeverCallClass_neverCallNative);
            int offset = findOffset(p, getParentSize(), 2, jniAddr);
            if (offset >= 0) {
                return static_cast<Size>(offset);
            }
            if (SDK_INT >= ANDROID_L2 && SDK_INT <= ANDROID_N) {
                return getParentSize() - 2 * BYTE_POINT;
            } else {
                return getParentSize() - 8 * 2 - 4 * 4;
            }
        }
    };


    class CastAccessFlag : public IMember<art::mirror::ArtMethod, uint32_t> {
    protected:
        Size calOffset(JNIEnv *jniEnv, art::mirror::ArtMethod *p) override {
            uint32_t accessFlag = getIntFromJava(jniEnv, "com/htfixlib/utils/ArtMethodUtils",
                                                 "testAccessFlag");
            if (accessFlag == 0) {
                accessFlag = 524313;
                //kAccPublicApi
                if (SDK_INT >= ANDROID_Q) {
                    accessFlag |= 0x10000000;
                }
            }
            int offset = findOffset(p, getParentSize(), 2, accessFlag);
            if (offset < 0) {
                if (SDK_INT >= ANDROID_N) {
                    return 4;
                } else if (SDK_INT == ANDROID_L2) {
                    return 20;
                } else if (SDK_INT == ANDROID_L) {
                    return 56;
                } else {
                    return getParentSize() + 1;
                }
            } else {
                return static_cast<size_t>(offset);
            }
        }
    };

    class CastShadowClass : public IMember<art::mirror::ArtMethod, GCRoot> {
    protected:
        Size calOffset(JNIEnv *jniEnv, art::mirror::ArtMethod *p) override {
            if (SDK_INT < ANDROID_N)
                return getParentSize() + 1;
            return 0;
        }
    };


    class CastDexMethodIndex : public IMember<art::mirror::ArtMethod, uint32_t> {
    protected:
        Size calOffset(JNIEnv *jniEnv, art::mirror::ArtMethod *p) override {
            if (SDK_INT >= ANDROID_P) {
                return CastArtMethod::accessFlag->getOffset()
                       + CastArtMethod::accessFlag->size()
                       + sizeof(uint32_t);
            }
            int offset = 0;
            jint index = getIntFromJava(jniEnv, "com/htfixlib/offset/HTFixMethodResolver",
                                        "dexMethodIndex");
            if (index != 0) {
                offset = findOffset(p, getParentSize(), 2, static_cast<uint32_t>(index));
                if (offset >= 0) {
                    return static_cast<Size>(offset);
                }
            }
            return getParentSize() + 1;
        }
    };

    class CastHotnessCount : public IMember<art::mirror::ArtMethod, uint16_t> {
    protected:
        Size calOffset(JNIEnv *jniEnv, art::mirror::ArtMethod *p) override {
            if (SDK_INT <= ANDROID_N)
                return getParentSize() + 1;
            return CastArtMethod::dexMethodIndex->getOffset()
                   + CastArtMethod::dexMethodIndex->size()
                   + sizeof(uint16_t);
        }
    };


    void CastArtMethod::init(JNIEnv *env) {
        //init ArtMethodSize
        jclass sizeTestClass = env->FindClass("com/htfixlib/ArtMethodSize");
        Size artMethod1 = (Size) env->GetStaticMethodID(sizeTestClass, "method1", "()V");
        Size artMethod2 = (Size) env->GetStaticMethodID(sizeTestClass, "method2", "()V");

        // get artmethod size
        size = artMethod2 - artMethod1;
        LOGD("setArtMethodSize size = %d", size);

        art::mirror::ArtMethod *m1 = reinterpret_cast<art::mirror::ArtMethod *>(artMethod1);
        art::mirror::ArtMethod *m2 = reinterpret_cast<art::mirror::ArtMethod *>(artMethod2);

        //init Members
        entryPointQuickCompiled = new CastEntryPointQuickCompiled();
        entryPointQuickCompiled->init(env, m1, size);
        LOGD("entryPointQuickCompiled offset = %d", entryPointQuickCompiled->getOffset());

        accessFlag = new CastAccessFlag();
        accessFlag->init(env, m1, size);

        entryPointFromInterpreter = new CastEntryPointFormInterpreter();
        entryPointFromInterpreter->init(env, m1, size);

        dexCacheResolvedMethods = new CastDexCacheResolvedMethods();
        dexCacheResolvedMethods->init(env, m1, size);

        dexMethodIndex = new CastDexMethodIndex();
        dexMethodIndex->init(env, m1, size);

        declaringClass = new CastShadowClass();
        declaringClass->init(env, m1, size);

        hotnessCount = new CastHotnessCount();
        hotnessCount->init(env, m1, size);

        jclass neverCallTestClass = env->FindClass("com/htfixlib/offset/NeverCallClass");


        art::mirror::ArtMethod *neverCall = reinterpret_cast<art::mirror::ArtMethod *>(env->GetMethodID(
                neverCallTestClass, "neverCall", "()V"));
        art::mirror::ArtMethod *neverCall2 = reinterpret_cast<art::mirror::ArtMethod *>(env->GetMethodID(
                neverCallTestClass, "neverCall2", "()V"));

        bool beAot =
                entryPointQuickCompiled->get(neverCall) != entryPointQuickCompiled->get(neverCall2);
        if (beAot) {
            quickToInterpreterBridge = getInterpreterBridge(false);
            if (quickToInterpreterBridge == nullptr) {
                quickToInterpreterBridge = entryPointQuickCompiled->get(neverCall);
                canGetInterpreterBridge = false;
            }
        } else {
            quickToInterpreterBridge = entryPointQuickCompiled->get(neverCall);
        }

        entryPointFromJNI = new CastEntryPointFromJni();
        entryPointFromJNI->init(env, neverCall, size);

        art::mirror::ArtMethod *neverCallStatic = reinterpret_cast<art::mirror::ArtMethod *>(env->GetStaticMethodID(
                neverCallTestClass, "neverCallStatic", "()V"));
        staticResolveStub = entryPointQuickCompiled->get(neverCallStatic);
    }

    void CastArtMethod::copy(art::mirror::ArtMethod *from, art::mirror::ArtMethod *to) {
        memcpy(to, from, size);
    }

    Size CastArtMethod::size = 0;
    IMember<art::mirror::ArtMethod, void *> *CastArtMethod::entryPointQuickCompiled = nullptr;
    IMember<art::mirror::ArtMethod, void *> *CastArtMethod::entryPointFromInterpreter = nullptr;
    IMember<art::mirror::ArtMethod, void *> *CastArtMethod::entryPointFromJNI = nullptr;
    ArrayMember<art::mirror::ArtMethod, void *> *CastArtMethod::dexCacheResolvedMethods = nullptr;
    IMember <art::mirror::ArtMethod, uint32_t> *CastArtMethod::dexMethodIndex = nullptr;
    IMember <art::mirror::ArtMethod, uint32_t> *CastArtMethod::accessFlag = nullptr;
    IMember <art::mirror::ArtMethod, GCRoot> *CastArtMethod::declaringClass = nullptr;
    IMember <art::mirror::ArtMethod, uint16_t> *CastArtMethod::hotnessCount = nullptr;
    void *CastArtMethod::quickToInterpreterBridge = nullptr;
    void *CastArtMethod::genericJniStub = nullptr;
    void *CastArtMethod::staticResolveStub = nullptr;
    bool CastArtMethod::canGetInterpreterBridge = true;
    bool CastArtMethod::canGetJniBridge = true;

}