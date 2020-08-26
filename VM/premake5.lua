workspace "VirtualMachine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--Include directories relative to root
IncludeDir = {}

project "VirtualMachine"
	location "VirtualMachine"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"VirtualMachine/src"
	}

	links
	{
		"VirtualMachine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"LE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "LE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "LE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "LE_DIST"
		optimize "On"