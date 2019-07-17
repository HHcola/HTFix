#ifndef HTFix_CAST_COMPILER_OPTIONS_H
#define HTFix_CAST_COMPILER_OPTIONS_H

#include "../base/arch.h"
#include "art_compiler_options.h"
#include "../base/cast.h"

namespace HTFix {

    class CastCompilerOptions {
    public:
        static void init(JNIEnv *jniEnv);
        static IMember<art::CompilerOptions, size_t>* inlineMaxCodeUnits;
    };


}

#endif //SANDHOOK_CAST_COMPILER_OPTIONS_H


