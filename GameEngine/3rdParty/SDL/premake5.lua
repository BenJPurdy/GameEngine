project "SDL"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" ..outputdir.. "/%{prj.name}")
    objdir ("bin-int/" ..outputdir.. "/%{prj.name}")

    files
    {
        "include/**.h"
    }

    includedirs
    {
        "include/"
    }

    links
    {
        "SDL2"
    }

    flags
    {
        "NoPCH"
    }

filter "system:windows"
    systemversion "latest"
    
filter "configurations:Debug"
    runtime "Debug"
    symbols "on"

filter "configurations:Release"
    runtime "Release"
    symbols "off"
    