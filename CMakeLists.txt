cmake_minimum_required(VERSION 3.13)

project(opengl_game
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for clangd/Kate
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/filehandling.cpp
    src/glbuffer.cpp
    src/glcamera.cpp
    src/glmesh.cpp
    src/glprimitives.cpp
    src/gltime.cpp
    src/glshader.cpp
    src/gltools.cpp
    src/gltexture.cpp
    src/project.cpp
    thirdparty/glad/src/glad.c
    thirdparty/stb_image/stb_image_definition.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    src
    include
    thirdparty/glad/include
    thirdparty/stb_image
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    glfw
    assimp::assimp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    TARGET_OPENGL
)
