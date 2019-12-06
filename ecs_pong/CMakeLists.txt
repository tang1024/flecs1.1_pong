cmake_minimum_required (VERSION 3.14)

# target
set(target_name "ecs_pong")
file(GLOB source "src/*.c" "src/*.cpp")
file(GLOB header "src/*.h" "include/*h")
add_executable(${target_name} ${source} ${header})

target_link_libraries(
    ${target_name} 
    
    "SDL2.lib"
    
    "flecs_static"
    
    "flecs-math"
    
    "flecs-components-transform"
    "flecs-components-physics"
    "flecs-components-geometry"
    "flecs-components-graphics"
    "flecs-components-input"
    
    "flecs-systems-transform"
    "flecs-systems-physics"
    "flecs-systems-sdl2"
)

target_compile_definitions(${target_name} PRIVATE FLECS_STATIC)

# visual studio executable debug directory
if(MSVC)
	set_target_properties(${target_name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
endif()
