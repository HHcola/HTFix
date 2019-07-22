//
// Created by Administrator on 2019/7/21.
//
#include <unistd.h>
#include "MethodTrampoline.h"
#include "../base/arch_base.h"

extern "C" void DIRECT_JUMP_TRAMPOLINE();
extern "C" void INLINE_HOOK_TRAMPOLINE();
extern "C" void REPLACEMENT_HOOK_TRAMPOLINE();
extern "C" void CALL_ORIGIN_TRAMPOLINE();

#if defined(__arm__)
#include <unistd.h>
extern "C" void DIRECT_JUMP_TRAMPOLINE_T();
extern "C" void INLINE_HOOK_TRAMPOLINE_T();
extern "C" void CALL_ORIGIN_TRAMPOLINE_T();
#endif

void MethodTrampoline::init() {
    codeLen = codeLength();
    tempCode = templateCode();
}

void MethodTrampoline::setExecuteSpace(unsigned char *start) {
    code = start;
    LOGD("setExecuteSpace start = 0x%x, tempCode = 0x%x, codeLen = %d", start, tempCode, codeLen);
    memcpy(code, tempCode, codeLen);
    flushCache(reinterpret_cast<Size>(code), codeLen);
}

void MethodTrampoline::setEntryCodeOffset(Size offSet) {
    this->codeEntryOffSet = offSet;
    LOGD("setEntryCodeOffset codeEntryOffSet = %d", codeEntryOffSet);
}

void MethodTrampoline::codeCopy(unsigned char *src, Size targetOffset, Size len) {
    memcpy(reinterpret_cast<void*>((Size)code + targetOffset), src, len);
    flushCache((Size)code + targetOffset, len);
}

bool MethodTrampoline::flushCache(Size addr, Size len) {
#if defined(__arm__)
    //clearCacheArm32(reinterpret_cast<char*>(addr), reinterpret_cast<char*>(addr + len));
    int i = cacheflush(addr, addr + len, 0);
    if (i == -1) {
        return false;
    }
    return true;
#elif defined(__aarch64__)
    char *begin = reinterpret_cast<char *>(addr);
            __builtin___clear_cache(begin, begin + len);
#endif
    return true;
}

unsigned char *MethodTrampoline::getCode() {
    if (isThumbCode()) {
        return getThumbCodePcAddress(code);
    } else {
        return code;
    }
}

bool MethodTrampoline::isThumbCode() {
    return isThumb;
}

unsigned char *MethodTrampoline::getThumbCodeAddress(unsigned char *code) {
    Size addr = reinterpret_cast<Size>(code) & (~0x00000001);
    return reinterpret_cast<Code>(addr);;
}

unsigned char *MethodTrampoline::getThumbCodePcAddress(unsigned char *code) {
    Size addr = reinterpret_cast<Size>(code) & (~0x00000001);
    return reinterpret_cast<Code>(addr + 1);
}

Size MethodTrampoline::getCodeLen() {
    LOGD("MethodReplacementTrampoline getCodeLen = %d",codeLen);
    return codeLen;
}

bool MethodTrampoline::isBigEnd(void) {
    int i = 1;
    unsigned char *pointer;
    pointer = (unsigned char *) &i;
    return *pointer == 0;
}

void *MethodTrampoline::getEntryCodeAddr(void *method) {
    return reinterpret_cast<void*>((Size)method + codeEntryOffSet);
}

unsigned char *MethodTrampoline::getEntryCode(void *method) {
    Code entryCode = *reinterpret_cast<Code*>((Size)method + codeEntryOffSet);
    return entryCode;
}

void MethodReplacementTrampoline::initTrampoline() {
#if defined(__arm__)
    replacementHookTrampolineSize = 4 * 5;
    replacementArtMethodOffset =  4 * 3;
    replacementCodeEntryOffset =  4 * 4;
#elif defined(__aarch64__)
    replacementHookTrampolineSize = 4 * 8;
replacementArtMethodOffset =  4 * 4;
replacementCodeEntryOffset =  4 * 6;
#else
#endif

}

void MethodReplacementTrampoline::setHookMethod(unsigned char *hookMethod) {
    LOGD("setHookMethod hookMethod = 0x%x, replacementArtMethodOffset = %d, replacementCodeEntryOffset = %d", hookMethod, replacementArtMethodOffset, replacementCodeEntryOffset);
    codeCopy(reinterpret_cast<Code>(&hookMethod), replacementArtMethodOffset, BYTE_POINT);
    void* codeEntry = getEntryCodeAddr(hookMethod);
    codeCopy(reinterpret_cast<Code>(&codeEntry), replacementCodeEntryOffset, BYTE_POINT);
}

Size MethodReplacementTrampoline::codeLength() {
    LOGD("MethodReplacementTrampoline codeLength = %d",replacementHookTrampolineSize);
    return replacementHookTrampolineSize;
}

unsigned char *MethodReplacementTrampoline::templateCode() {
    return reinterpret_cast<Code>(REPLACEMENT_HOOK_TRAMPOLINE);
}
