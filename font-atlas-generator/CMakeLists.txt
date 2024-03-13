cmake_minimum_required(VERSION 3.27)
project(font-atlas-generator)

# Set C++ standard and optimization level
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG -Wall -Wpedantic")

# Include directories
include_directories(
    src
    /usr/include/freetype2
)

# Add executable
file(GLOB SOURCES src/*.cpp )
add_executable(${PROJECT_NAME}
    ${SOURCES}
)

# Set build path
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    png
    freetype
    fontatlas
)
