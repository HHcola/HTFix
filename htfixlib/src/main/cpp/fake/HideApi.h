//
// Created by timothyhe on 2019/7/16.
//

#ifndef HTFIX_HIDEAPI_H
#define HTFIX_HIDEAPI_H

#include "dlfcn.h"
#include <memory>
#include "../base/arch.h"
#include "../includes/art_jit.h"
#include "../includes/art_compiler_options.h"

extern "C" {

void initHideApi(JNIEnv *env);
bool compileMethod(void *artMethod, void *thread);

void suspendVM();
void resumeVM();

bool canGetObject();
jobject getJavaObject(JNIEnv* env, void* thread, void* address);

art::jit::JitCompiler* getGlobalJitCompiler();

art::CompilerOptions* getCompilerOptions(art::jit::JitCompiler* compiler);

art::CompilerOptions* getGlobalCompilerOptions();

bool disableJitInline(art::CompilerOptions* compilerOptions);

void* getInterpreterBridge(bool isNative);

bool replaceUpdateCompilerOptionsQ();

bool forceProcessProfiles();

}
#endif //HTFIX_HIDEAPI_H
