set(NAME engine)

file(GLOB ${NAME}_SRC
    "src/*.c"
    "src/*.cpp"
    "src/components/*.cpp"
    "src/systems/*.cpp"
    "src/lua/*.cpp"
    )

set_source_files_properties(vendor/src/glad.c PROPERTIES LANGUAGE CXX )

add_library(${NAME} STATIC ${${NAME}_SRC} ${${NAME}_lib_src})

set_target_properties(${NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${OUT_PATH}/${NAME}"
    LIBRARY_OUTPUT_DIRECTORY "${OUT_PATH}/${NAME}"
)

# Include directories f/or the ${NAME}
target_include_directories(${NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/vendor/spdlog/include
    ${CMAKE_SOURCE_DIR}/vendor/entt
    ${CMAKE_SOURCE_DIR}/vendor/yaml-cpp/include
    ${CMAKE_SOURCE_DIR}/vendor/lua/include
    ${CMAKE_SOURCE_DIR}/vendor/sol2/include/
    ${CMAKE_SOURCE_DIR}/vendor/glad/include/
    ${CMAKE_SOURCE_DIR}/vendor/glm
    ${CMAKE_SOURCE_DIR}/vendor/stb_image
    ${CMAKE_SOURCE_DIR}/${NAME}/include
    ${RAYLIB_DIR}/src
)


set(HEADER_FILES PRIVATE 
    "${CMAKE_SOURCE_DIR}/${NAME}/include/espch.h"
)

target_precompile_headers(${NAME} PRIVATE ${HEADER_FILES})

find_package(OpenGL REQUIRED)

link_directories(${NAME} 
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
target_link_libraries(${NAME} PRIVATE glfw OpenGL::GL YAML_CPP GLAD ${LUA_PATH})
