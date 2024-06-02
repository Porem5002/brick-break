workspace "brick-breaker"  
    configurations { "Debug", "Release" } 

project "brick-breaker"   
    language "C++"
    cppdialect "C++17"
    warnings "High"
    
    targetdir "bin/%{cfg.buildcfg}" 
    files { "src/vendor/**.h", "src/cpp/**.hpp", "src/cpp/**.cpp" }

    includedirs { "src/vendor/include" }

    libdirs { "./lib" }
    links { "mingw32", "SDL2main", "SDL2" }
    
    postbuildcommands {
        "{COPY} %{cfg.buildtarget.relpath} ./",
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        kind "ConsoleApp"  
        symbols "On" 
    
    filter "configurations:Release"  
        defines { "NDEBUG" }
        kind "WindowedApp"    
        optimize "On"