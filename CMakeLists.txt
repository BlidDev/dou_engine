cmake_minimum_required(VERSION 3.19)
project(RayProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=gold")


include("cmake/settings.cmake")


add_subdirectory("vendor/spdlog")
add_subdirectory("vendor/glad")


set(WITH_FUNCTION_X OFF CACHE BOOL "Enable parse tools" FORCE)
set(WITH_FUNCTION_X OFF CACHE BOOL "Enable generation of yaml-cpp install targets" FORCE)
set(YAML_CPP_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_definitions(-DYAML_CPP_STATIC_DEFINE)
add_subdirectory("vendor/yaml-cpp")


set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Disable GLFW examples")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Disable GLFW tests")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Disable GLFW docs")
set(GLFW_STATIC TRUE CACHE BOOL "Set to build as static library")
add_subdirectory("vendor/glfw")

set(OUT_PATH "${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME} - ${CMAKE_SYSTEM_PROCESSOR}/${CMAKE_BUILD_TYPE}")

add_subdirectory(engine)


option(EXAMPLE_GAME "Build an example game" OFF)
if (EXAMPLE_GAME)
    add_subdirectory(game)
endif()

option(EXAMPLE_EDITOR "Build a simple editor" OFF)
if (EXAMPLE_EDITOR)
    add_subdirectory(radiator)
endif()



