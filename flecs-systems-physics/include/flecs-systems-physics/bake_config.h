/*
                                   )
                                  (.)
                                  .|.
                                  | |
                              _.--| |--._
                           .-';  ;`-'& ; `&.
                          \   &  ;    &   &_/
                           |"""---...---"""|
                           \ | | | | | | | /
                            `---.|.|.|.---'

 * This file is generated by bake.lang.c for your convenience. Headers of
 * dependencies will automatically show up in this file. Include bake_config.h
 * in your main project file. Do not edit! */

#ifndef FLECS_SYSTEMS_PHYSICS_BAKE_CONFIG_H
#define FLECS_SYSTEMS_PHYSICS_BAKE_CONFIG_H

/* Headers of public dependencies */
#include <flecs.h>
#include <flecs_math.h>
#include <flecs_components_transform.h>
#include <flecs_components_geometry.h>
#include <flecs_components_physics.h>
#include <flecs_systems_transform.h>

/* Headers of private dependencies */
#ifdef FLECS_SYSTEMS_PHYSICS_IMPL
/* No dependencies */
#endif

/* Convenience macro for exporting symbols */
#ifndef FLECS_SYSTEMS_PHYSICS_STATIC
  #if FLECS_SYSTEMS_PHYSICS_IMPL && (defined(_MSC_VER) || defined(__MINGW32__))
    #define FLECS_SYSTEMS_PHYSICS_EXPORT __declspec(dllexport)
  #elif FLECS_SYSTEMS_PHYSICS_IMPL
    #define FLECS_SYSTEMS_PHYSICS_EXPORT __attribute__((__visibility__("default")))
  #elif defined _MSC_VER
    #define FLECS_SYSTEMS_PHYSICS_EXPORT __declspec(dllimport)
  #else
    #define FLECS_SYSTEMS_PHYSICS_EXPORT
  #endif
#else
  #define FLECS_SYSTEMS_PHYSICS_EXPORT
#endif

#endif

