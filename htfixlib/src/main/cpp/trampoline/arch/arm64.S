#include "../../base/arch_base.h"

#if defined(__aarch64__)

#define Reg0 x17
#define Reg1 x16
#define RegMethod x0

FUNCTION_START(REPLACEMENT_HOOK_TRAMPOLINE)
    ldr RegMethod, addr_art_method
    ldr Reg0, addr_code_entry
    ldr Reg0, [Reg0]
    br Reg0
addr_art_method:
    .long 0
    .long 0
addr_code_entry:
    .long 0
    .long 0
FUNCTION_END(REPLACEMENT_HOOK_TRAMPOLINE)
#define SIZE_JUMP #0x10
FUNCTION_START(DIRECT_JUMP_TRAMPOLINE)
    ldr Reg0, addr_target
    br Reg0
addr_target:
    .long 0
    .long 0
FUNCTION_END(DIRECT_JUMP_TRAMPOLINE)

FUNCTION_START(INLINE_HOOK_TRAMPOLINE)
    ldr Reg0, origin_art_method
    cmp RegMethod, Reg0
    bne origin_code
    ldr RegMethod, hook_art_method
    ldr Reg0, addr_hook_code_entry
    ldr Reg0, [Reg0]
    br Reg0
origin_code:
    .long 0
    .long 0
    .long 0
    .long 0
    ldr Reg0, addr_origin_code_entry
    ldr Reg0, [Reg0]
    add Reg0, Reg0, SIZE_JUMP
    br Reg0
origin_art_method:
    .long 0
    .long 0
addr_origin_code_entry:
    .long 0
    .long 0
hook_art_method:
    .long 0
    .long 0
addr_hook_code_entry:
    .long 0
    .long 0
FUNCTION_END(INLINE_HOOK_TRAMPOLINE)

FUNCTION_START(CALL_ORIGIN_TRAMPOLINE)
    ldr RegMethod, call_origin_art_method
    ldr Reg0, addr_call_origin_code
    br Reg0
call_origin_art_method:
    .long 0
    .long 0
addr_call_origin_code:
    .long 0
    .long 0
FUNCTION_END(CALL_ORIGIN_TRAMPOLINE)

#endif
