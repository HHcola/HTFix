//
// Created by timothyhe on 2019/7/11.
//

#ifndef HTFIX_METHODHOOK_H
#define HTFIX_METHODHOOK_H


namespace HTFix {
    class MethodHook {
    public:
        int doHookMethod(void *targetMethod, void *hookMethod);
        void ensureMethodCached(void *hookMethod);

    private:
        size_t kDexCacheMethodCacheSize = 1024;
    };
}
#endif //HTFIX_METHODHOOK_H
