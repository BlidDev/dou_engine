set(NAME engine)

file(GLOB_RECURSE ${NAME}_SRC 
    "src/*.cpp" 
    "src/*.c"
    "include/*.h" 
    "include/*.hpp"
)

add_library(${NAME} STATIC ${${NAME}_SRC})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${${NAME}_SRC})


# We use PUBLIC so the 'game' target inherits these automatically
target_include_directories(${NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/entt
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/yaml-cpp/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/lua/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/sol2/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb_image
)

target_precompile_headers(${NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/espch.h")

find_package(OpenGL REQUIRED)

# Handle Lua platform specifics cleanly
if(WIN32)
    set(LUA_LIB "${CMAKE_CURRENT_SOURCE_DIR}/vendor/lua/windows/lua54.lib")
else()
    set(LUA_LIB "${CMAKE_CURRENT_SOURCE_DIR}/vendor/lua/linux/liblua54.a")
endif()

# Link against TARGETS, not file paths, whenever possible
target_link_libraries(${NAME} 
    PUBLIC 
        glfw 
        glad 
        yaml-cpp 
        OpenGL::GL
    PRIVATE 
        ${LUA_LIB}
)