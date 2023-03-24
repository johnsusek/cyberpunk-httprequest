add_library(cyberpunk-httprequest SHARED "")

set(PROJECT_SRC_FILES
  Main.cpp
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${PROJECT_SRC_FILES})

target_include_directories(cyberpunk-httprequest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(cyberpunk-httprequest PRIVATE ${PROJECT_SRC_FILES})

target_link_libraries(cyberpunk-httprequest PRIVATE RED4ext::SDK)
target_link_libraries(cyberpunk-httprequest PRIVATE Poco::Foundation Poco::JSON)
