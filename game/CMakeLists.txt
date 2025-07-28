file(GLOB game_SRC
    "src/*.c"
    "src/*.cpp"
    )

add_executable(game ${game_SRC})



target_include_directories(game PRIVATE
    ${CMAKE_SOURCE_DIR}/engine/include
    ${CMAKE_SOURCE_DIR}/game/include
    ${CMAKE_SOURCE_DIR}/vendor/spdlog/include
    ${CMAKE_SOURCE_DIR}/vendor/entt
    ${CMAKE_SOURCE_DIR}/vendor/glfw/include
    ${YAML_CPP_INCLUDE_DIRS}
    ${RAYLIB_DIR}/src
    ${CMAKE_SOURCE_DIR}/vendor/glad/include/
    ${CMAKE_SOURCE_DIR}/vendor/glm
)


set(HEADER_FILES PRIVATE 
    "${CMAKE_SOURCE_DIR}/engine/include/engine.h")

find_package(OpenGL REQUIRED)

target_precompile_headers(game PRIVATE ${HEADER_FILES})

target_link_libraries(game PRIVATE YAML_CPP glfw  OpenGL::GL engine)
