
cmake_minimum_required(VERSION 3.16)

project(AdaptiveLearningSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(AdaptiveLearningSystem ${SOURCES})

target_include_directories(AdaptiveLearningSystem PRIVATE include)

