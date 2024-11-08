# Define the static library target
file(GLOB engine_SRC
    "src/*.c"
    "src/*.cpp"
    "src/components/*.cpp"
    )
add_library(engine STATIC ${engine_SRC})


# Include directories for the engine
target_include_directories(engine PUBLIC
    ${CMAKE_SOURCE_DIR}/vendor/entt
    ${CMAKE_SOURCE_DIR}/vendor/yaml-cpp/include
    ${CMAKE_SOURCE_DIR}/engine/include
    ${RAYLIB_DIR}/src
)

link_directories(engine PRIVATE
    ${CMAKE_SOURCE_DIR}/vendor/yaml-cpp/build/libyaml-cpp.a
)

# Link raylib to the engine library
target_link_libraries(engine PRIVATE raylib yaml-cpp)
