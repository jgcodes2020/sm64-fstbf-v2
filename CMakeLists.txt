if (NOT DEFINED ENV{ONEAPI_ROOT})
    message(FATAL_ERROR "Please enable the oneAPI environment.")
endif()

if (WIN32)
    set(CMAKE_C_COMPILER "$ENV{CMPLR_ROOT}/bin/icx")
    set(CMAKE_CXX_COMPILER "$ENV{CMPLR_ROOT}/bin/icx")
else()
    set(CMAKE_C_COMPILER "$ENV{CMPLR_ROOT}/bin/icx")
    set(CMAKE_CXX_COMPILER "$ENV{CMPLR_ROOT}/bin/icpx")
endif()

cmake_minimum_required(VERSION 3.27 FATAL_ERROR)
project(sm64-fstbf-v2
    DESCRIPTION "Refactored FST bruteforcer"
    LANGUAGES CXX
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

if (NOT DEFINED CMAKE_BUILD_TYPE)
    option(CMAKE_BUILD_TYPE "Desired build type (Debug/Release/RelWithDebInfo/MinSizeRel)" Release)
endif()

find_package(IntelSYCL REQUIRED)
include(cmake/CompilerFlags.cmake)

add_subdirectory(src)