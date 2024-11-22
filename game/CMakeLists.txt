# Define the executable target
file(GLOB game_SRC
    "src/*.c"
    "src/*.cpp"
    )
add_executable(game ${game_SRC})


# Include directories for the game
target_include_directories(game PRIVATE
    ${CMAKE_SOURCE_DIR}/engine/include
    ${CMAKE_SOURCE_DIR}/game/include
    ${CMAKE_SOURCE_DIR}/vendor/spdlog/include
    ${CMAKE_SOURCE_DIR}/vendor/entt
    ${YAML_CPP_INCLUDE_DIRS}
    ${RAYLIB_DIR}/src
)


link_directories(game ${YAML_CPP_LIBRARIES})

set(HEADER_FILES PRIVATE 
    "${CMAKE_SOURCE_DIR}/engine/include/epch.h")

target_precompile_headers(game PRIVATE ${HEADER_FILES})

# Link the engine library to the game executable
target_link_libraries(game PRIVATE engine raylib yaml-cpp)
