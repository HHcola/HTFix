#ifndef HTFIX_ARTMETHODOFFSET_H
#define HTFIX_ARTMETHODOFFSET_H

#include "../base/cast.h"
#include "ArtMethod.h"

namespace HTFix {

    class CastArtMethod {
    public:
        static Size size;
        static IMember<art::mirror::ArtMethod, void*>* entryPointQuickCompiled;
        static IMember<art::mirror::ArtMethod, void*>* entryPointFromInterpreter;
        static IMember<art::mirror::ArtMethod, void*>* entryPointFromJNI;
        static ArrayMember<art::mirror::ArtMethod,void*>* dexCacheResolvedMethods;
        static IMember<art::mirror::ArtMethod, uint32_t>* dexMethodIndex;
        static IMember<art::mirror::ArtMethod, uint32_t>* accessFlag;
        static IMember<art::mirror::ArtMethod, GCRoot>* declaringClass;
        static IMember<art::mirror::ArtMethod, uint16_t>* hotnessCount;
        static void* quickToInterpreterBridge;
        static void* staticResolveStub;
        static void* genericJniStub;
        static bool canGetJniBridge;
        static bool canGetInterpreterBridge;

        static void init(JNIEnv *env);
        static void copy(art::mirror::ArtMethod* from, art::mirror::ArtMethod* to);

    };

}
#endif //HTFIX_ARTMETHODOFFSET_H
