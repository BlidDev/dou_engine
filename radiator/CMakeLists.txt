set(NAME radiator)
file(GLOB radiator_SRC
    "src/*.c"
    "src/*.cpp"
    )

file(GLOB imgui_SRC
    "${CMAKE_SOURCE_DIR}/vendor/imgui/*.cpp"
    "${CMAKE_SOURCE_DIR}/vendor/imgui/backends/*.cpp"
    "${CMAKE_SOURCE_DIR}/vendor/imgui/misc/cpp/*.cpp"
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
    ${CMAKE_SOURCE_DIR}/${NAME}/vendor/nativefiledialog/src/include
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

add_subdirectory("${CMAKE_SOURCE_DIR}/${NAME}/vendor/nativefiledialog")

if(WIN32)
    set(NFD_PATH
        "${OUT_PATH}/nfd/nfd.lib")
elseif(UNIX AND NOT APPLE)
    set(NFD_PATH
        "${OUT_PATH}/nfd/libnfd.a")
endif()



if(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    target_include_directories(${NAME} PRIVATE ${GTK3_INCLUDE_DIRS})
    target_link_libraries(${NAME} PRIVATE ${GTK3_LIBRARIES})
endif()



target_link_libraries(${NAME} PRIVATE OpenGL::GL engine "${NFD_PATH}")
