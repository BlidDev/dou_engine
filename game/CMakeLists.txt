set(NAME game)

file(GLOB ${NAME}_SRC
    "src/*.c"
    "src/*.cpp"
    )

add_executable(${NAME} ${${NAME}_SRC})

set_target_properties(${NAME} 
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${OUT_PATH}/${NAME}"
)


target_include_directories(${NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/engine/include
    ${CMAKE_SOURCE_DIR}/${NAME}/include
    ${CMAKE_SOURCE_DIR}/vendor/spdlog/include
    ${CMAKE_SOURCE_DIR}/vendor/entt
    ${CMAKE_SOURCE_DIR}/vendor/glfw/include
    ${YAML_CPP_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/vendor/glad/include/
    ${CMAKE_SOURCE_DIR}/vendor/glm
)


set(HEADER_FILES PRIVATE 
    "${CMAKE_SOURCE_DIR}/engine/include/engine.h")

find_package(OpenGL REQUIRED)

target_precompile_headers(${NAME} PRIVATE ${HEADER_FILES})

target_link_libraries(${NAME} PRIVATE YAML_CPP glfw  OpenGL::GL engine)
