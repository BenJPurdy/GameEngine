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
IncludeDir["yamlcpp"] = "GameEngine/3rdParty/yamlcpp/include"
IncludeDir["box2d"] = "GameEngine/3rdParty/box2d/include"
IncludeDir["fmod"] = "GameEngine/3rdParty/fmod"



--find the other premake files in these folders
include "GameEngine/3rdParty/GLFW/"
include "GameEngine/3rdParty/GL/"
include "GameEngine/3rdParty/imgui"
include "GameEngine/3rdParty/yamlcpp"
include "GameEngine/3rdParty/box2d"
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
        "%{IncludeDir.imguizmo}/**.cpp",
        "%{IncludeDir.imgui}/imgui_stdlib.h",
        "%{IncludeDir.imgui}/imgui_stdlib.cpp",
        "%{prj.name}/natvis/**.natvis"
    }

    defines
    {
        "_CRT_SECUURE_NO_WARNINGS",
        "YAML_CPP_STATIC_DEFINE"
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
        "%{IncludeDir.imguizmo}",
        "%{IncludeDir.yamlcpp}",
        "%{IncludeDir.box2d}",
        "%{IncludeDir.fmod}/include"
    }

    libdirs
    {
        "%{prj.name}/3rdParty/GLFW/lib/",
        "%{IncludeDir.fmod}/lib"
    }

    links
    {
        --"SDL2","SDL2main",
        "GLFW",
        "GL3W",
        "imgui",
        "yaml-cpp",
        "box2d",
        "fmod_vc"

    }


    filter {"action:vs*"}
        vpaths{["Visualization"] = {"**.natvis"}}

    filter {"files:**.natvis"}
        removeflags{"ExcludeFromBuild"}
        buildaction "None"
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

    filter "configurations:Debug"
        debugenvs {
            "PATH=3rdParty\\w64devkit\\bin"
        }
        defines {"DEBUG"}
        staticruntime "off"
        runtime "Debug"
        symbols "On"
    
    filter "configurations:Release"
        staticruntime "off"
        runtime "Release"
        optimize "on"
    
    filter "configurations:Dist"
        runtime "Release"
        optimize "on"
        symbols "Off"