cmake_minimum_required(VERSION 3.12)

project(cyberpunk-httprequest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../dist/)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../dist/)

include(FetchContent)
FetchContent_Declare(red4ext GIT_REPOSITORY https://github.com/WopsS/RED4ext.SDK.git
                         GIT_TAG af2a50d3f172c9d80e37a6ff5ad61d336273cf2a)
FetchContent_MakeAvailable(red4ext)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG c4713a704ca12237485ecbfec185f76c2a81bd09)
FetchContent_MakeAvailable(cpr)

add_subdirectory(src)
