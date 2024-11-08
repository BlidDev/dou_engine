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
    ${CMAKE_SOURCE_DIR}/vendor/entt
    ${CMAKE_SOURCE_DIR}/vendor/yaml-cpp/include
    ${RAYLIB_DIR}/src
)

link_directories(game PRIVATE
    ${CMAKE_SOURCE_DIR}/vendor/yaml-cpp/build/libyaml-cpp.a
)




# Link the engine library to the game executable
target_link_libraries(game PRIVATE engine raylib yaml-cpp)
