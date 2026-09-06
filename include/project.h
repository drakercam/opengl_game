#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "filehandling.h"
#include "glbuffer.h"
#include "glmath.h"
#include "glmesh.h"
#include "glshader.h"
#include "gltools.h"
#include "gltexture.h"
#include "gltime.h"
#include "glcamera.h"
#include "glprimitives.h"

namespace project {

    struct dataGPU {
        buffer::data<mesh::data> meshes;
        buffer::data<shader::data> shaders;
        buffer::data<texture::data> textures;
    };

    struct dataCPU {
        GLFWwindow* window;
    };

    void initResourcesGPU(dataGPU& gpuData);
    int initResourcesCPU(dataCPU& cpuData);

    void update(dataCPU& cpuData, gltime::data& time);
    void loop(dataCPU& cpuData, dataGPU& gpuData);
    void terminate();
}

#endif
