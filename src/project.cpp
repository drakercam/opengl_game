#include "project.h"
#include "filehandling.h"
#include "glbuffer.h"
#include "glcamerathird.h"
#include "glprimitives.h"
#include "glshader.h"
#include "gltools.h"
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H

int glengine::initialize() {

    if (!gltools::loadGLFW()) {
        return -1;
    }

    gltools::setVersion(3, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    dgl.window = gltools::createWindow(1366, 768, "untitled");
    if (!dgl.window){

        std::cout << "Failed to create glfw window" << std::endl;
        return -1;
    }

    gltools::setContextCurrent(dgl.window);

    if (!gltools::loadGlad()) {
        std::cout << "Couldn't load opengl" << std::endl;
        glengine::terminate();
        return -1;
    }

    gltools::enable(GL_DEPTH_TEST);

    // glfwSetCursorPosCallback(cpuData.window, gltools::mouseCallback);

    int four;

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    std::cout << "GL Vendor: " << vendor << std::endl;
    std::cout << "GL Renderer: " << renderer << std::endl;

    return 0;
}

void glengine::initializeResources() {

    gd.shaders.getElements().reserve(10);

    gd.textures.emplace("../resources/cobblestone.jpg");
    std::vector<size_t> textureRefs = { 0 };

    gd.meshes.getElements().reserve(100);

    gd.shaders.emplace(file::read("../shaders/basic_vertex1.glsl").c_str(), file::read("../shaders/basic_frag1.glsl").c_str());

    gd.models.emplace("../models/penguin/PenguinBaseMesh.obj", gd.meshes, gd.textures);

    gd.cubes.emplace(std::vector<size_t>{0});

    auto& shader = gd.shaders.get(0);
    shader.bind();
    shader.intLoad(shader.getUniformLocation("tex1"), 0);
    shader.unbind();

    // create free type shaders + text
    gd.shaders.emplace(file::read("../shaders/text_vertex.glsl").c_str(), file::read("../shaders/text_frag.glsl").c_str());

    font yellowBanana = initializeTextRenderResources();
    yellowBanana.shaderRef = 1;
    gd.fonts.emplace(yellowBanana);
}

void glengine::update(gltime& time) {

    return;
}

void glengine::run() {

    vec3 cameraPosition{0.0f, 0.0f, 3.0f};
    vec3 cameraTarget{0.0f, 1.0f, 0.0f};
    vec3 up{0.0f, 1.0f, 0.0f};
    vec3 cameraDirection{vec3::normalize(cameraPosition - cameraTarget)};

    camerathird camera{cameraPosition, cameraTarget, cameraDirection, {0.0f, 0.0f, -1.0f}, vec3::normalize(vec3::cross(up, cameraDirection)), up};
    gltools::setInputMode(dgl.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    mat4 modelMatrix, rotation, translation, scale;
    mat4 viewMatrix, projectionMatrix;

    mat4::identity(projectionMatrix);
    mat4::getProjection(projectionMatrix, ops::degreesToRadians(45.0f), 1366.0f / 768.0f, 0.1f, 100.0f);

    gltime time;
    input in;

    auto& shader = gd.shaders.get(0);
    auto& cube = gd.cubes.get(0);
    // auto& rectangle = gd.rectangles.get(0);

    player player{{0.0f, 0.0f, 0.0f}, 0};

    // for text rendering
    mat4 ortho;
    mat4::identity(ortho);
    mat4::getOrthographic(ortho, 0.0f, 1366.0f, 0.0f, 768.0f, -1.0f, 1.0f);

    text sample{ 0, "hello world", {50.0f, 50.0f}, 1.0f };

    while (!gltools::windowShouldClose(dgl.window)){

        time.update();

        gltools::clearColor({0.0f, 0.0f, 0.0, 1.0f});
        gltools::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        player.update(dgl.window, in, time, camera.getFront(), camera.getRight());

        //camera.inputKeyboard(dgl.window, in, time);
        camera.inputMouse(dgl.window);
        camera.setTarget(player.getPosition() + (vec3){0.0f, 0.5f, 0.0f});
        camera.update(viewMatrix, time);

        shader.bind();

        shader.mat4Load(shader.getUniformLocation("view"), viewMatrix);
        shader.mat4Load(shader.getUniformLocation("projection"), projectionMatrix);

        player.draw(shader, gd.models, gd.meshes, gd.textures);

        mat4::identity(modelMatrix);
        mat4::translate(modelMatrix, {0.0f, -0.25f, 0.0f});
        mat4::scale(modelMatrix, {20.0f, 0.5f, 20.0f});

        shader.mat4Load(shader.getUniformLocation("model"), modelMatrix);

        cube.draw(shader, gd.textures);

        shader.unbind();

        // Text rendering
        sample.draw(gd.fonts, gd.textures, gd.shaders, dgl.textVAO, dgl.textVBO, ortho);

        gltools::swapBuffers(dgl.window);
        gltools::pollEvents();
    }
}

void glengine::terminate() {

    gd.clear();

    glfwDestroyWindow(dgl.window);
    glfwTerminate();

    dgl.window = nullptr;
}
