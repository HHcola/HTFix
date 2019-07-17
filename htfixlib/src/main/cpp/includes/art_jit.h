//
// Created by 甘尧 on 2019/2/23.
//

#ifndef HTFIX_ART_JIT_H
#define HTFIX_ART_JIT_H

#include <mutex>
#include <stdint.h>
#include "art_compiler_options.h"
namespace art {
    namespace jit {

        //7.0 - 9.0
        class JitCompiler {
        public:
            virtual ~JitCompiler();
            std::unique_ptr<art::CompilerOptions> compilerOptions;
        };

        class Jit {
        public:
            //void* getCompilerOptions();
        };



    };
}

#endif //SANDHOOK_ART_JIT_H
