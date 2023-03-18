cmake_minimum_required(VERSION 3.12)

# Change the project name here!
project(cyberpunk-httprequest LANGUAGES CXX)

# Set this option to ON to be able to use folders / filters in your project when using Visual Studio.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set the C++ standard to 20 (C++20).
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../dist/)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../dist/)

# Add RED4ext.SDK as a dependency.

# Uncomment the following line if you want to use the header-only version of RED4ext.SDK.
#option(RED4EXT_HEADER_ONLY "" ON)

add_subdirectory(deps/red4ext.sdk)
set_target_properties(RED4ext.SDK PROPERTIES FOLDER "Dependencies")

# This will mark the following RED4ext variables as advanced. They won't be displayed in CMake's GUI.
mark_as_advanced(
  RED4EXT_BUILD_EXAMPLES
  RED4EXT_HEADER_ONLY
)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 871ed52d350214a034f6ef8a3b8f51c5ce1bd400) # The commit hash for 1.9.0. Replace with the latest from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

# Include the main project.
add_subdirectory(src)
