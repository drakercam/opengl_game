cmake_minimum_required(VERSION 3.13)

project(untitled
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for clangd/Kate
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------------------------------------------------
# COMPILER CACHE
# ---------------------------------------------------------

find_program(CCACHE_PROGRAM ccache)

if(CCACHE_PROGRAM)
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()

# ---------------------------------------------------------
# OPTIONS
# ---------------------------------------------------------

option(ENABLE_SANITIZERS "Enable AddressSanitizer and UBSan" OFF)

# ---------------------------------------------------------
# DEPENDENCIES
# ---------------------------------------------------------

find_package(OpenGL REQUIRED)
find_package(SDL3 REQUIRED CONFIG)
find_package(assimp REQUIRED)
find_package(Freetype REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(LUA REQUIRED lua5.4)

# ---------------------------------------------------------
# EXECUTABLE
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
# INCLUDES
# ---------------------------------------------------------

target_include_directories(${PROJECT_NAME} PRIVATE
    src
    include
    game
    thirdparty/stb_image
    ${LUA_INCLUDE_DIRS}
)

# ---------------------------------------------------------
# LIBRARIES
# ---------------------------------------------------------

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
    OpenGL::GL
    assimp::assimp
    Freetype::Freetype
    ${LUA_LIBRARIES}
)

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

if(ENABLE_SANITIZERS)

    target_compile_options(${PROJECT_NAME} PRIVATE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
    )

    target_link_options(${PROJECT_NAME} PRIVATE
        -fsanitize=address,undefined
    )

endif()
