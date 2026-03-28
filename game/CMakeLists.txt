set(NAME game)

file(GLOB_RECURSE ${NAME}_SRC 
    "src/*.cpp" 
    "src/*.c"
    "include/*.h" 
    "include/*.hpp"
)
add_executable(${NAME} ${${NAME}_SRC})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${${NAME}_SRC})


target_link_libraries(${NAME} PRIVATE engine)

target_include_directories(${NAME} PRIVATE "${CMAKE_SOURCE_DIR}/${NAME}/include")

target_precompile_headers(${NAME} PRIVATE "${CMAKE_SOURCE_DIR}/engine/include/engine.h")