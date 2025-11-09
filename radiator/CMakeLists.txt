set(NAME radiator)
file(GLOB radiator_SRC
    "src/*.c"
    "src/*.cpp"
    )

file(GLOB imgui_SRC
    "${CMAKE_SOURCE_DIR}/vendor/imgui/*.cpp"
    "${CMAKE_SOURCE_DIR}/vendor/imgui/backends/*.cpp"
    "${CMAKE_SOURCE_DIR}/vendor/imgui/misc/cpp/*.cpp"
    "${CMAKE_SOURCE_DIR}/${NAME}/vendor/tinyfiledialogs/tinyfiledialogs.cpp"
)

add_executable(${NAME} ${${NAME}_SRC} ${imgui_SRC})

set_target_properties(${NAME} 
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${OUT_PATH}/${NAME}"
)


target_include_directories(${NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/engine/include
    ${CMAKE_SOURCE_DIR}/${NAME}/include
    ${CMAKE_SOURCE_DIR}/vendor/glfw/include
    ${CMAKE_SOURCE_DIR}/vendor/glad/include/
    ${CMAKE_SOURCE_DIR}/vendor/glm
    ${CMAKE_SOURCE_DIR}/vendor/imgui
    ${CMAKE_SOURCE_DIR}/vendor/imgui/backends
    "${CMAKE_SOURCE_DIR}/${NAME}/vendor/tinyfiledialogs"
)


set(HEADER_FILES PRIVATE 
    "${CMAKE_SOURCE_DIR}/engine/include/engine.h")

find_package(OpenGL REQUIRED)

target_precompile_headers(${NAME} PRIVATE ${HEADER_FILES})

if(UNIX AND NOT APPLE)
    target_link_libraries(${NAME} PRIVATE
        X11
        Xi
        Xcursor
        Xrandr
        Xinerama
    )
endif()


target_link_libraries(${NAME} PRIVATE OpenGL::GL engine)
