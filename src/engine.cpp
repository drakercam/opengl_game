#include "engine.h"

void engine::initialize() {
    if (!gltools::loadGLFW()) {
        std::cout << "Failed to load glfw" << std::endl;
        terminate();
        return;
    }

    gltools::setVersion(3, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = gltools::createWindow(1366, 768, "untitled");
    if (!window){

        std::cout << "Failed to create glfw window" << std::endl;
        terminate();
        return;
    }

    gltools::setContextCurrent(window);

    if (!gltools::loadGlad()) {
        std::cout << "Couldn't load opengl::glad" << std::endl;
        terminate();
        return;
    }

    gltools::enable(GL_DEPTH_TEST);
    gltools::enable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetCursorPosCallback(window, gltools::mouseCallback);

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    std::cout << "GL Vendor: " << vendor << std::endl;
    std::cout << "GL Renderer: " << renderer << std::endl;

    // initialize free type
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    gltools::setInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void engine::terminate() {
    glfwDestroyWindow(window);
    glfwTerminate();

    window = nullptr;
}
