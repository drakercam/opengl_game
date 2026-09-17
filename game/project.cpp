#include "project.h"
#include "gltexture.h"
#include "gltools.h"
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
    rd.textures.getElements().reserve(10);
    rd.rectangles.getElements().reserve(10);

    rd.textures.emplace("../resources/cobblestone.jpg");
    rd.textures.emplace("../resources/pistol1.png");
    rd.textures.emplace("../resources/shotgun1.png");

    rd.meshes.getElements().reserve(100);

    rd.shaders.emplace(file::read("../shaders/basic_vertex1.glsl").c_str(), file::read("../shaders/basic_frag1.glsl").c_str());
    rd.shaders.emplace(file::read("../shaders/weapon_vertex.glsl").c_str(), file::read("../shaders/weapon_frag.glsl").c_str());
    rd.shaders.emplace(file::read("../shaders/basic_vertex2.glsl").c_str(), file::read("../shaders/basic_frag2.glsl").c_str());
    rd.shaders.emplace(file::read("../shaders/text_vertex.glsl").c_str(), file::read("../shaders/text_frag.glsl").c_str());

    rd.models.emplace("../models/penguin/PenguinBaseMesh.obj", rd.meshes, rd.textures);

    rd.cubes.emplace(std::vector<size_t>{0});
    rd.rectangles.emplace(std::vector<size_t>{1});
    rd.rectangles.emplace();    // crosshair

    auto& shader = rd.shaders.get(0);
    shader.bind();
    shader.intLoad(shader.getUniformLocation("tex1"), 0);
    shader.unbind();

    auto& weaponshader = rd.shaders.get(1);
    weaponshader.bind();
    weaponshader.intLoad(weaponshader.getUniformLocation("tex"), 1);
    weaponshader.unbind();

    rd.fonts.emplace("../resources/Yellow Banana.otf", 3, eng.getFTLibrary(), rd.textures);
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
    vec3 camera2Offset{0.0f, 0.8f, 0.0f};
    vec3 cameraTarget{0.0f, 1.0f, 0.0f};
    vec3 up{0.0f, 1.0f, 0.0f};
    vec3 cameraDirection{vec3::normalize(cameraPosition - cameraTarget)};

    camerathird camera{cameraPosition, cameraTarget, cameraDirection, {0.0f, 0.0f, -1.0f}, vec3::normalize(vec3::cross(up, cameraDirection)), up};

    camerafirst camera2{cameraPosition, cameraTarget, cameraDirection, {0.0f, 0.0f, -1.0f}, vec3::normalize(vec3::cross(up, cameraDirection)), up};

    CAMERA_MODE camMode = CAMERA_MODE::FIRST_PERSON;

    mat4 modelMatrix, rotation, translation, scale;
    mat4 viewMatrix, projectionMatrix;

    mat4::identity(projectionMatrix);
    mat4::getProjection(projectionMatrix, ops::degreesToRadians(45.0f), 1366.0f / 768.0f, 0.1f, 100.0f);

    gltime time;
    input in;

    auto& shader = rd.shaders.get(0);
    auto& weaponShader = rd.shaders.get(1);
    auto& crosshairShader = rd.shaders.get(2);
    auto& cube = rd.cubes.get(0);

    weapon pistol{1, 0, {100.0f, 100.0f}, {0.5f, 0.5f}};
    weapon shotgun{2, 0, {1000.0f, 250.0f}, {700.0f, 500.0f}};

    auto& crosshair = rd.rectangles.get(1);

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

        if (camMode == CAMERA_MODE::FIRST_PERSON) {
            player.update(eng.getWindow(), in, time, camera2.getFront(), camera2.getRight());
            camera2.inputMouse(eng.getWindow());
            camera2.setPosition(player.getPosition() + camera2Offset);
            camera2.update(viewMatrix, time);
        }
        else if (camMode == CAMERA_MODE::THIRD_PERSON) {
            player.update(eng.getWindow(), in, time, camera.getFront(), camera.getRight());
            camera.inputMouse(eng.getWindow());
            camera.setTarget(player.getPosition() + (vec3){0.0f, 0.5f, 0.0f});
            camera.update(viewMatrix, time);
        }

        shader.bind();

        shader.mat4Load(shader.getUniformLocation("view"), viewMatrix);
        shader.mat4Load(shader.getUniformLocation("projection"), projectionMatrix);

        if (camMode == CAMERA_MODE::THIRD_PERSON) {
            player.draw(shader, rd.models, rd.meshes, rd.textures);
        }

        mat4::identity(modelMatrix);
        mat4::translate(modelMatrix, {0.0f, -0.25f, 0.0f});
        mat4::scale(modelMatrix, {20.0f, 0.5f, 20.0f});

        shader.mat4Load(shader.getUniformLocation("model"), modelMatrix);

        cube.draw(shader, rd.textures);

        shader.unbind();

        if (camMode == CAMERA_MODE::FIRST_PERSON) {

            gltools::disable(GL_DEPTH_TEST);

            // drawing crosshair
            crosshairShader.bind();

            crosshairShader.mat4Load(crosshairShader.getUniformLocation("projection"), ortho);

            mat4::identity(modelMatrix);
            mat4::translate(modelMatrix, {1366.0f / 2.0f, 768.0f / 2.0f, 0.0f});
            mat4::scale(modelMatrix, {5.0f, 5.0f, 1.0f});

            crosshairShader.mat4Load(crosshairShader.getUniformLocation("model"), modelMatrix);

            crosshairShader.vec3Load(crosshairShader.getUniformLocation("inColor"), {1.0f, 1.0f, 1.0f});
            crosshair.draw(crosshairShader);
            crosshairShader.vec3Load(crosshairShader.getUniformLocation("inColor"), {0.0f, 0.0f, 0.0f});
            crosshair.drawWireFrame(crosshairShader);

            crosshairShader.unbind();

            // drawing weapon
            weaponShader.bind();

            weaponShader.mat4Load(weaponShader.getUniformLocation("projection"), ortho);

            mat4::identity(modelMatrix);
            mat4::translate(modelMatrix, {shotgun.position.x, shotgun.position.y, 0.0f});
            mat4::scale(modelMatrix, {shotgun.scale.x, shotgun.scale.y, 1.0f});

            weaponShader.mat4Load(weaponShader.getUniformLocation("model"), modelMatrix);

            shotgun.draw(weaponShader, rd.rectangles, rd.textures);

            weaponShader.unbind();

            gltools::enable(GL_DEPTH_TEST);
        }

        // Text rendering
        cF.draw(rd.fonts, rd.textures, rd.shaders, ortho);
        cT.draw(rd.fonts, rd.textures, rd.shaders, ortho);

        gltools::swapBuffers(eng.getWindow());
        gltools::pollEvents();
    }
}
