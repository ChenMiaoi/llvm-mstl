#set(MODULE_PATH "" CACHE INTERNAL "Path to the module")
#set(MODULE_RIGHT_PATH "")

function(find_module_path MODULE_NAME CURRENT_PATH)
  file (GLOB SUB_MODULES RELATIVE ${CURRENT_PATH} ${CURRENT_PATH}/*)
  foreach (SUB_MODULE ${SUB_MODULES})
    if (IS_DIRECTORY ${CURRENT_PATH}/${SUB_MODULE})
      if (${SUB_MODULE} STREQUAL ${MODULE_NAME})
        set(${MODULE_NAME}_PATH ${CURRENT_PATH}/${SUB_MODULE} CACHE PATH "Path to the ${MODULE_NAME}")
        # set(${MODULE_PATH} ${CURRENT_PATH}/${SUB_MODULE})
        # set(${MODULE_RIGHT_PATH} ${MODULE_PATH} PARENT_SCOPE)
        return()
      else ()
        find_module_path(${MODULE_NAME} ${CURRENT_PATH}/${SUB_MODULE})
        if (DEFINED ${MODULE_NAME}_PATH)
          message(STATUS "Find Module Path ${${MODULE_NAME}_PATH}")
          return()
        endif ()
      endif ()
    endif ()
  endforeach ()
endfunction()


function(add_test_module MODULE_NAME)
  find_module_path(${MODULE_NAME} "${CMAKE_CURRENT_SOURCE_DIR}")

  if (NOT DEFINED ${MODULE_NAME}_PATH)
    message(FATAL_ERROR "Module '${MODULE_NAME}' not found")
  else ()
    message(STATUS "Find Module Test '${MODULE_NAME}'")
  endif()
  
  file (GLOB TEST_MODULE_SRCS ${${MODULE_NAME}_PATH}/*.cc)
  add_compile_options(${LLVM_MSTL_FLAGS})
  add_executable(${MODULE_NAME} ${TEST_MODULE_SRCS})
  if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_link_libraries(${MODULE_NAME} mstl gtest GTest::gtest_main spdlog -fsanitize=address)
  else()
  target_link_libraries(${MODULE_NAME} mstl gtest GTest::gtest_main spdlog)
  endif()
  add_test(
    NAME ${MODULE_NAME}
    COMMAND $<TARGET_FILE:${MODULE_NAME}>
    WORKING_DIRECTORY ${CTEST_BINARY_DIRECTORY}
  )
  set_property(TEST ${MODULE_NAME} PROPERTY
    ENVIRONMENT "CTEST_BUILD_CONFIGURATION=$<CONFIG>"
  )
  set_tests_properties(${MODULE_NAME} PROPERTIES
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
  )
endfunction()

find_program(MEMORYCHECK_COMMAND valgrind)
set(MEMORYCHECK_COMMAND_OPTIONS "--leak-check=full --show-leak-kinds=all")

file(GLOB TEST_MODULES "*.cc")

foreach(TEST_MODULE ${TEST_MODULES})
  get_filename_component(TEST_MODULE_NAME ${TEST_MODULE} NAME_WE)
  add_compile_options(${LLVM_MSTL_FLAGS})
  add_executable(${TEST_MODULE_NAME} ${TEST_MODULE})
  if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_link_libraries(${TEST_MODULE_NAME} mstl gtest GTest::gtest_main spdlog -fsanitize=address)
  else()
  target_link_libraries(${TEST_MODULE_NAME} mstl gtest GTest::gtest_main spdlog)
  endif()
  add_test(
    NAME ${TEST_MODULE_NAME}
    COMMAND $<TARGET_FILE:${TEST_MODULE_NAME}>
    WORKING_DIRECTORY ${CTEST_BINARY_DIRECTORY}
  )
  set_property(TEST ${TEST_MODULE_NAME} PROPERTY
    ENVIRONMENT "CTEST_BUILD_CONFIGURATION=$<CONFIG>"
  )
  set_tests_properties(${TEST_MODULE_NAME} PROPERTIES
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
  )
endforeach()

add_test_module(__split_buffer)
add_test_module(vector)
