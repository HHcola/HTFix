
#include "../includes/art_compiler_options.h"
#include "../includes/cast_compiler_options.h"
#include "../fake/HideApi.h"

using namespace HTFix;
using namespace art;

extern int SDK_INT;

size_t CompilerOptions::getInlineMaxCodeUnits() {
    if (SDK_INT < ANDROID_N)
        return 0;
    return CastCompilerOptions::inlineMaxCodeUnits->get(this);
}

bool CompilerOptions::setInlineMaxCodeUnits(size_t units) {
    if (SDK_INT < ANDROID_N)
        return false;
    CastCompilerOptions::inlineMaxCodeUnits->set(this, units);
    return true;
}
