cmake_minimum_required(VERSION 3.10)
project(nativefiledialog C CXX)

set(SRC src/nfd_common.c)

if(WIN32)
    list(APPEND SRC src/nfd_win.cpp)
    set(LIBS comctl32)
elseif(APPLE)
    list(APPEND SRC src/nfd_cocoa.m)
    find_library(COCOA Cocoa)
    set(LIBS ${COCOA})
elseif(UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    list(APPEND SRC src/nfd_gtk.c)
    include_directories(${GTK3_INCLUDE_DIRS})
    set(LIBS ${GTK3_LIBRARIES})
endif()

add_library(nfd STATIC ${SRC})

set(OUT_PATH "${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME} - ${CMAKE_SYSTEM_PROCESSOR}/${CMAKE_BUILD_TYPE}")


set_target_properties(nfd
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${OUT_PATH}/nfd"
    LIBRARY_OUTPUT_DIRECTORY "${OUT_PATH}/nfd"
)

target_link_libraries(nfd PRIVATE ${LIBS})
target_include_directories(nfd PUBLIC src/include)
