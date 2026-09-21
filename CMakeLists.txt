cmake_minimum_required(VERSION 3.20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Must be set BEFORE the project() command

project(Nanus LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_SYSTEM_NAME STREQUAL "CafeOS")
    option(WIIU "Denote wiiu build" ON)   
else()
    option(WIIU "Denote wiiu build" OFF)
endif()

# Main nanus library
add_library(nanus SHARED
    src/platform/log.cpp
)
file(GLOB_RECURSE BEDROCK CONFIGURE_DEPENDS
    src/bedrock/*.cpp
)
target_sources(nanus PRIVATE
    ${BEDROCK}
)

target_include_directories(nanus
    PUBLIC include
)
target_compile_definitions(nanus PUBLIC
    $<$<CONFIG:Debug>:NANUS_DEBUG>
    $<$<CONFIG:Release>:NANUS_RELEASE>
)
target_compile_options(nanus PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -ffile-prefix-map=${CMAKE_SOURCE_DIR}/=
    -fms-extensions
)

function(libc_backend target)
    message(STATUS "Using standard libc")
    file(GLOB_RECURSE STDLIB CONFIGURE_DEPENDS
        src/platform/stdlib/*.cpp
    )
    target_sources(${target} PRIVATE
        ${STDLIB}
    )
endfunction()

function(cafeos_backend target)
    message(STATUS "Using CafeOS + libc")
    # find_package(wut REQUIRED)

    file(GLOB_RECURSE CAFEOS_SOURCES CONFIGURE_DEPENDS
        src/platform/cafeos/*.cpp
    )
    target_sources(${target} PRIVATE
        ${CAFEOS_SOURCES}
    )
endfunction()

function(opengl_backend target)
    message(STATUS "Using OpenGL backend")
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(SDL2 REQUIRED)

    file(GLOB_RECURSE GL3_SOURCES CONFIGURE_DEPENDS
        src/gfx/gl3/*.cpp
    )
    target_sources(${target} PRIVATE
        ${GL3_SOURCES}
    )
    target_link_libraries(${target} PRIVATE
        OpenGL::GL
        GLEW::glew
        SDL2::SDL2
    )
endfunction()

function(gx2_backend target)
    message(STATUS "Using GX2 backend")
    file(GLOB_RECURSE GX2_SOURCES CONFIGURE_DEPENDS
        src/gfx/gx2/*.cpp
    )
    target_sources(${target} PRIVATE
        ${GX2_SOURCES}
    )
endfunction()

function(sdl2_input_backend target)
    message(STATUS "Using SDL2 input backend")
    find_package(SDL2 REQUIRED)

    file(GLOB_RECURSE SDL2_SOURCES CONFIGURE_DEPENDS
        src/input/sdl2/*.cpp
    )
    target_sources(${target} PRIVATE
        ${SDL2_SOURCES}
    )
    target_link_libraries(${target} PRIVATE
        SDL2::SDL2
    )
endfunction()

if(WIIU)
    cafeos_backend(nanus)
    gx2_backend(nanus)
    sdl2_input_backend(nanus)
elseif(WIN32 OR APPLE OR UNIX)
    # support ansi
    target_compile_definitions(nanus PUBLIC NANUS_ANSI_COLOR)
    libc_backend(nanus)
    opengl_backend(nanus)
    sdl2_input_backend(nanus)
endif()

# Sandbox executable for nanus
add_executable(sandbox
    sandbox/main.cpp
    sandbox/assimp.cpp
)

# Use with assimp
find_package(assimp REQUIRED)
target_link_libraries(sandbox PRIVATE nanus)
target_link_libraries(sandbox PRIVATE assimp::assimp)

target_compile_options(sandbox PRIVATE
    -Wall
    -Wextra
    -ffile-prefix-map=${CMAKE_SOURCE_DIR}/=
)

if(WIIU)
    message(STATUS "Making rpx")
    wut_create_rpx(sandbox)
endif()

message(STATUS "SYSTEM NAME: ${CMAKE_SYSTEM_NAME}")
message(STATUS "C COMPILER: ${CMAKE_C_COMPILER}")
