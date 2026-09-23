#ifndef ENGINE_H
#define ENGINE_H


#include <freetype/freetype.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// ENGINE TOOLING

#include "filehandling.h"
#include "glbuffer.h"
#include "glmath.h"
#include "glmesh.h"
#include "glshader.h"
#include "gltools.h"
#include "gltexture.h"
#include "gltime.h"
#include "glcamerafirst.h"
#include "glcamerathird.h"
#include "glmodel.h"
#include "text.h"

class engine {

public:

    void initialize();
    void terminate();

    GLFWwindow* getWindow(void) const { return window; }
    FT_Library& getFTLibrary(void) { return ft; }

private:
    GLFWwindow* window{nullptr};
    FT_Library ft{nullptr};

};

#endif
