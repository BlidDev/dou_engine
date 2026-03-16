option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." FALSE)
if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
       add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if(MSVC)
  add_compile_options(/permissive- /bigobj)
  set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Od")
  set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")


  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  set(CMAKE_VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

  add_compile_options("/utf-8")
else()
  set(CMAKE_CXX_FLAGS_DEBUG "-g")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
  add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

if (WIN32) 
    add_compile_definitions(SPDLOG_WCHAR_TO_UTF8_SUPPORT)
endif()


add_compile_definitions($<$<CONFIG:Debug>:DU_DEBUG>)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(DYNAMIC_SOURCE ${CMAKE_SOURCE_DIR})
else()
    set(DYNAMIC_SOURCE  ${CMAKE_CURRENT_SOURCE_DIR})
endif()


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
