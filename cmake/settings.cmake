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
  add_compile_options(/W4 /permissive-)
  set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Od")
  set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")


  foreach(flag_var
          CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
          CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
    string(REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
  endforeach()

else()
  set(CMAKE_CXX_FLAGS_DEBUG "-g")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
endif()

if(WIN32)
    add_compile_options("/utf-8")
endif()


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DU_DEBUG)
endif()


if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(DYNAMIC_SOURCE ${CMAKE_SOURCE_DIR})
else()
    set(DYNAMIC_SOURCE  ${CMAKE_CURRENT_SOURCE_DIR})
endif()


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
