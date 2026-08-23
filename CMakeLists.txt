cmake_minimum_required(VERSION 3.13)
project(minicraft C)

set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE SOURCES "source/*.c")

if(PSP OR SONY_PSP OR CMAKE_SYSTEM_NAME STREQUAL "Generic" OR CMAKE_SYSTEM_NAME STREQUAL "PSP")
    add_definitions(-D__PSP__ -DUSE_SDL1 -D_GNU_SOURCE=1)
    
    add_executable(minicraft ${SOURCES})
    
    target_include_directories(minicraft PRIVATE
        source
        $ENV{PSPDEV}/psp/include/SDL
        $ENV{PSPDEV}/psp/include
        $ENV{PSPDEV}/psp/sdk/include
    )
    
    target_link_libraries(minicraft PRIVATE
        SDL_mixer
        SDLmain
        SDL
        vorbisfile
        vorbis
        ogg
        mad
        mikmod
        pspaudio
        pspgu
        psphprm
        psprtc
        psputility
        pspirkeyb
        psppower
        pspdebug
        pspdisplay
        pspge
        pspctrl
        pspsdk
        pspnet
        pspnet_apctl
        pspnet_inet
        pspnet_resolver
        pspuser
        pspkernel
        m
    )
    
    if(COMMAND create_pbp_file)
        create_pbp_file(
            TARGET minicraft
            TITLE "Minicraft PSP"
            ICON_PATH "${CMAKE_CURRENT_SOURCE_DIR}/ICON0.PNG"
            BACKGROUND_PATH "${CMAKE_CURRENT_SOURCE_DIR}/PIC1.PNG"
            BUILD_PRX
        )
    endif()

elseif(NINTENDO_WII)
    if(NOT COMMAND ogc_create_dol)
        message(FATAL_ERROR "ogc_create_dol not found. Use Wii.cmake toolchain.")
    endif()
    add_definitions(-D__wii__ -DGEKKO -DUSE_SDL1)

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

    ogc_create_dol(minicraft)
else()
    add_executable(minicraft ${SOURCES})
    target_include_directories(minicraft PRIVATE source)
    find_package(SDL REQUIRED)
    find_package(SDL_mixer REQUIRED)
    target_link_libraries(minicraft PRIVATE ${SDL_LIBRARY} ${SDL_MIXER_LIBRARY} m)
endif()
