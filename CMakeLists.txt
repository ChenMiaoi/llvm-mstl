cmake_minimum_required(VERSION 3.7)
project(llvm-mstl VERSION 0.1.0 LANGUAGES C CXX)

# Find Clang
find_program(CLANG_EXECUTABLE clang REQUIRED)
if(NOT CLANG_EXECUTABLE)
    message(FATAL_ERROR "Clang compiler not found.")
endif()

# Find Clang++
find_program(CLANGXX_EXECUTABLE clang++ REQUIRED)
if(NOT CLANGXX_EXECUTABLE)
    message(FATAL_ERROR "Clang++ compiler not found.")
endif()

# Execute Clang command to get version
execute_process(COMMAND ${CLANG_EXECUTABLE} --version OUTPUT_VARIABLE _clang_version_output)
execute_process(COMMAND ${CLANGXX_EXECUTABLE} --version OUTPUT_VARIABLE _clangxx_version_output)

# Extract Clang version using regex
string(REGEX MATCH "[0-9]+\\.[0-9]+" _clang_version "${_clang_version_output}")
string(REGEX MATCH "[0-9]+\\.[0-9]+" _clangxx_version "${_clangxx_version_output}")

# Check Clang version
if(_clang_version VERSION_GREATER_EQUAL "14")
    message(STATUS "Found Clang version: ${_clang_version}")
else()
    message(FATAL_ERROR "Clang version must be greater than or equal to 14.")
endif()

if(_clang_version VERSION_GREATER_EQUAL "14")
    message(STATUS "Found Clang version: ${_clangxx_version}")
else()
    message(FATAL_ERROR "Clang version must be greater than or equal to 14.")
endif()

set(CMAKE_C_COMPILER ${CLANG_EXECUTABLE})
set(CMAKE_CXX_COMPILER ${CLANGXX_EXECUTABLE})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(LLVM_MSTL_INCLUDE_ROOT ${CMAKE_SOURCE_DIR}/include)
set(LLVM_MSTL_SRC_ROOT ${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCE_INCLUDES ${LLVM_MSTL_INCLUDE_ROOT}/*)
file(GLOB_RECURSE SOURCE ${LLVM_MSTL_SRC_ROOT}/*)

add_library(mstl SHARED ${SOURCE_INCLUDES} ${SOURCE})
include_directories(${LLVM_MSTL_INCLUDE_ROOT})
include_directories(${LLVM_MSTL_SRC_ROOT})

add_executable(llvm-mstl main.cc)
target_link_libraries(llvm-mstl mstl)
