cmake_minimum_required(VERSION 3.13)
project(minicraft C)

set(CMAKE_C_STANDARD 11)

if(NINTENDO_WII)
    if(NOT COMMAND ogc_create_dol)
        message(FATAL_ERROR "ogc_create_dol not found. Use Wii.cmake toolchain.")
    endif()
    add_definitions(-D__wii__ -DGEKKO -DUSE_SDL1)
endif()

file(GLOB_RECURSE SOURCES "source/*.c")

add_executable(minicraft ${SOURCES})

target_include_directories(minicraft PRIVATE
    source
    $ENV{DEVKITPRO}/libogc/include
    $ENV{DEVKITPRO}/portlibs/wii/include
    $ENV{DEVKITPRO}/portlibs/wii/include/SDL
)

link_directories(
    $ENV{DEVKITPRO}/portlibs/wii/lib
    $ENV{DEVKITPRO}/portlibs/ppc/lib
    $ENV{DEVKITPRO}/libogc/lib/wii
)

target_link_libraries(minicraft PRIVATE
    SDLmain
    SDL_mixer
    modplug
    stdc++
    vorbisfile
    vorbis
    ogg
    mad
    SDL
    wiikeyboard
    asnd
    aesnd
    fat
    wiiuse
    bte
    ogc
    m
)

if(NINTENDO_WII)
    ogc_create_dol(minicraft)
endif()
