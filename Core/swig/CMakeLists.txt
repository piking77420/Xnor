cmake_minimum_required(VERSION 3.27)

project(CoreCSharp VERSION 0.3 LANGUAGES CXX)

set(CORE_PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(SOLUTION_DIR "${CORE_PROJECT_DIR}/..")
set(OUT_DIR "${SOLUTION_DIR}/x64/$(Configuration)")
set(OUT_DIR_DEBUG "${SOLUTION_DIR}/x64/Debug")
set(OUT_DIR_RELEASE "${SOLUTION_DIR}/x64/Release")

if (MSVC)
    set(SWIG_DIR "${SOLUTION_DIR}/SWIG/Lib")
    set(SWIG_EXECUTABLE "${SOLUTION_DIR}/SWIG/swig.exe")
endif (MSVC)

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

set(CORE_PROJECT_INCLUDE_DIR "${CORE_PROJECT_DIR}/include")
set(CORE_PROJECT_SOURCE_DIR "${CORE_PROJECT_DIR}/src")

set(NET_PROJECT_NAME "Game")
set(NET_PROJECT_DIR "${CORE_PROJECT_DIR}/../${NET_PROJECT_NAME}")
set(NET_PROJECT_XNOR_DIR "${NET_PROJECT_DIR}/Xnor")
set(NET_PROJECT_DLL_DIR "${NET_PROJECT_DIR}")

set(CMAKE_SWIG_FLAGS -std=c++23 -namespace Xnor)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

cmake_policy(SET CMP0122 NEW)

set_property(SOURCE "${CORE_PROJECT_INCLUDE_DIR}/core.i" PROPERTY CPLUSPLUS ON)

swig_add_library(CoreCSharp
    TYPE SHARED
    LANGUAGE CSharp
    SOURCES "${CORE_PROJECT_INCLUDE_DIR}/core.i"
    OUTPUT_DIR ${NET_PROJECT_XNOR_DIR}
    OUTFILE_DIR ${CMAKE_BINARY_DIR}
)

set_target_properties(CoreCSharp
    PROPERTIES
    # These copy the shared library on Windows to .Net project directory
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUT_DIR_RELEASE}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUT_DIR_DEBUG}
    
    # This copies the shared library on Linux to .Net project directory
    LIBRARY_OUTPUT_DIRECTORY ${OUT_DIR}

    # Set location of C++ headers
    INCLUDE_DIRECTORIES ${CORE_PROJECT_INCLUDE_DIR}
)

target_include_directories(
    CoreCSharp
    PUBLIC
    "${CORE_PROJECT_DIR}/include"
    "${CORE_PROJECT_DIR}/inline"
    "${CORE_PROJECT_DIR}/externals/include"
)

target_link_directories(CoreCSharp PUBLIC "${SOLUTION_DIR}/x64/")

target_link_libraries(CoreCSharp "Core.lib")

add_compile_definitions("MATH_DEFINE_FORMATTER")
