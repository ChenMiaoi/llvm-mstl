file(GLOB TEST_MODULES "*.cc")

foreach(TEST_MODULE ${TEST_MODULES})
  get_filename_component(TEST_MODULE_NAME ${TEST_MODULE} NAME_WE)
  add_executable(${TEST_MODULE_NAME} ${TEST_MODULE})
  target_link_libraries(${TEST_MODULE_NAME} mstl gtest GTest::gtest_main)
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
