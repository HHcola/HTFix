# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.
#cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
cmake_minimum_required(VERSION 3.4.1)
project(htfix)

ENABLE_LANGUAGE(ASM)

set(${PROJECT_NAME}_SOURCES
        HTFixHook.cpp
        base/utils.cpp
        fake/HideApi.cpp
        fake/dlfcn_nougat.cpp
        fake/ElfUtil.cpp
        art/ArtMethod.cpp
        art/ArtMethodOffSet.cpp
        art/ArtMethodOffSet.cpp
        art/art_compiler_options.cpp
        art/cast_compiler_options.cpp
        insts/insts_arm32.cpp
        insts/insts_arm64.cpp
        trampoline/arch/arm32.S
        trampoline/arch/arm64.S
        hook/MethodHook.cpp
        hook/MethodTrampoline.cpp
        hook/HTFixMethodHook.cpp
        trampoline/TrampolineManager.cpp
        )

add_definitions(-std=c++11)

add_library(${PROJECT_NAME}
        SHARED
        ${${PROJECT_NAME}_SOURCES})

target_link_libraries(${PROJECT_NAME} log)