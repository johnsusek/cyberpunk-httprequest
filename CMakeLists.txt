cmake_minimum_required(VERSION 3.15...3.26)

project(cyberpunk-httprequest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../dist/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../dist/)

set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/deps)

find_package(Poco REQUIRED Foundation JSON)
find_package(RED4ext REQUIRED)

include(FetchContent)

add_subdirectory(src)
