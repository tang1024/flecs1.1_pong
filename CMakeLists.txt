cmake_minimum_required (VERSION 3.14)
project (flecs11)

##############################################################
# Build Mode
##############################################################
set(CMAKE_BUILD_TYPE "Debug")

add_compile_definitions(
ECS_PONG_STATIC

FLECS_STATIC
FLECS_MATH_STATIC

FLECS_COMPONENTS_PHYSICS_STATIC
FLECS_COMPONENTS_INPUT_STATIC
FLECS_COMPONENTS_GRAPHICS_STATIC
FLECS_COMPONENTS_GEOMETRY_STATIC
FLECS_COMPONENTS_TRANSFORM_STATIC

FLECS_SYSTEMS_SDL2_STATIC
FLECS_SYSTEMS_TRANSFORM_STATIC
FLECS_SYSTEMS_PHYSICS_STATIC
)

##############################################################
# Compiler
##############################################################
#set(CMAKE_CXX_COMPILER "g++")

# Compiler Flags
# for gcc... http://man7.org/linux/man-pages/man1/gcc.1.html
if(MinGW)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99") 
endif()
message("CMAKE_C_FLAGS = ${CMAKE_C_FLAGS}")

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14")
endif()
message("CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")

#set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
#set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# MSVC
if(MSVC)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS) # _CRT_SECURE_NO_WARNINGS for fopen(), strcpy(), ...
endif()

##############################################################
# Platform
##############################################################
# win32
# WIN32: Defined in WinDef.h. Stands for Win32 Platform.
# _WIN32: Defined for applications for Win32 and Win64. Always defined.
# _WIN64: Defined for applications for Win64.

if(WIN32)
    message("Win32 CMake Build")
    add_compile_definitions(WIN32 __WIN32__ WIN32_LEAN_AND_MEAN NOMINMAX)
endif()

if(WIN64)
    message("Win32 X64 CMake Build")
    add_compile_definitions(_WIN64)
else()
    message("Win32 X64 CMake Build")
    add_compile_definitions(_WIN32)
endif()

##############################################################
# Output Directories
##############################################################
message("PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")

# global: 		CMAKE_<ARTIFACT>_OUTPUT_DIRECTORY_<CONFIGURATION>
# per-target: 	<ARTIFACT>_OUTPUT_DIRECTORY_<CONFIGURATION>
set(BUILD_TARGET_ROOT_DIR "${PROJECT_SOURCE_DIR}/target") # or "../../target"
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${BUILD_TARGET_ROOT_DIR}/lib_d/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${BUILD_TARGET_ROOT_DIR}/lib/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BUILD_TARGET_ROOT_DIR}/bin_d/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BUILD_TARGET_ROOT_DIR}/bin/")

##############################################################
# Include Directories
##############################################################
include_directories(
    "SDL2-2.0.10/include"
    
    "flecs-1.1/include"
    
    "flecs-math/include"
    
    "flecs-components-transform/include"
    "flecs-components-physics/include"
    "flecs-components-geometry/include"
    "flecs-components-graphics/include"
    "flecs-components-input/include"
    
    "flecs-systems-transform/include"
    "flecs-systems-physics/include"
    "flecs-systems-sdl2/include"
    
    "ecs_pong/include"
)

##############################################################
# Link Directories
##############################################################
link_directories(
	"SDL2-2.0.10/lib/x64"
    
	${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}
	${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE}
)

##############################################################
# Static Library Targets
##############################################################
add_subdirectory("flecs-1.1")
add_subdirectory("flecs-math")

add_subdirectory("flecs-components-transform")
add_subdirectory("flecs-components-physics")
add_subdirectory("flecs-components-geometry")
add_subdirectory("flecs-components-graphics")
add_subdirectory("flecs-components-input")

add_subdirectory("flecs-systems-transform")
add_subdirectory("flecs-systems-physics")
add_subdirectory("flecs-systems-sdl2")
##############################################################
# Executable Targets
##############################################################
add_subdirectory("ecs_pong")
