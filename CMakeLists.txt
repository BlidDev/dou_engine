cmake_minimum_required(VERSION 3.19)
project(DouEngine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Only use the gold linker on Linux/GCC/Clang
if(NOT MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
endif()

# Global Output Directories (Prevents multi-config nesting issues)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

include("cmake/settings.cmake")

# Vendor targets
add_subdirectory("vendor/spdlog")
add_subdirectory("vendor/glad")

set(YAML_CPP_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_definitions(-DYAML_CPP_STATIC_DEFINE)
add_subdirectory("vendor/yaml-cpp")

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_STATIC ON CACHE BOOL "" FORCE)
add_subdirectory("vendor/glfw")

add_subdirectory(engine)

option(EXAMPLE_GAME "Build an example game" OFF)
if (EXAMPLE_GAME)
    add_subdirectory(game)
endif()