workspace "HSIAGE"
    architecture "x64"
    startproject "HSIAGE"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--Include directories
IncludeDir = {}
IncludeDir["SDL"] = "GameEngine/3rdParty/SDL/include"
IncludeDir["GL"] = "GameEngine/3rdParty/GL/include"


include "GameEngine/3rdParty/GL/"
include "GameEngine/3rdParty/SDL/"




project "GameEngine"
    location "GameEngine"
    kind "ConsoleApp"
    language "C++"
    staticruntime "on"
    cppdialect "C++20"

    targetdir("bin/" ..outputdir.. "/%{prj.name}")
    objdir("bin-int/" ..outputdir.."%{prj.name}")

    --precomiled header file here
    pchheader "GameEngine_PCH.h"
    pchsource "%{prj.name}/src/GameEngine_PCH.cpp"

    files
    {
        "%{prj.name}/src/",
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    defines
    {
        "_CRT_SECUURE_NO_WARNINGS"
    }

    includedirs
    {
        "%{prj.name}/src/",
        "%{prj.name}/3rdParty/",
        "%{IncludeDir.SDL}",
        "%{IncludeDir.GL}"
    }

    libdirs
    {
        "%{prj.name}/3rdParty/SDL/lib/"
    }

    links
    {
        "SDL2","SDL2main",
        "GL3W"


    }

    filter "system:windows"
        systemversion "latest"
        defines
        {
            --any windows system defines that are needed can go here
            --such as IMGUI_DEFINE stuff

        }