#ifndef HTFIX_ARCH_H
#define HTFIX_ARCH_H

#include "base.h"

#define BYTE_POINT sizeof(void*)

typedef size_t Size;

//32bit
#if defined(__i386__) || defined(__arm__)
//64bit
#elif defined(__aarch64__) || defined(__x86_64__)
#else
#endif

#if defined(__arm__)
static void clearCacheArm32(char* begin, char *end)
{
    const int syscall = 0xf0002;
    __asm __volatile (
        "mov     r0, %0\n"
        "mov     r1, %1\n"
        "mov     r3, %2\n"
        "mov     r2, #0x0\n"
        "svc     0x00000000\n"
        :
        :    "r" (begin), "r" (end), "r" (syscall)
        :    "r0", "r1", "r3"
        );
}
#endif


#endif //HTFIX_ARCH_H