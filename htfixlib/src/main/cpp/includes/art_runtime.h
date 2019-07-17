#ifndef HTFix_ART_RUNTIME_H
#define HTFix_ART_RUNTIME_H

#include "art_jit.h"

namespace art {
    class Runtime {

    public:
        jit::Jit* getJit();
    };
}

#endif //SANDHOOK_ART_RUNTIME_H
