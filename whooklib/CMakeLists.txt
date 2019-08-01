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
add_definitions(-std=c++11)

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/)
include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include/)
include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/libffi/)
include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/libffi/arm/)

ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/src/main/cpp/)