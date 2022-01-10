solution "Keson"

language "C++"
location(_WORKING_DIR)
startproject "Test"
targetprefix "Keson"

configurations { 
	"Debug", 
	"Release"
}

platforms { 
	"native",
}

flags {
	"Cpp17",
	"EnableSSE2",
	"ExtraWarnings",
	"FatalWarnings",
}

configuration "windows"
	defines {
		"WIN32_LEAN_AND_MEAN",
		"VC_EXTRALEAN",
		"NOMINMAX",
		"_USE_MATH_DEFINES",
	}

configuration "Debug"
	targetsuffix "-debug"
	flags {
		"DebugRuntime",
		"Symbols",
	}

configuration "Release"
	targetsuffix "-release"
	flags { 
		"ReleaseRuntime", 
		"OptimizeSpeed", 
		"NoFramePointer", 
		"FloatFast",
		"Symbols",
	}

configuration {}


project "Keson"
	kind "StaticLib"
	targetdir "out"
	files { 
		"src/**",
	}

project "Test"
	kind "ConsoleApp"
	targetdir "out"
	files { 
		"test/**",
	}
	links { 
		"Keson",
	}
	includedirs { 
		"src/",
	}

