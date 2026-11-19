FetchContent_Declare(googletest GIT_REPOSITORY https://github.com/google/googletest.git GIT_TAG v1.14.0 GIT_SHALLOW TRUE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS unit/*.cpp integration/*.cpp)
add_executable(als_tests ${TEST_SOURCES})
target_link_libraries(als_tests PRIVATE als_core GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(als_tests)
