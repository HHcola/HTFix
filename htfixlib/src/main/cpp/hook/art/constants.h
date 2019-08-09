//
// Created by timothyhe on 2019/8/9.
//

#ifndef HTFIX_CONSTANTS_H
#define HTFIX_CONSTANTS_H

namespace HTFix {
    namespace Constants {
        static constexpr size_t kAccPublic = 0x0001;  // class, field, method, ic
        static constexpr size_t kAccPrivate = 0x0002;  // field, method, ic
        static constexpr size_t kAccProtected = 0x0004;  // field, method, ic
        static constexpr size_t kAccStatic = 0x0008;  // field, method, ic
        static constexpr size_t kAccNative = 0x0100;  // field, method, ic
        static constexpr size_t kAccFinal = 0x0010;  // class, field, method, ic
        static constexpr size_t kAccSynchronized = 0x0020;  // method (only allowed on natives)
        static constexpr size_t kAccAbstract = 0x0400;  // class, method, ic



        static constexpr const char *kMethodHTFixOne = "htfixNativeOne";
        static constexpr const char *kMethodHTFixTwo = "htfixNativeTwo";
    }
}
#endif //HTFIX_CONSTANTS_H
