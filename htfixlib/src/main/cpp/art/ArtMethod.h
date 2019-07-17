//
// Created by timothyhe on 2019/7/9.
//

#ifndef HTFIX_ARTMETHOD_H
#define HTFIX_ARTMETHOD_H

#include "../base/base.h"
#include "../base/arch.h"

#define GCRoot uint32_t

namespace art {
    namespace mirror {
        class Object {
        public:
        };
        class Class: public Object {
        public:
        };

        class ArtField {
        public:
        };

        class ArtMethod {
        public:

            bool isAbstract();
            bool isNative();
            bool isStatic();
            bool isCompiled();
            bool isThumbCode();

            void setAccessFlags(uint32_t flags);
            void disableCompilable();
            void tryDisableInline();
            void disableInterpreterForO();
            void setPrivate();
            void setStatic();
            void setNative();

            void setQuickCodeEntry(void* entry);
            void setJniCodeEntry(void* entry);
            void setInterpreterCodeEntry(void* entry);
            void setDexCacheResolveList(void* list);
            void setDexCacheResolveItem(uint32_t index, void* item);
            void setDeclaringClass(GCRoot classPtr);
            void setHotnessCount(uint16_t count);

            void* getQuickCodeEntry();
            void* getInterpreterCodeEntry();
            uint32_t getAccessFlags();
            uint32_t getDexMethodIndex();
            GCRoot getDeclaringClass();
            uint16_t getHotnessCount();

            bool compile(JNIEnv* env);
            bool deCompile();
            void flushCache();
            void backup(ArtMethod* backup);

            static Size size();

        };

    }

}

#endif //HTFIX_ARTMETHOD_H
