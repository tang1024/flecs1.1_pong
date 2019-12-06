cmake_minimum_required (VERSION 3.14)

# target
set(target_name "flecs-math")
file(GLOB_RECURSE source "src/*.c" "src/*.cpp")
file(GLOB_RECURSE header "src/*.h" "include/*h")
add_library(${target_name} ${source} ${header})

# target_include_directories(${target_name} PRIVATE "src")
#message("xxx = ${xxx}")
