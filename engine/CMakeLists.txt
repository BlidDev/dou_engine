# Define the static library target
file(GLOB engine_SRC
    "src/*.c"
    "src/*.cpp"
    "src/components/*.cpp"
    )
add_library(engine STATIC ${engine_SRC})


# Include directories for the engine
target_include_directories(engine PUBLIC
    ${CMAKE_SOURCE_DIR}/vendor/spdlog/include
    ${CMAKE_SOURCE_DIR}/vendor/entt
    ${CMAKE_SOURCE_DIR}/vendor/yaml-cpp/include
    ${CMAKE_SOURCE_DIR}/vendor/lua/include
    ${CMAKE_SOURCE_DIR}/vendor/sol2/single/include/sol
    ${CMAKE_SOURCE_DIR}/engine/include
    ${RAYLIB_DIR}/src
)


set(HEADER_FILES PRIVATE 
    "${CMAKE_SOURCE_DIR}/vendor/sol2/single/include/sol/sol.hpp")

target_precompile_headers(engine PRIVATE ${HEADER_FILES})

link_directories(game ${YAML_CPP_LIBRARIES}
    ${CMAKE_SOURCE_DIR}/vendor/lua/linux/liblua54.a)

# Link raylib to the engine library
target_link_libraries(engine PRIVATE raylib yaml-cpp lua)
