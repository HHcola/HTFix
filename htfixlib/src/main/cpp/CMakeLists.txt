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
        base/utils.cpp
        trampoline/arch/arm32.S
        trampoline/arch/arm64.S
        hook/art/MethodHook.cpp
        hook/art/MethodTrampoline.cpp
        hook/dalvik/DalvikMethodReplace.cpp
        hook/HTFixMethodHook.cpp
        )

add_definitions(-std=c++11)

add_library(${PROJECT_NAME}
        SHARED
        ${${PROJECT_NAME}_SOURCES})

target_link_libraries(${PROJECT_NAME} log)