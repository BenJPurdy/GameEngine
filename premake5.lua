workspace "HSIAGE"
    architecture "x64"
    startproject "GameEngine"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--Include directories
IncludeDir = {}
IncludeDir["GLFW"] = "GameEngine/3rdParty/GLFW/include"
IncludeDir["SDL"] = "GameEngine/3rdParty/SDL/include"
IncludeDir["GL"] = "GameEngine/3rdParty/GL/include"
IncludeDir["spdlog"] = "GameEngine/3rdParty/spdlog/include"
IncludeDir["glm"] = "GameEngine/3rdParty/glm"
IncludeDir["imgui"] = "GameEngine/3rdParty/imgui"
IncludeDir["entt"] = "GameEngine/3rdParty/entt/single_include"
IncludeDir["imguizmo"] = "GameEngine/3rdParty/ImGuizmo"

--find the other premake files in these folders
include "GameEngine/3rdParty/GLFW/"
include "GameEngine/3rdParty/GL/"
include "GameEngine/3rdParty/imgui"
--include "GameEngine/3rdParty/SDL/"




project "GameEngine"
    location "GameEngine"
    kind "ConsoleApp"
    language "C++"
    staticruntime "on"
    cppdialect "C++17"

    targetdir("bin/" ..outputdir.. "/%{prj.name}")
    objdir("bin-int/" ..outputdir.."%{prj.name}")

    --precomiled header file here
    pchheader "GameEngine_PCH.h"
    pchsource "%{prj.name}/src/GameEngine_PCH.cpp"

    files
    {
        "%{prj.name}/src/",
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{IncludeDir.imguizmo}/**.h",
        "%{IncludeDir.imguizmo}/**.cpp"
    }

    defines
    {
        "_CRT_SECUURE_NO_WARNINGS"
    }

    includedirs
    {
        "%{prj.name}/src/",
        "%{prj.name}/3rdParty/",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GL}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.imguizmo}"
    }

    libdirs
    {
        "%{prj.name}/3rdParty/GLFW/lib/"
    }

    links
    {
        --"SDL2","SDL2main",
        "GLFW",
        "GL3W",
        "imgui"


    }

    filter "files:GameEngine/3rdParty/ImGuizmo/**.cpp"
        flags{"NoPCH"}

    filter "system:windows"
        systemversion "latest"
        buildoptions { "/utf-8"}
        defines
        {
            --any windows system defines that are needed can go here
            --such as IMGUI_DEFINE stuff

            "GLFW_INCLUDE_NONE"

        }