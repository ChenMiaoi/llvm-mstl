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

set(GTEST_VERSION "v1.14.0")
set(GTEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/deps/gtest)

function(update_gtest_version SUBMODULE_PATH SUBMODULE_TAG) 
    execute_process(
        COMMAND git -C ${SUBMODULE_PATH} describe --tags --abbrev=0
        OUTPUT_VARIABLE GTEST_CURRENT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE GIT_RESULT
    )
    if(NOT GTEST_CURRENT_VERSION STREQUAL GTEST_VERSION)
        execute_process(
            COMMAND git -C ${SUBMODULE_PATH} checkout ${SUBMODULE_TAG}
            RESULT_VARIABLE GIT_RESULT
        )
        message(STATUS "Updating Git submodule ${SUBMODULE_PATH} to ${SUBMODULE_TAG}")
    else()
        message(STATUS "Git submodule GTest is ${SUBMODULE_TAG} now")
    endif()
    
    if(NOT GIT_RESULT EQUAL "0")
        message(FATAL_ERROR "Failed to update Git submodule ${SUBMODULE_PATH} to ${SUBMODULE_TAG}")
    endif()
endfunction()  

if(NOT EXISTS ${GTEST_ROOT})
    set(GTEST_DOWNLOAD_COMMAND
        "git submodule deinit -f deps/gtest"
        "git submodule init deps/gtest"
        "git submodule update --remote --checkout deps/gtest"
    )

    execute_process(
        COMMAND ${GTEST_DOWNLOAD_COMMAND}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE GTEST_RESULT
    )

    update_gtest_version(${GTEST_ROOT} ${GTEST_VERSION})

    if(GTEST_RESULT)
        message(FATAL_ERROR "Failed to download and update Google Test.")
    endif()
else()
    update_gtest_version(${GTEST_ROOT} ${GTEST_VERSION})
endif()

option(gtest_force_shared_crt "Use shared (DLL) run-time lib even when Google Test is built as static lib." ON)

add_subdirectory(${GTEST_ROOT})
include_directories(${gtest_SOURCE_DIR}/include)

set(CMAKE_C_COMPILER ${CLANG_EXECUTABLE})
set(CMAKE_CXX_COMPILER ${CLANGXX_EXECUTABLE})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

list(APPEND LLVM_MSTL_FLAGS -pedantic
    -Wall -Wextra -Werror -Wwrite-strings
    -Wshadow -Winit-self -Wcast-align -Wformat=2 -Wmissing-prototypes
    -Wstrict-overflow=2 -Wcast-qual -Wundef -Wswitch-default -Wconversion
    -Wc++-compat -fstack-protector-strong -Wcomma -Wdouble-promotion -Wparentheses
    -Wunused-macros -Wmissing-variable-declarations -Wused-but-marked-unused
    -Wswitch-enum
  )

set(LLVM_MSTL_INCLUDE_ROOT ${CMAKE_SOURCE_DIR}/include)
set(LLVM_MSTL_SRC_ROOT ${CMAKE_SOURCE_DIR}/src)
set(CTEST_SOURCE_DIRECTORY "${CMAKE_BINARY_DIR}/tests")
set(CTEST_BINARY_DIRECTORY "${CMAKE_BINARY_DIR}/tests")

file(GLOB_RECURSE SOURCE_INCLUDES ${LLVM_MSTL_INCLUDE_ROOT}/*)
file(GLOB_RECURSE SOURCE ${LLVM_MSTL_SRC_ROOT}/*)

add_library(mstl SHARED ${SOURCE_INCLUDES} ${SOURCE})
include_directories(${LLVM_MSTL_INCLUDE_ROOT})
include_directories(${LLVM_MSTL_SRC_ROOT})

enable_testing()
include(CTest)

add_subdirectory(tests)

add_compile_options(${LLVM_MSTL_FLAGS})
add_executable(llvm-mstl main.cc)
target_link_libraries(llvm-mstl mstl)
