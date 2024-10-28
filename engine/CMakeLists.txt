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
    ${CMAKE_SOURCE_DIR}/engine/include
    ${RAYLIB_DIR}/src
)


# Link raylib to the engine library
target_link_libraries(engine PRIVATE raylib)
