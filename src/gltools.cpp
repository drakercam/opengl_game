#include "gltools.h"
#include <GLFW/glfw3.h>

int gltools::loadGLFW() {
    return glfwInit();
}

void gltools::setInputMode(GLFWwindow* window, int mode, int value) {
    glfwSetInputMode(window, mode, value);
}

void gltools::setVersion(int major, int minor) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow* gltools::createWindow(int width, int height, const char* title) {
    return glfwCreateWindow(width, height, title, NULL, NULL);
}

void gltools::setContextCurrent(GLFWwindow* window) {
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, gltools::framebufferSizeCallback);
}

bool gltools::loadGlad() {
    return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

void gltools::enable(int setting) {
    glEnable(setting);
}

void gltools::disable(int setting) {
    glDisable(setting);
}

int gltools::maxNumberVertexAttr(void) {
    int nrAttributes;

    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    return nrAttributes;
}

void gltools::framebufferSizeCallback(GLFWwindow* window, int width, int height) {

    glViewport(0, 0, width, height);
}

void gltools::processInput(GLFWwindow* window) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {

        glfwSetWindowShouldClose(window, true);
    }
}

void gltools::mouseCallback(GLFWwindow* window, double xpos, double ypos) {

}

bool gltools::windowShouldClose(GLFWwindow* window) {
    return glfwWindowShouldClose(window);
}

void gltools::clearColor(vec4 color) {
    glClearColor(color.x, color.y, color.z, color.w);
}

void gltools::clear(int bufferBit) {
    glClear(bufferBit);
}

void gltools::swapBuffers(GLFWwindow* window) {
    glfwSwapBuffers(window);
}

void gltools::pollEvents() {
    glfwPollEvents();
}
