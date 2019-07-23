#include <jni.h>
#include "../base/arch_base.h"
#include "../base/arch.h"

#ifndef HTFIX_METHODTRAMPOLINE_H
#define HTFIX_METHODTRAMPOLINE_H

class MethodTrampoline {
public:
    Code code;
    virtual void init();
    void setExecuteSpace(Code start);
    void setEntryCodeOffset(Size offSet);
    void codeCopy(Code src, Size targetOffset, Size len);
    bool flushCache(Size addr, Size len);
    bool isThumbCode();
    Code getThumbCodeAddress(Code code);
    Code getThumbCodePcAddress(Code code);
    Code getCode();
    Size getCodeLen();
    bool isBigEnd(void);
    void* getEntryCodeAddr(void* method);
    Code getEntryCode(void* method);
protected:
    virtual Size codeLength() = 0;
    virtual Code templateCode() = 0;
private:
    Code tempCode;
    Size codeLen;
    Size codeEntryOffSet;
    bool isThumb = false;
};

class MethodReplacementTrampoline : public MethodTrampoline {
public:
    void initTrampoline();
    void setHookMethod(Code hookMethod);
protected:
    Size codeLength() override;
    Code templateCode() override;
private:
    size_t replacementHookTrampolineSize;
    size_t replacementArtMethodOffset;
    size_t replacementCodeEntryOffset;
};
#endif //HTFIX_METHODTRAMPOLINE_H
