cmake_minimum_required(VERSION 3.19)
project(RayProject VERSION 1.0 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME} - ${CMAKE_SYSTEM_PROCESSOR}/${CMAKE_BUILD_TYPE}/engine")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME} - ${CMAKE_SYSTEM_PROCESSOR}/${CMAKE_BUILD_TYPE}/engine")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME} - ${CMAKE_SYSTEM_PROCESSOR}/${CMAKE_BUILD_TYPE}/game")


# Include raylib if needed
set(RAYLIB_DIR "${CMAKE_SOURCE_DIR}/raylib")

# Add subdirectories for the engine library and the game executable

add_subdirectory("vendor/yaml-cpp")
add_subdirectory(engine)
add_subdirectory(game)
