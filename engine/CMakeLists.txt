# Define the static library target
file(GLOB engine_SRC
    "src/*.c"
    "src/*.cpp"
    "src/components/*.cpp"
    "src/systems/*.cpp"
    "src/lua/*.cpp"
    )

set_source_files_properties(vendor/src/glad.c PROPERTIES LANGUAGE CXX )

add_library(engine STATIC ${engine_SRC} ${engine_lib_src})


# Include directories f/or the engine
target_include_directories(engine PUBLIC
    ${CMAKE_SOURCE_DIR}/vendor/spdlog/include
    ${CMAKE_SOURCE_DIR}/vendor/entt
    ${CMAKE_SOURCE_DIR}/vendor/yaml-cpp/include
    ${CMAKE_SOURCE_DIR}/vendor/lua/include
    ${CMAKE_SOURCE_DIR}/vendor/sol2/include/
    ${CMAKE_SOURCE_DIR}/vendor/glad/include/
    ${CMAKE_SOURCE_DIR}/vendor/glm
    ${CMAKE_SOURCE_DIR}/vendor/stb_image
    ${CMAKE_SOURCE_DIR}/engine/include
    ${RAYLIB_DIR}/src
)


set(HEADER_FILES PRIVATE 
    "${CMAKE_SOURCE_DIR}/engine/include/espch.h"
)

target_precompile_headers(engine PRIVATE ${HEADER_FILES})

find_package(OpenGL REQUIRED)

link_directories(engine 
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libGLAD.a
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libGLFW.a
)

if(WIN32)
    set(LUA_PATH PRIVATE
            "${CMAKE_SOURCE_DIR}/vendor/lua/windows/lua54.lib")
elseif(UNIX AND NOT APPLE)
    set(LUA_PATH PRIVATE
        "${CMAKE_SOURCE_DIR}/vendor/lua/linux/liblua54.a")
endif()
target_link_libraries(engine PRIVATE glfw OpenGL::GL YAML_CPP GLAD ${LUA_PATH})
