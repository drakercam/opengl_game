cmake_minimum_required(VERSION 3.16)

project(untitled
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# ---------------------------------------------------------
# PLATFORM
# ---------------------------------------------------------

if(EMSCRIPTEN)

    message(STATUS "Building for Emscripten")

    set(CMAKE_EXECUTABLE_SUFFIX ".html" CACHE INTERNAL "")

else()

    message(STATUS "Building for native platform")

endif()


# ---------------------------------------------------------
# OPTIONS
# ---------------------------------------------------------

option(ENABLE_SANITIZERS
    "Enable AddressSanitizer and UBSan"
    OFF
)


# ---------------------------------------------------------
# COMPILER CACHE
# ---------------------------------------------------------

find_program(CCACHE_PROGRAM ccache)

if(CCACHE_PROGRAM AND NOT EMSCRIPTEN)

    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})

endif()


# ---------------------------------------------------------
# VENDORED DEPENDENCIES
# ---------------------------------------------------------

# SDL3
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_X11_XTEST OFF CACHE BOOL "" FORCE)

add_subdirectory(
    thirdparty/SDL
    EXCLUDE_FROM_ALL
)


# Assimp
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)

add_subdirectory(thirdparty/assimp EXCLUDE_FROM_ALL)

if(TARGET assimp)
    target_compile_options(assimp PRIVATE
        -Wno-unsafe-buffer-usage
    )
endif()

# FreeType
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(
    thirdparty/freetype
    EXCLUDE_FROM_ALL
)


# ---------------------------------------------------------
# OPENGL
# ---------------------------------------------------------

if(NOT EMSCRIPTEN)

    find_package(OpenGL REQUIRED)

endif()


# ---------------------------------------------------------
# GAME
# ---------------------------------------------------------

add_executable(${PROJECT_NAME}

    src/filehandling.cpp
    src/glbuffer.cpp
    src/glcamerafirst.cpp
    src/glcamerathird.cpp
    src/glmesh.cpp
    src/gltime.cpp
    src/glshader.cpp
    src/gltools.cpp
    src/gltexture.cpp
    src/glmodel.cpp
    src/text.cpp
    src/engine.cpp

    game/player.cpp
    game/enemy.cpp
    game/game.cpp
    game/main.cpp

    thirdparty/stb_image/stb_image_definition.cpp

)


# ---------------------------------------------------------
# INCLUDE DIRECTORIES
# ---------------------------------------------------------

target_include_directories(${PROJECT_NAME} PRIVATE

    src
    include
    game
    thirdparty/stb_image

)


# ---------------------------------------------------------
# LIBRARIES
# ---------------------------------------------------------

target_link_libraries(${PROJECT_NAME} PRIVATE

    SDL3::SDL3
    assimp
    freetype

)

if(NOT EMSCRIPTEN)

    target_link_libraries(${PROJECT_NAME} PRIVATE

        OpenGL::GL

    )

endif()


# ---------------------------------------------------------
# WEB
# ---------------------------------------------------------

if(EMSCRIPTEN)

    target_link_options(${PROJECT_NAME} PRIVATE

        "-sMIN_WEBGL_VERSION=2"
        "-sMAX_WEBGL_VERSION=2"
        "-sALLOW_MEMORY_GROWTH=1"

        "--preload-file=${CMAKE_SOURCE_DIR}/resources@/resources"
        "--preload-file=${CMAKE_SOURCE_DIR}/shaders@/shaders"
        "--preload-file=${CMAKE_SOURCE_DIR}/models@/models"

    )

endif()

# ---------------------------------------------------------
# DEFINITIONS
# ---------------------------------------------------------

target_compile_definitions(${PROJECT_NAME} PRIVATE

    TARGET_OPENGL

)


# ---------------------------------------------------------
# DEBUG
# ---------------------------------------------------------

target_compile_options(${PROJECT_NAME} PRIVATE

    -g

)


# ---------------------------------------------------------
# SANITIZERS
# ---------------------------------------------------------

if(ENABLE_SANITIZERS AND NOT EMSCRIPTEN)

    target_compile_options(${PROJECT_NAME} PRIVATE

        -fsanitize=address,undefined
        -fno-omit-frame-pointer

    )

    target_link_options(${PROJECT_NAME} PRIVATE

        -fsanitize=address,undefined

    )

endif()
