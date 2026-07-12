newoption {
    trigger     = "build-game",
    description = "Builds the internal sandbox game project"
}

DOU_INCLUDES = {
    "engine/include",
    "vendor/spdlog/include",
    "vendor/entt",
    "vendor/glm",
    "vendor/glfw/include",
    "vendor/sol2/include",
    "vendor/lua/include",
    "vendor/glad/include",
    "vendor/yaml-cpp/include"
}

function link_dou_engine()
    links { "engine", "Glad", "YAML_CPP", "GLFW" }

    filter "system:windows"
        libdirs { "vendor/lua/windows" }
        links { "lua54", "opengl32", "gdi32", "user32", "shell32" }
        postbuildcommands { "{COPY} \"%{wks.location}/vendor/lua/windows/liblua54.lib\" \"%{cfg.targetdir}/\"" }

    filter "system:linux"
        libdirs { "vendor/lua/linux" }
        links { "lua54", "GL", "X11", "pthread", "dl", "m", "Xrandr", "Xi", "Xcursor", "Xinerama" }
        postbuildcommands { "{COPY} \"%{wks.location}/vendor/lua/linux/liblua54.so\" \"%{cfg.targetdir}/\"" }
        linkoptions { "-Wl,-rpath,'$$ORIGIN'" }
        
    filter "system:macosx"
        libdirs { "vendor/lua/macos" } 
        links { "lua54", "Cocoa.framework", "IOKit.framework", "CoreVideo.framework", "CoreFoundation.framework", "OpenGL.framework" }

    filter "toolset:gcc or toolset:clang"
        linkoptions { "-fuse-ld=gold" }

    filter {} 
end

workspace "DouEngine"
    architecture "x86_64"
    configurations { "Debug", "Release" }
    
    pic "On" 

    -- COMPILER & BUILD SETTINGS 
    debugdir "%{wks.location}"
    targetdir ("bin/%{cfg.buildcfg}")
    objdir ("bin-int/%{cfg.buildcfg}/%{prj.name}")

    filter "configurations:Debug"
        defines { "DU_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "Speed"

    filter "system:windows"
        defines { "SPDLOG_WCHAR_TO_UTF8_SUPPORT" }

    filter "toolset:msc"
        staticruntime "On" 
        buildoptions { "/permissive-", "/bigobj", "/utf-8" }

    filter "toolset:gcc or toolset:clang"
        buildoptions { "-finput-charset=UTF-8", "-fexec-charset=UTF-8" }

    filter "toolset:gcc"
        buildoptions { "-fdiagnostics-color=always" }
    filter "toolset:clang"
        buildoptions { "-fcolor-diagnostics" }

    filter {} 

-- ==============================================================================
project "Glad"
    kind "StaticLib"
    language "C"
    targetdir ("vendor/bin/%{cfg.buildcfg}/Glad")
    objdir ("vendor/bin-int/%{cfg.buildcfg}/Glad")
    files { "vendor/glad/src/glad.c" }
    includedirs { "vendor/glad/include" }
    filter "toolset:msc"
        removebuildoptions { "/permissive-" }
    filter {}

project "YAML_CPP"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir ("vendor/bin/%{cfg.buildcfg}/YAML_CPP")
    objdir ("vendor/bin-int/%{cfg.buildcfg}/YAML_CPP")
    defines { "YAML_CPP_STATIC_DEFINE" }
    files { "vendor/yaml-cpp/src/**.cpp" }
    includedirs { "vendor/yaml-cpp/include", "vendor/yaml-cpp/src" }

project "GLFW"
    kind "StaticLib"
    language "C"
    targetdir ("vendor/bin/%{cfg.buildcfg}/GLFW")
    objdir ("vendor/bin-int/%{cfg.buildcfg}/GLFW")
    includedirs { "vendor/glfw/include" }
    
    files {
        "vendor/glfw/src/context.c", 
        "vendor/glfw/src/init.c",
        "vendor/glfw/src/input.c", 
        "vendor/glfw/src/monitor.c",
        "vendor/glfw/src/vulkan.c", 
        "vendor/glfw/src/window.c",
        "vendor/glfw/src/platform.c",
        "vendor/glfw/src/null_init.c",
        "vendor/glfw/src/null_monitor.c",
        "vendor/glfw/src/null_window.c",
        "vendor/glfw/src/null_joystick.c"
    }

    filter "system:windows"
        defines { "_GLFW_WIN32" }
        files { 
            "vendor/glfw/src/win32_*.c", 
            "vendor/glfw/src/wgl_*.c", 
            "vendor/glfw/src/egl_*.c", 
            "vendor/glfw/src/osmesa_*.c" 
        }

    filter "system:linux"
        defines { "_GLFW_X11" }
        files { 
            "vendor/glfw/src/x11_*.c", 
            "vendor/glfw/src/glx_*.c", 
            "vendor/glfw/src/posix_*.c", 
            "vendor/glfw/src/linux_*.c", 
            "vendor/glfw/src/xkb_*.c", 
            "vendor/glfw/src/egl_*.c", 
            "vendor/glfw/src/osmesa_*.c" 
        }
        
    filter "system:macosx"
        defines { "_GLFW_COCOA" }
        files {
            "vendor/glfw/src/cocoa_*.m",
            "vendor/glfw/src/nsgl_*.m",
            "vendor/glfw/src/posix_*.c",
            "vendor/glfw/src/egl_*.c",
            "vendor/glfw/src/osmesa_*.c"
        }

    filter "toolset:msc"
        removebuildoptions { "/permissive-" }
        
    filter {}

-- ==============================================================================
project "engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir ("bin/%{cfg.buildcfg}")
    objdir ("bin-int/%{cfg.buildcfg}/%{prj.name}")

    files {
        "engine/src/**.cpp", "engine/src/**.c",
        "engine/include/**.h", "engine/include/**.hpp"
    }

    includedirs (DOU_INCLUDES)
    includedirs {
        "vendor/lua/include",
        "vendor/sol2/include",
        "vendor/glad/include",
        "vendor/stb_image"
    }

    pchheader "espch.h"
    defines { "YAML_CPP_STATIC_DEFINE" }

    links { "Glad", "YAML_CPP", "GLFW" }

-- ==============================================================================
-- GAME PROJECT (Clean Client Implementation)
-- ==============================================================================
if _OPTIONS["build-game"] then
    project "game"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++20"
        targetdir ("bin/%{cfg.buildcfg}")
        objdir ("bin-int/%{cfg.buildcfg}/%{prj.name}")

        files {
            "game/src/**.cpp", "game/src/**.c",
            "game/include/**.h", "game/include/**.hpp"
        }

        includedirs (DOU_INCLUDES)
        includedirs { "game/include" }

        link_dou_engine()

        pchheader "engine.h"
end
