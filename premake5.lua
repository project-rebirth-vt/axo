workspace "axo"
  configurations { "debug", "release" }
  architecture "x86_64"
  location "build"
  startproject "axo"

  flags { "multiprocessorcompile" }
  warnings "extra"

project "axo"
  kind "consoleapp"
  language "c"
  cdialect "c17"

  targetdir "%{wks.location}/bin/%{cfg.buildcfg}"
  objdir "%{wks.location}/obj/%{cfg.buildcfg}"

  files { "src/**.h", "src/**.c" }
  includedirs { "vendor/glad/include" }
  links { "SDL3" }

  filter "system:windows"
    files { "assets/res.rc" }
    includedirs { "vendor/sdl/include" }
    libdirs { "vendor/sdl/lib" }

    postbuildcommands { "{COPYFILE} ../vendor/sdl/lib/SDL3.dll %{cfg.targetdir}" }

  filter "configurations:debug"
    defines { "DEBUG" }
    symbols "on"

  filter "configurations:release"
    defines { "NDEBUG" }
    optimize "on"
