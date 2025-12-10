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

message(${CMAKE_SOURCE_DIR} " -- " ${CMAKE_CURRENT_SOURCE_DIR} -- ${DYNAMIC_SOURCE})
target_include_directories(${NAME} PUBLIC
    ${DYNAMIC_SOURCE}/vendor/spdlog/include
    ${DYNAMIC_SOURCE}/vendor/entt
    ${DYNAMIC_SOURCE}/vendor/yaml-cpp/include
    ${DYNAMIC_SOURCE}/vendor/lua/include
    ${DYNAMIC_SOURCE}/vendor/sol2/include/
    ${DYNAMIC_SOURCE}/vendor/glad/include/
    ${DYNAMIC_SOURCE}/vendor/glm
    ${DYNAMIC_SOURCE}/vendor/stb_image
    ${DYNAMIC_SOURCE}/${NAME}/include
)


set(HEADER_FILES PRIVATE 
    "${DYNAMIC_SOURCE}/${NAME}/include/espch.h"
)

target_precompile_headers(${NAME} PRIVATE ${HEADER_FILES})

find_package(OpenGL REQUIRED)

link_directories(${NAME} 
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libGLAD.a
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libGLFW.a
)

if(WIN32)
    set(LUA_PATH PRIVATE
            "${DYNAMIC_SOURCE}/vendor/lua/windows/lua54.lib")
elseif(UNIX AND NOT APPLE)
    set(LUA_PATH PRIVATE
        "${DYNAMIC_SOURCE}/vendor/lua/linux/liblua54.a")
endif()
target_link_libraries(${NAME} PRIVATE glfw OpenGL::GL YAML_CPP GLAD ${LUA_PATH})
