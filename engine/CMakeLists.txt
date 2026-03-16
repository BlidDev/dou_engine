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
    "${DYNAMIC_SOURCE}/${NAME}/include"
    "${DYNAMIC_SOURCE}/vendor/spdlog/include"
    "${DYNAMIC_SOURCE}/vendor/entt"
    "${DYNAMIC_SOURCE}/vendor/yaml-cpp/include"
    "${DYNAMIC_SOURCE}/vendor/lua/include"
    "${DYNAMIC_SOURCE}/vendor/sol2/include"
    "${DYNAMIC_SOURCE}/vendor/glad/include"
    "${DYNAMIC_SOURCE}/vendor/glm"
    "${DYNAMIC_SOURCE}/vendor/stb_image"
)

target_precompile_headers(${NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/espch.h")

find_package(OpenGL REQUIRED)

# Handle Lua platform specifics cleanly
if(WIN32)
    set(LUA_LIB "${DYNAMIC_SOURCE}/vendor/lua/windows/lua54.lib")
else()
    set(LUA_LIB "${DYNAMIC_SOURCE}/vendor/lua/linux/liblua54.a")
endif()

# Link against TARGETS, not file paths, whenever possible
target_link_libraries(${NAME} 
    PUBLIC 
        glfw 
        GLAD 
        YAML_CPP 
        OpenGL::GL
    PRIVATE 
        ${LUA_LIB}
)
