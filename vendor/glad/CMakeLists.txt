cmake_minimum_required(VERSION 3.4...3.14)
project(GLAD VERSION 0.8.0 LANGUAGES C)


file(GLOB GLAD_SRC
    "src/*.c"
    )

add_library(GLAD STATIC ${GLAD_SRC})

add_compile_definitions(GLAD_STATIC_DEFINE)

target_include_directories(GLAD PUBLIC
    "include"
)

