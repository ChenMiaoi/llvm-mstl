cmake_minimum_required(VERSION 3.7.0)
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
set(SPDLOG_VERSION "v1.14.1")
set(SPDLOG_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/deps/spdlog)

function(update_module_version SUBMODULE_PATH SUBMODULE_TAG) 
    execute_process(
        COMMAND git -C ${SUBMODULE_PATH} describe --tags --abbrev=0
        OUTPUT_VARIABLE MODULE_CURRENT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE GIT_RESULT
    )
    if(NOT MODULE_CURRENT_VERSION STREQUAL SUBMODULE_TAG)
        execute_process(
            COMMAND git -C ${SUBMODULE_PATH} checkout ${SUBMODULE_TAG}
            RESULT_VARIABLE GIT_RESULT
        )
        message(STATUS "Updating Git submodule ${SUBMODULE_PATH} to ${SUBMODULE_TAG}")
    else()
        message(STATUS "Git submodule ${SUBMODULE_PATH} is ${SUBMODULE_TAG} now")
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

    update_module_version(${GTEST_ROOT} ${GTEST_VERSION})

    if(GTEST_RESULT)
        message(FATAL_ERROR "Failed to download and update Google Test.")
    endif()
else()
    update_module_version(${GTEST_ROOT} ${GTEST_VERSION})
endif()
add_subdirectory(${GTEST_ROOT})
include_directories(${gtest_SOURCE_DIR}/include)

if(NOT EXISTS ${SPDLOG_ROOT})
set(GTEST_DOWNLOAD_COMMAND
        "git submodule deinit -f deps/spdlog"
        "git submodule init deps/spdlog"
        "git submodule update --remote --checkout deps/spdlog"
    )

    execute_process(
        COMMAND ${GTEST_DOWNLOAD_COMMAND}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE SPDLOG_RESULT
    )

    update_module_version(${SPDLOG_ROOT} ${SPDLOG_VERSION})

    if(SPDLOG_RESULT)
        message(FATAL_ERROR "Failed to download and update spdlog module.")
    endif()
else()
    update_module_version(${SPDLOG_ROOT} ${SPDLOG_VERSION})
endif()
add_subdirectory(${SPDLOG_ROOT})
include_directories(${SPDLOG_ROOT}/include)

option(gtest_force_shared_crt "Use shared (DLL) run-time lib even when Google Test is built as static lib." ON)

# if I use `add_subdirectory(gtest)` after here, caused a loop call

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
    -Wswitch-enum -g
  )

message(STATUS "Current Operator System: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Current Operator System Kernel: ${CMAKE_SYSTEM_VERSION}")
message(STATUS "Current Operator System Arch: ${CMAKE_SYSTEM_PROCESSOR}")

# TODO: memory check tools or library
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND WIN32)
    # get_filename_component(CLANG_PATH "${CLANG_EXECUTABLE}" DIRECTORY)
    # string(REGEX MATCH ".*LLVM.*" LLVM_CLANG_PATH "${CLANG_PATH}")
    # if (LLVM_CLANG_PATH)
    #     string(REGEX REPLACE "/bin$" "" LLVM_PATH "${LLVM_CLANG_PATH}")
    #     message(STATUS "Found LLVM PATH: ${LLVM_PATH}")
    #     file(GLOB_RECURSE ASAN_LIBS "${LLVM_PATH}/*/clang_rt.asan_dynamic*.lib")
    #     if (ASAN_LIBS)
    #         message(STATUS "${ASAN_LIBS}")
    #         message(STATUS "Found libasan")
    #         list(APPEND LLVM_MSTL_FLAGS -fsanitize=address -fno-omit-frame-pointer -fsanitize-recover=address)
    #     else ()
    #         message(STATUS "NOT Found libasan")
    #     endif ()
    # else()
    #     message(FATAL_ERROR "can't find LLVM Path")
    # endif()
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND LLVM_MSTL_FLAGS -fsanitize=address -fsanitize-recover=address -fno-stack-protector 
    -fno-omit-frame-pointer -static-libsan)
endif()

set(LLVM_MSTL_INCLUDE_ROOT ${CMAKE_SOURCE_DIR}/include)
set(LLVM_MSTL_SRC_ROOT ${CMAKE_SOURCE_DIR}/src)
set(CTEST_SOURCE_DIRECTORY "${CMAKE_BINARY_DIR}/tests")
set(CTEST_BINARY_DIRECTORY "${CMAKE_BINARY_DIR}/tests")

file(GLOB_RECURSE SOURCE_INCLUDES ${LLVM_MSTL_INCLUDE_ROOT}/*)
file(GLOB_RECURSE SOURCE ${LLVM_MSTL_SRC_ROOT}/*)

add_library(mstl SHARED ${SOURCE_INCLUDES} ${SOURCE})
include_directories(${LLVM_MSTL_INCLUDE_ROOT})
include_directories(${LLVM_MSTL_SRC_ROOT})

foreach(header ${SOURCE_INCLUDES})
#    message(${header})
endforeach()

enable_testing()
include(CTest)

add_subdirectory(tests)

add_compile_options(${LLVM_MSTL_FLAGS})
add_executable(llvm-mstl main.cc)
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(llvm-mstl mstl -fsanitize=address)
else()
    target_link_libraries(llvm-mstl mstl spdlog)
endif()
