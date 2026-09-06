#ifndef GLTOOLS_H
#define GLTOOLS_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glmath.h"

namespace gltools {

    int loadGLFW();
    void setInputMode(GLFWwindow* window, int mode, int value);
    void setVersion(int major, int minor);
    GLFWwindow* createWindow(int width, int height, const char* title);
    void setContextCurrent(GLFWwindow* window);
    bool loadGlad();
    void enable(int setting);
    int maxNumberVertexAttr(void);
    void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void processInput(GLFWwindow* window);
    void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    bool windowShouldClose(GLFWwindow* window);
    void clearColor(vec4 color);
    void clear(int bufferBit);
    void swapBuffers(GLFWwindow* window);
    void pollEvents();
}

#endif
