#include "project.h"
#include "gltexture.h"
#include "gltools.h"
#include <print>
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

enum WEAPON_CHOICE {
    PISTOL,
    SHOTGUN
};

void game::init() {

    eng.initialize();

    rd.shaders.getElements().reserve(20);
    rd.textures.getElements().reserve(150);
    rd.rectangles.getElements().reserve(20);
    rd.circles.getElements().reserve(20);
    rd.spheres.getElements().reserve(100);
    rd.cubes.getElements().reserve(100);
    rd.fonts.getElements().reserve(20);
    rd.texts.getElements().reserve(20);

    rd.textures.emplace("../resources/stone-brick.jpg");
    rd.textures.emplace("../resources/pistol1.png");
    rd.textures.emplace("../resources/shotgun1.png");
    rd.textures.emplace("../resources/skybox2.jpg");
    rd.textures.emplace("../resources/dirt.jpg");

    rd.meshes.getElements().reserve(100);

    rd.shaders.emplace(file::read("../shaders/basic_vertex1.glsl").c_str(), file::read("../shaders/basic_frag1.glsl").c_str());
    rd.shaders.emplace(file::read("../shaders/weapon_vertex.glsl").c_str(), file::read("../shaders/weapon_frag.glsl").c_str());
    rd.shaders.emplace(file::read("../shaders/basic_vertex2.glsl").c_str(), file::read("../shaders/basic_frag2.glsl").c_str());
    rd.shaders.emplace(file::read("../shaders/text_vertex.glsl").c_str(), file::read("../shaders/text_frag.glsl").c_str());
    rd.shaders.emplace(file::read("../shaders/basic_vertex3.glsl").c_str(), file::read("../shaders/basic_frag3.glsl").c_str());

    rd.models.emplace("../models/penguin/PenguinBaseMesh.obj", rd.meshes, rd.textures);

    rd.cubes.emplace(std::vector<size_t>{0});
    rd.cubes.emplace(std::vector<size_t>{0});    // player hitbox
    rd.cubes.emplace(std::vector<size_t>{0});    // enemy hitbox
    rd.rectangles.emplace(std::vector<size_t>{1});
    rd.circles.emplace(std::vector<size_t>{}, 128);    // crosshair
    rd.spheres.emplace(std::vector<size_t>{3}); // skybox
    rd.spheres.emplace(std::vector<size_t>{0}); // enemy placeholder

    auto& shader = rd.shaders.get(0);
    shader.bind();
    shader.intLoad(shader.getUniformLocation("materialTexture"), 0);
    shader.unbind();

    auto& weaponshader = rd.shaders.get(1);
    weaponshader.bind();
    weaponshader.intLoad(weaponshader.getUniformLocation("tex"), 1);
    weaponshader.unbind();

    rd.fonts.emplace("../resources/Yellow Banana.otf", 3, eng.getFTLibrary(), rd.textures);

    // create texts
    rd.texts.emplace("Press F to switch to first person camera", vec2{50.0f, 680.0f}, 0.5f, 0);
    rd.texts.emplace("Press T to switch to third person camera", vec2{50.0f, 650.0f}, 0.5f, 0);
    rd.texts.emplace("Press 1 to switch to shotgun", vec2{50.0f, 620.0f}, 0.5f, 0);
    rd.texts.emplace("Press 2 to switch to pistol", vec2{50.0f, 590.0f}, 0.5f, 0);
}

void game::terminate() {

    rd.clear();

    eng.terminate();
}

void game::update(gltime& time) {

    return;
}

void game::run() {
    vec3 cameraPosition{0.0f, 0.0f, 2.0f};
    vec3 camera2Offset{0.0f, 0.8f, 0.0f};
    vec3 cameraTarget{0.0f, 0.5f, 0.0f};
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

    player player{{0.0f, 0.0f, 0.0f}, 0, 1};
    enemy enemy{{2.0f, 0.5f, 2.0f}, 1, 2};

    auto& shader = rd.shaders.get(0);
    auto& weaponShader = rd.shaders.get(1);
    auto& crosshairShader = rd.shaders.get(2);
    auto& staticShader = rd.shaders.get(4);
    auto& floor = rd.cubes.get(0);
    auto& skybox = rd.spheres.get(0);
    auto& playerHitbox = rd.cubes.get(1);
    auto& enemyHitbox = rd.cubes.get(2);

    weapon pistol{1, 0, {1000.0f, 250.0f}, {732.0f, 500.0f}};
    weapon shotgun{2, 0, {1000.0f, 250.0f}, {732.0f, 500.0f}};

    WEAPON_CHOICE weapon = WEAPON_CHOICE::SHOTGUN;

    auto& crosshair = rd.circles.get(0);

    // for text rendering
    mat4 ortho;
    mat4::identity(ortho);
    mat4::getOrthographic(ortho, 0.0f, 1366.0f, 0.0f, 768.0f, -1.0f, 1.0f);

    auto& cF = rd.texts.get(0);
    auto& cT = rd.texts.get(1);
    auto& w1 = rd.texts.get(2);
    auto& w2 = rd.texts.get(3);

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

        // WEAPON CHOICE
        if (in.isKeyPressed(eng.getWindow(), GLFW_KEY_1)) {
            weapon = WEAPON_CHOICE::SHOTGUN;
        }
        else if (in.isKeyPressed(eng.getWindow(), GLFW_KEY_2)) {
            weapon = WEAPON_CHOICE::PISTOL;
        }

        // check if player is colliding with enemy + camera switching
        if (camMode == CAMERA_MODE::FIRST_PERSON) {

            vec3 oldPlayerPosition = player.getPosition();
            player.update(eng.getWindow(), in, time, camera2.getFront(), camera2.getRight());
            if (aabb::isColliding(player.getBounds(), enemy.getBounds())) {
                player.setPosition(oldPlayerPosition);
            }

            camera2.inputMouse(eng.getWindow());
            camera2.setPosition(player.getPosition() + camera2Offset);
            camera2.update(viewMatrix, time);
        }
        else if (camMode == CAMERA_MODE::THIRD_PERSON) {

            vec3 oldPlayerPosition = player.getPosition();
            player.update(eng.getWindow(), in, time, camera.getFront(), camera.getRight());
            if (aabb::isColliding(player.getBounds(), enemy.getBounds())) {
                player.setPosition(oldPlayerPosition);
            }

            camera.inputMouse(eng.getWindow());
            camera.setTarget(player.getPosition() + (vec3){0.0f, 0.5f, 0.0f});
            camera.update(viewMatrix, time);
        }

        // shooting if C is pressed and the enemy has not been tagged
        if (in.isKeyPressed(eng.getWindow(), GLFW_KEY_C) && !enemy.isHit()) {
            float hitDistancePlayerEnemy;
            if (ray::aabbIntersect(player.getRay(), enemy.getBounds(), hitDistancePlayerEnemy)) {
                std::cout << "enemy hit" << std::endl;
                enemy.setHit(true);
            }
        }

        shader.bind();

        shader.mat4Load(shader.getUniformLocation("view"), viewMatrix);
        shader.mat4Load(shader.getUniformLocation("projection"), projectionMatrix);

        gltools::disable(GL_CULL_FACE);

        if (camMode == CAMERA_MODE::FIRST_PERSON) {
            mat4::identity(modelMatrix);
            mat4::translate(modelMatrix, camera2.getPosition());
            mat4::scale(modelMatrix, {150.0f, 150.0f, 150.0f});
        }
        else if (camMode == CAMERA_MODE::THIRD_PERSON) {
            mat4::identity(modelMatrix);
            mat4::translate(modelMatrix, camera.getPosition());
            mat4::scale(modelMatrix, {150.0f, 150.0f, 150.0f});
        }

        shader.mat4Load(shader.getUniformLocation("model"), modelMatrix);

        shader.setTextures(rd.textures, skybox.getTextureRefs());
        skybox.draw(shader);

        gltools::enable(GL_CULL_FACE);

        if (camMode == CAMERA_MODE::THIRD_PERSON) {
            player.draw(shader, rd.models, rd.meshes, rd.textures);
        }

        mat4::identity(modelMatrix);
        mat4::translate(modelMatrix, {0.0f, -0.5f, 0.0f});
        mat4::scale(modelMatrix, {20.0f, 1.0f, 20.0f});

        shader.mat4Load(shader.getUniformLocation("model"), modelMatrix);

        shader.setTextures(rd.textures, floor.getTextureRefs());
        floor.draw(shader);

        shader.unbind();

        // draw enemy
        staticShader.bind();

        staticShader.mat4Load(staticShader.getUniformLocation("view"), viewMatrix);
        staticShader.mat4Load(staticShader.getUniformLocation("projection"), projectionMatrix);

        // if the enemy is hit, turn it red, else its green
        if (enemy.isHit()) {
            staticShader.vec3Load(staticShader.getUniformLocation("inColor"), {1.0f, 0.0f, 0.0f});
        }
        else {
            staticShader.vec3Load(staticShader.getUniformLocation("inColor"), {0.0f, 1.0f, 0.0f});
        }

        enemy.draw(staticShader, rd.spheres);

        staticShader.unbind();

        // draw bounding boxes
        if (camMode == CAMERA_MODE::THIRD_PERSON) {

            staticShader.bind();

            staticShader.mat4Load(staticShader.getUniformLocation("view"), viewMatrix);
            staticShader.mat4Load(staticShader.getUniformLocation("projection"), projectionMatrix);

            player.drawBounds(staticShader, playerHitbox);
            enemy.drawBounds(staticShader, enemyHitbox);

            staticShader.unbind();
        }

        if (camMode == CAMERA_MODE::FIRST_PERSON) {

            gltools::disable(GL_DEPTH_TEST);

            // drawing crosshair
            crosshairShader.bind();

            crosshairShader.mat4Load(crosshairShader.getUniformLocation("projection"), ortho);

            mat4::identity(modelMatrix);
            mat4::translate(modelMatrix, {1366.0f / 2.0f, 768.0f / 2.0f, 0.0f});
            mat4::scale(modelMatrix, {12.0f, 12.0f, 0.0f});

            crosshairShader.mat4Load(crosshairShader.getUniformLocation("model"), modelMatrix);

            crosshairShader.vec3Load(crosshairShader.getUniformLocation("inColor"), {1.0f, 1.0f, 1.0f});
            crosshair.draw(crosshairShader);

            crosshairShader.unbind();

            // drawing weapon
           if (weapon == WEAPON_CHOICE::SHOTGUN) {
               weaponShader.bind();

               weaponShader.mat4Load(weaponShader.getUniformLocation("projection"), ortho);

               mat4::identity(modelMatrix);
               mat4::translate(modelMatrix, {shotgun.position.x, shotgun.position.y, 0.0f});
               mat4::scale(modelMatrix, {shotgun.scale.x, shotgun.scale.y, 1.0f});

               weaponShader.mat4Load(weaponShader.getUniformLocation("model"), modelMatrix);

               shotgun.draw(weaponShader, rd.rectangles, rd.textures);

               weaponShader.unbind();

               gltools::enable(GL_DEPTH_TEST);

            } else if (weapon == WEAPON_CHOICE::PISTOL) {
                weaponShader.bind();

                weaponShader.mat4Load(weaponShader.getUniformLocation("projection"), ortho);

                mat4::identity(modelMatrix);
                mat4::translate(modelMatrix, {pistol.position.x, pistol.position.y, 0.0f});
                mat4::scale(modelMatrix, {pistol.scale.x, pistol.scale.y, 1.0f});

                weaponShader.mat4Load(weaponShader.getUniformLocation("model"), modelMatrix);

                pistol.draw(weaponShader, rd.rectangles, rd.textures);

                weaponShader.unbind();

                gltools::enable(GL_DEPTH_TEST);
            }
        }

        // Text rendering
        cF.draw(rd.fonts, rd.textures, rd.shaders, ortho);
        cT.draw(rd.fonts, rd.textures, rd.shaders, ortho);
        w1.draw(rd.fonts, rd.textures, rd.shaders, ortho);
        w2.draw(rd.fonts, rd.textures, rd.shaders, ortho);

        gltools::swapBuffers(eng.getWindow());
        gltools::pollEvents();
    }
}
