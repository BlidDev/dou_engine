find_program(CCACHE_PROGRAM ccache)
find_program(CCACHE "ccache")
if(CCACHE)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE}")
endif()

cmake_minimum_required(VERSION 3.19)
project(RayProject VERSION 1.0 LANGUAGES CXX)

find_program(CCACHE "ccache")
if(CCACHE)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE}")
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if(MSVC)
  add_compile_options(/W4 /permissive-)
  set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Od")
  set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
else()
    #add_compile_options(-Wall -Wextra -Wno-unused-parameter)
  set(CMAKE_CXX_FLAGS_DEBUG "-g")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME} - ${CMAKE_SYSTEM_PROCESSOR}/${CMAKE_BUILD_TYPE}/engine")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME} - ${CMAKE_SYSTEM_PROCESSOR}/${CMAKE_BUILD_TYPE}/engine")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME} - ${CMAKE_SYSTEM_PROCESSOR}/${CMAKE_BUILD_TYPE}/game")



set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=gold")

message(${CMAKE_EXE_LINKER_FLAGS})

if(WIN32)
    add_compile_options("/utf-8")
endif()

# Match the static or dynamic runtime across all targets (MSVC only)
if (MSVC)
  foreach(flag_var
          CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
          CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
    string(REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
  endforeach()
endif()

add_subdirectory("vendor/spdlog")

set(WITH_FUNCTION_X OFF CACHE BOOL "Enable parse tools" FORCE)
set(WITH_FUNCTION_X OFF CACHE BOOL "Enable generation of yaml-cpp install targets" FORCE)
set(YAML_CPP_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_definitions(-DYAML_CPP_STATIC_DEFINE)
add_subdirectory("vendor/yaml-cpp")
add_subdirectory("vendor/glad")


set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Disable GLFW examples")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Disable GLFW tests")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Disable GLFW docs")
set(GLFW_STATIC TRUE CACHE BOOL "Set to build as static library")
add_subdirectory("vendor/glfw")

add_subdirectory(engine)
add_subdirectory(game)
