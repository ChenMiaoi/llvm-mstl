cmake_minimum_required(VERSION 3.7)
project(llvm-mstl VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
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
