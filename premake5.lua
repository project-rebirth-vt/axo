workspace "axo"
  configurations { "debug", "release" }
  architecture "x86_64"
  location "build"
  startproject "axo"

  flags { "multiprocessorcompile" }

project "lua"
  language "c"
  cdialect "c17"

  targetdir "%{wks.location}/lua/bin/%{cfg.buildcfg}"
  objdir "%{wks.location}/lua/obj/%{cfg.buildcfg}"

  files { "vendor/lua/*.h", "vendor/lua/*.c" }
  includedirs { "vendor/lua" }

  filter "system:windows"
    kind "sharedlib"
    defines { "LUA_BUILD_AS_DLL" }
    targetname "lua54"

  filter "system:linux"
    kind "staticlib"

  filter "configurations:debug"
    defines { "DEBUG" }
    symbols "on"

  filter "configurations:release"
    defines { "NDEBUG" }
    optimize "on"

project "physfs"
  kind "staticlib"
  language "c"
  cdialect "c17"

  targetdir "%{wks.location}/physfs/bin/%{cfg.buildcfg}"
  objdir "%{wks.location}/physfs/obj/%{cfg.buildcfg}"

  files { "vendor/physfs/*.h", "vendor/physfs/*.c" }
  includedirs { "vendor/physfs" }

  defines {
    "PHYSFS_SUPPORTS_7Z=0",
    "PHYSFS_SUPPORTS_GRP=0",
    "PHYSFS_SUPPORTS_WAD=0",
    "PHYSFS_SUPPORTS_CSM=0",
    "PHYSFS_SUPPORTS_HOG=0",
    "PHYSFS_SUPPORTS_MVL=0",
    "PHYSFS_SUPPORTS_QPAK=0",
    "PHYSFS_SUPPORTS_SLB=0",
    "PHYSFS_SUPPORTS_ISO9660=0",
    "PHYSFS_SUPPORTS_VDF=0",
    "PHYSFS_SUPPORTS_LECARCHIVES=0",
  }

  filter "configurations:debug"
    defines { "DEBUG" }
    symbols "on"

  filter "configurations:release"
    defines { "NDEBUG" }
    optimize "on"

project "axo"
  kind "consoleapp"
  language "c"
  cdialect "c17"
  warnings "extra"

  targetdir "%{wks.location}/bin/%{cfg.buildcfg}"
  objdir "%{wks.location}/obj/%{cfg.buildcfg}"

  files { "src/**.h", "src/**.c" }

  includedirs {
    "vendor/sdl/include",
    "vendor/glad/include",
    "vendor/lua",
    "vendor/physfs",
    "vendor/linmath",
  }

  libdirs { "vendor/sdl/lib" }
  links { "SDL3", "lua", "physfs" }

  filter "system:windows"
    files { "assets/res.rc" }
    postbuildcommands { "{COPYFILE} ../vendor/sdl/lib/SDL3.dll %{cfg.targetdir}" }
    postbuildcommands { "{COPYFILE} lua/bin/%{cfg.buildcfg}/lua54.dll %{cfg.targetdir}" }

  filter "system:linux"
    links { "m" }
    linkoptions { "-rdynamic" }

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
