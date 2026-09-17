#include "project.h"
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H

// ---------------------------------------------------------
// GAME RELATED CODE
// ---------------------------------------------------------

enum CAMERA_MODE {
    FIRST_PERSON,
    THIRD_PERSON
};

void game::init() {

    eng.initialize();

    rd.shaders.getElements().reserve(10);

    rd.textures.emplace("../resources/cobblestone.jpg");
    std::vector<size_t> textureRefs = { 0 };

    rd.meshes.getElements().reserve(100);

    rd.shaders.emplace(file::read("../shaders/basic_vertex1.glsl").c_str(), file::read("../shaders/basic_frag1.glsl").c_str());

    rd.models.emplace("../models/penguin/PenguinBaseMesh.obj", rd.meshes, rd.textures);

    rd.cubes.emplace(std::vector<size_t>{0});

    auto& shader = rd.shaders.get(0);
    shader.bind();
    shader.intLoad(shader.getUniformLocation("tex1"), 0);
    shader.unbind();

    // create free type shaders + text
    rd.shaders.emplace(file::read("../shaders/text_vertex.glsl").c_str(), file::read("../shaders/text_frag.glsl").c_str());

    rd.fonts.emplace("../resources/Yellow Banana.otf", 1, eng.getFTLibrary(), rd.textures);
}

void game::terminate() {

    rd.clear();

    eng.terminate();
}

void game::update(gltime& time) {

    return;
}

void game::run() {

    vec3 cameraPosition{0.0f, 0.0f, 3.0f};
    vec3 cameraTarget{0.0f, 1.0f, 0.0f};
    vec3 up{0.0f, 1.0f, 0.0f};
    vec3 cameraDirection{vec3::normalize(cameraPosition - cameraTarget)};

    camerathird camera{cameraPosition, cameraTarget, cameraDirection, {0.0f, 0.0f, -1.0f}, vec3::normalize(vec3::cross(up, cameraDirection)), up};

    camerafirst camera2{cameraPosition, cameraTarget, cameraDirection, {0.0f, 0.0f, -1.0f}, vec3::normalize(vec3::cross(up, cameraDirection)), up};

    CAMERA_MODE camMode = CAMERA_MODE::THIRD_PERSON;

    mat4 modelMatrix, rotation, translation, scale;
    mat4 viewMatrix, projectionMatrix;

    mat4::identity(projectionMatrix);
    mat4::getProjection(projectionMatrix, ops::degreesToRadians(45.0f), 1366.0f / 768.0f, 0.1f, 100.0f);

    gltime time;
    input in;

    auto& shader = rd.shaders.get(0);
    auto& cube = rd.cubes.get(0);
    // auto& rectangle = gd.rectangles.get(0);

    player player{{0.0f, 0.0f, 0.0f}, 0};

    // for text rendering
    mat4 ortho;
    mat4::identity(ortho);
    mat4::getOrthographic(ortho, 0.0f, 1366.0f, 0.0f, 768.0f, -1.0f, 1.0f);

    text cF{"Press F to switch to first person camera", {50.0f, 680.0f}, 0.5f, 0};
    text cT{"Press T to switch to third person camera", {50.0f, 630.0f}, 0.5f, 0};

    while (!gltools::windowShouldClose(eng.getWindow())){

        time.update();

        gltools::clearColor({0.0f, 0.0f, 0.0, 1.0f});
        gltools::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // CAMERA
        if (in.isKeyPressed(eng.getWindow(), GLFW_KEY_F)) {
            camMode = CAMERA_MODE::FIRST_PERSON;
        }
        else if (in.isKeyPressed(eng.getWindow(), GLFW_KEY_T)) {
            camMode = CAMERA_MODE::THIRD_PERSON;
        }

        if (camMode == CAMERA_MODE::THIRD_PERSON) {
            player.update(eng.getWindow(), in, time, camera.getFront(), camera.getRight());
        }

        if (camMode == CAMERA_MODE::FIRST_PERSON) {
            camera2.inputMouse(eng.getWindow());
            camera2.inputKeyboard(eng.getWindow(), in, time);
            camera2.update(viewMatrix, time);
        }
        else if (camMode == CAMERA_MODE::THIRD_PERSON) {
            camera.inputMouse(eng.getWindow());
            camera.setTarget(player.getPosition() + (vec3){0.0f, 0.5f, 0.0f});
            camera.update(viewMatrix, time);
        }

        shader.bind();

        shader.mat4Load(shader.getUniformLocation("view"), viewMatrix);
        shader.mat4Load(shader.getUniformLocation("projection"), projectionMatrix);

        player.draw(shader, rd.models, rd.meshes, rd.textures);

        mat4::identity(modelMatrix);
        mat4::translate(modelMatrix, {0.0f, -0.25f, 0.0f});
        mat4::scale(modelMatrix, {20.0f, 0.5f, 20.0f});

        shader.mat4Load(shader.getUniformLocation("model"), modelMatrix);

        cube.draw(shader, rd.textures);

        shader.unbind();

        // Text rendering
        cF.draw(rd.fonts, rd.textures, rd.shaders, ortho);
        cT.draw(rd.fonts, rd.textures, rd.shaders, ortho);

        gltools::swapBuffers(eng.getWindow());
        gltools::pollEvents();
    }
}
