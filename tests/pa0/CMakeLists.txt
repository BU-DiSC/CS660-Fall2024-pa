get_filename_component(EXEC ${CMAKE_CURRENT_SOURCE_DIR} NAME)
file(GLOB_RECURSE CPP_TESTS "*_test.cpp")
add_executable(${EXEC} ${CPP_TESTS})
target_link_libraries(${EXEC} PRIVATE db GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${EXEC})
