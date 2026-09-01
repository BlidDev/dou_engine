-- Make Dou generate a standalone workspace by default 
if DOU_STANDALONE == nil then DOU_STANDALONE = true end

newoption {
    trigger     = "build-game",
    description = "Builds the internal sandbox game project"
}

newoption {
    trigger     = "build-btest",
    description = "DEBUG ONLY, WILL BE REMOVED. Builds the batching test project"
}

local DOU_INCLUDES = {
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


local API = {};

function M.get_dou_includes(module_path)
    local tmp = {}

    for _, path in ipairs(DOU_INCLUDES) do
        table.insert(tmp, module_path .."/" .. path)
    end

    return tmp
end



local DOU_ROOT = _MAIN_SCRIPT_DIR

function link_dou_engine(dou_root)
    links { "engine", "Glad", "YAML_CPP", "GLFW" }

    local root = dou_root or DOU_ROOT

    local lua_win_src = path.getabsolute(root .. "/vendor/lua/windows/lua54.lib")
    local lua_linux_src = path.getabsolute(root .. "/vendor/lua/linux/liblua54.so")

    filter "system:windows"
        libdirs { root .. "/vendor/lua/windows" }
        links { "lua54", "opengl32", "gdi32", "user32", "shell32" }
        postbuildcommands { "{COPY} \"" .. lua_win_src .. "\" \"%{cfg.targetdir}/\"" }

    filter "system:linux"
        libdirs { root .."/vendor/lua/linux" }
        links { "lua54", "GL", "X11", "pthread", "dl", "m", "Xrandr", "Xi", "Xcursor", "Xinerama" }
        postbuildcommands { "{COPY} \"" .. lua_linux_src .. "\" \"%{cfg.targetdir}/\"" }
        linkoptions { "-Wl,-rpath,'$$ORIGIN'" }
        
  --filter "system:macosx"
  --    libdirs { root .. "/vendor/lua/macos" } 
  --    links { "lua54", "Cocoa.framework", "IOKit.framework", "CoreVideo.framework", "CoreFoundation.framework", "OpenGL.framework" }

    filter "toolset:gcc or toolset:clang"
        linkoptions { "-fuse-ld=gold" }

    filter {} 
end


if DOU_STANDALONE then
    workspace "DouEngine"
        architecture "x86_64"
        configurations { "Debug", "Release" }

        location ("build")
        pic "On" 

        -- COMPILER & BUILD SETTINGS 
        debugdir "%{DOU_ROOT}"
        targetdir ("bin/%{cfg.buildcfg}")
        objdir ("build/bin-int/%{cfg.buildcfg}/%{prj.name}")

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
end

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
    objdir ("build/bin-int/%{cfg.buildcfg}/%{prj.name}")

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
if _OPTIONS["build-game"] then
    project "game"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++20"
        targetdir ("bin/%{cfg.buildcfg}")
        objdir ("build/bin-int/%{cfg.buildcfg}/%{prj.name}")

        files {
            "game/src/**.cpp", "game/src/**.c",
            "game/include/**.h", "game/include/**.hpp"
        }

        includedirs (DOU_INCLUDES)
        includedirs { "game/include" }

        link_dou_engine()

        pchheader "engine.h"
end


-- ==============================================================================
if _OPTIONS["build-btest"] then
    project "btest"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir ("bin/%{cfg.buildcfg}")
    objdir ("build/bin-int/%{cfg.buildcfg}/%{prj.name}")

    files {
        "btest/src/**.cpp", "btest/src/**.c",
        "btest/include/**.h", "btest/include/**.hpp"
    }

    includedirs (DOU_INCLUDES)
    includedirs { "btest/include" }

    link_dou_engine()

    pchheader "engine.h"
end



-- = COMPILE COMMANDS PREMAKE EXTENTION =========================================
require "utils/ecc/ecc"

return API
