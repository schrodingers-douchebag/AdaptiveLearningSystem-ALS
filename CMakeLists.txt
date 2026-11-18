cmake_minimum_required(VERSION 3.20)
project(AdaptiveLearningSystem VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug)
endif()

option(ALS_BUILD_TESTS "Build unit and integration tests" ON)

# ---------- system dependencies (install with apt / brew / vcpkg) ----------
find_package(SQLite3 REQUIRED)
find_package(Threads REQUIRED)

find_package(unofficial-sodium CONFIG QUIET)          # vcpkg
if(unofficial-sodium_FOUND)
  set(SODIUM_TARGET unofficial-sodium::sodium)
else()                                                 # apt / brew
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SODIUM REQUIRED IMPORTED_TARGET libsodium)
  set(SODIUM_TARGET PkgConfig::SODIUM)
endif()

# ---------- header-only / small dependencies (downloaded automatically) ----------
include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(json GIT_REPOSITORY https://github.com/nlohmann/json.git GIT_TAG v3.11.3 GIT_SHALLOW TRUE)
FetchContent_Declare(spdlog GIT_REPOSITORY https://github.com/gabime/spdlog.git GIT_TAG v1.14.1 GIT_SHALLOW TRUE)
FetchContent_Declare(asio GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git GIT_TAG asio-1-30-2 GIT_SHALLOW TRUE)
FetchContent_Declare(crow GIT_REPOSITORY https://github.com/CrowCpp/Crow.git GIT_TAG v1.3.5 GIT_SHALLOW TRUE)

set(JSON_BuildTests OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(json spdlog)

# Crow + standalone Asio are header-only: we only need their include folders.
FetchContent_GetProperties(asio)
if(NOT asio_POPULATED)
  FetchContent_Populate(asio)
endif()
FetchContent_GetProperties(crow)
if(NOT crow_POPULATED)
  FetchContent_Populate(crow)
endif()
add_library(crow_headers INTERFACE)
target_include_directories(crow_headers SYSTEM INTERFACE ${crow_SOURCE_DIR}/include ${asio_SOURCE_DIR}/asio/include)
target_compile_definitions(crow_headers INTERFACE ASIO_STANDALONE ASIO_NO_DEPRECATED)
if(WIN32)
  target_compile_definitions(crow_headers INTERFACE _WIN32_WINNT=0x0A00)
  target_link_libraries(crow_headers INTERFACE ws2_32 mswsock)
endif()

# ---------- core library (everything except main.cpp, so tests can reuse it) ----------
file(GLOB_RECURSE ALS_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)
list(FILTER ALS_SOURCES EXCLUDE REGEX ".*/src/main\\.cpp$")

add_library(als_core STATIC ${ALS_SOURCES})
target_include_directories(als_core PUBLIC include config)
target_link_libraries(als_core PUBLIC
  SQLite::SQLite3 Threads::Threads ${SODIUM_TARGET}
  nlohmann_json::nlohmann_json spdlog::spdlog crow_headers)

if(MSVC)
  target_compile_options(als_core PRIVATE /W4)
else()
  target_compile_options(als_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(AdaptiveLearningSystem src/main.cpp)
target_link_libraries(AdaptiveLearningSystem PRIVATE als_core)

if(ALS_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
