#ifndef HTFIX_ARCH_BASE_H
#define HTFIX_ARCH_BASE_H

#define FUNCTION_START(x) \
.text; \
.align 4; \
.global x; \
x: \

#define FUNCTION_START_T(x) \
.syntax unified; \
.text; \
.align 4; \
.thumb; \
.thumb_func; \
.global x; \
x: \

#define FUNCTION_END(x) .size x, .-x
#define Code unsigned char *

#define REPLACEMENT_HOOK_TRAMPOLINE replacement_hook_trampoline
#define INLINE_HOOK_TRAMPOLINE inline_hook_trampoline
#define DIRECT_JUMP_TRAMPOLINE direct_jump_trampoline
#define CALL_ORIGIN_TRAMPOLINE call_origin_trampoline

#define INLINE_HOOK_TRAMPOLINE_T inline_hook_trampoline_t
#define DIRECT_JUMP_TRAMPOLINE_T direct_jump_trampoline_t
#define CALL_ORIGIN_TRAMPOLINE_T call_origin_trampoline_t

#endif //HTFIX_ARCH_BASE_H
