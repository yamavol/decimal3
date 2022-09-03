
add_library(decimal3 INTERFACE)

target_sources(decimal3
INTERFACE
    decimal3.h
)

target_include_directories(decimal3 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})