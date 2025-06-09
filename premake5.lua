workspace "axo"
  configurations { "debug", "release" }
  architecture "x86_64"
  location "build"

  warnings "extra"

project "axo"
  kind "consoleapp"
  language "c"
  cdialect "c17"

  targetdir "%{wks.location}/bin/%{cfg.buildcfg}"
  objdir "%{wks.location}/obj/%{cfg.buildcfg}"

  files { "src/**.h", "src/**.c" }
  includedirs { "vendor/sdl/include", "vendor/glad/include" }
  libdirs { "vendor/sdl/lib" }
  links { "SDL3" }

  filter "system:linux"
    postbuildcommands {
      "{COPYDIR} ../assets/appdir %{cfg.targetdir}",
      "{MKDIR} %{cfg.targetdir}/appdir/usr/bin",
      "{MOVE} %{cfg.targetdir}/axo %{cfg.targetdir}/appdir/usr/bin/axo",
      "appimagetool %{cfg.targetdir}/appdir %{cfg.targetdir}/axo.AppImage > /dev/null 2>&1",
      "{RMDIR} %{cfg.targetdir}/appdir",
    }

  filter "configurations:debug"
    defines { "DEBUG" }
    symbols "on"

  filter "configurations:release"
    defines { "NDEBUG" }
    optimize "on"
