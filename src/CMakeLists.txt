add_library(HttpRequest SHARED "")

set(PROJECT_SRC_FILES
  Main.cpp
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${PROJECT_SRC_FILES})

target_include_directories(HttpRequest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(HttpRequest PRIVATE ${PROJECT_SRC_FILES})

target_link_libraries(HttpRequest PRIVATE RED4ext::SDK)
target_link_libraries(HttpRequest PRIVATE Poco::Foundation Poco::JSON)
