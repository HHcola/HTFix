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

include_directories(${CMAKE_SOURCE_DIR}/)

set(${PROJECT_NAME}_SOURCES
        android/art/art_jni_trampoline.cc
        android/art/art_method.cc
        android/art/art_runtime.cc
        android/art/art_symbol_resolver.cc
        android/art/java_types.cc
        android/art/scoped_thread_state_change.cc
        android/art/well_known_classes.cc
        android/art/native_on_load.cc
        whale.cc
        interceptor.cc
        platform/memory.cc
        platform/linux/elf_image.cc
        platform/linux/process_map.cc

        dbi/hook_common.cc
        dbi/instruction_set.cc
        dbi/arm/decoder_arm.cc
        dbi/arm/decoder_thumb.cc
        dbi/arm/inline_hook_arm.cc
        dbi/arm/instruction_rewriter_arm.cc
        )

add_definitions(-std=c++11)

add_library(${PROJECT_NAME}
        SHARED
        ${${PROJECT_NAME}_SOURCES})

target_link_libraries(${PROJECT_NAME} log)