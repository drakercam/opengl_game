#include "project.h"
#include "glmesh.h"
#include "glshader.h"
#include "gltexture.h"
#include "gltools.h"
#include "text.h"
#include <print>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include <vector>
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

    rd.shaders.reserve(20);
    rd.textures.reserve(150);
    rd.fonts.reserve(20);
    rd.texts.reserve(20);

    texture stoneBrick, pistol, shotgun, skybox, dirt;
    textureLoad(stoneBrick, "../resources/stone-brick.jpg");
    textureLoad(pistol, "../resources/pistol1.png");
    textureLoad(shotgun, "../resources/shotgun1.png");
    textureLoad(skybox, "../resources/skybox2.jpg");
    textureLoad(dirt, "../resources/dirt.jpg");

    rd.textures.push_back(stoneBrick);
    rd.textures.push_back(pistol);
    rd.textures.push_back(shotgun);
    rd.textures.push_back(skybox);
    rd.textures.push_back(dirt);

    rd.meshes.reserve(100);

    shader basicShader1, weaponShader, basicShader2, textShader, basicShader3;
    shaderLoad(basicShader1,
               file::read("../shaders/basic_vertex1.glsl").c_str(),
               file::read("../shaders/basic_frag1.glsl").c_str()
    );
    shaderLoad(weaponShader,
               file::read("../shaders/weapon_vertex.glsl").c_str(),
               file::read("../shaders/weapon_frag.glsl").c_str()
    );
    shaderLoad(basicShader2,
               file::read("../shaders/basic_vertex2.glsl").c_str(),
               file::read("../shaders/basic_frag2.glsl").c_str()
    );
    shaderLoad(textShader,
               file::read("../shaders/text_vertex.glsl").c_str(),
               file::read("../shaders/text_frag.glsl").c_str()
    );
    shaderLoad(basicShader3,
               file::read("../shaders/basic_vertex3.glsl").c_str(),
               file::read("../shaders/basic_frag3.glsl").c_str()
    );

    rd.shaders.push_back(basicShader1);
    rd.shaders.push_back(weaponShader);
    rd.shaders.push_back(basicShader2);
    rd.shaders.push_back(textShader);
    rd.shaders.push_back(basicShader3);

    mesh floorMesh, playerHitboxMesh, enemyHitboxMesh, quadMesh, crosshairMesh, skyboxMesh, enemyMesh;
    meshLoad(floorMesh, cubeMake());
    meshLoad(playerHitboxMesh, cubeMake());
    meshLoad(enemyHitboxMesh, cubeMake());
    meshLoad(quadMesh, rectangleMake());
    meshLoad(crosshairMesh, circleMake());
    meshLoad(skyboxMesh, sphereMake());
    meshLoad(enemyMesh, sphereMake());

    rd.meshes.push_back(floorMesh);
    rd.meshes.push_back(playerHitboxMesh);
    rd.meshes.push_back(enemyHitboxMesh);
    rd.meshes.push_back(quadMesh);
    rd.meshes.push_back(crosshairMesh);
    rd.meshes.push_back(skyboxMesh);
    rd.meshes.push_back(enemyMesh);

    model penguinModel;
    modelLoad(penguinModel, "../models/penguin/PenguinBaseMesh.obj", rd.meshes, rd.textures);
    rd.models.push_back(penguinModel);

    auto& shader = rd.shaders.at(0);
    shaderBind(shader);
    shaderIntLoad(shader, shaderGetUniformLocation(shader, "materialTexture"), 0);
    shaderUnbind();

    auto& weaponshader = rd.shaders.at(1);
    shaderBind(weaponshader);
    shaderIntLoad(weaponshader, shaderGetUniformLocation(weaponshader, "tex"), 1);
    shaderUnbind();

    font yellowBanana;
    fontLoad(yellowBanana, "../resources/Yellow Banana.otf", 3, eng.getFTLibrary(), rd.textures);
    rd.fonts.push_back(yellowBanana);

    // create texts
    text fpcText, tpcText, shotgunText, pistolText;
    textLoad(fpcText,"Press F to switch to first person camera", vec2{50.0f, 680.0f}, 0.5f, 0);
    textLoad(tpcText, "Press T to switch to third person camera", vec2{50.0f, 650.0f}, 0.5f, 0);
    textLoad(shotgunText, "Press 1 to switch to shotgun", vec2{50.0f, 620.0f}, 0.5f, 0);
    textLoad(pistolText, "Press 2 to switch to pistol", vec2{50.0f, 590.0f}, 0.5f, 0);

    rd.texts.push_back(fpcText);
    rd.texts.push_back(tpcText);
    rd.texts.push_back(shotgunText);
    rd.texts.push_back(pistolText);
}

void game::terminate() {

    rd.clear();

    eng.terminate();
}

void game::update(gltime& time) {

    return;
}

void game::run() {
    vec3 playerPosition{0.0f, 0.0f, 0.0f};
    vec3 enemyPosition{2.0f, 0.5f, 2.0f};
    float playerX{0.0f};
    float playerY{0.0f};

    vec3 cameraPosition{0.0f, 0.75f, -5.0f};
    float camera2Offset{1.5f};
    vec3 cameraTarget{0.0f, playerPosition.y + camera2Offset, 0.0f};
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

    player player{playerPosition, 0, 1};
    enemy enemy{enemyPosition, 6, 2};

    auto& shader = rd.shaders.at(0);
    auto& weaponShader = rd.shaders.at(1);
    auto& crosshairShader = rd.shaders.at(2);
    auto& staticShader = rd.shaders.at(4);

    auto& floor = rd.meshes.at(0);
    auto& skybox = rd.meshes.at(5);
    auto& playerHitbox = rd.meshes.at(1);
    auto& enemyHitbox = rd.meshes.at(2);

    weapon pistol{1, 0, {1000.0f, 250.0f}, {732.0f, 500.0f}};
    weapon shotgun{2, 0, {1000.0f, 250.0f}, {732.0f, 500.0f}};

    WEAPON_CHOICE weapon = WEAPON_CHOICE::SHOTGUN;

    auto& crosshair = rd.meshes.at(4);

    // for text rendering
    mat4 ortho;
    mat4::identity(ortho);
    mat4::getOrthographic(ortho, 0.0f, 1366.0f, 0.0f, 768.0f, -1.0f, 1.0f);

    auto& cF = rd.texts.at(0);
    auto& cT = rd.texts.at(1);
    auto& w1 = rd.texts.at(2);
    auto& w2 = rd.texts.at(3);

    size_t skyboxTexture[] = {3};
    size_t floorTexture[] = {0};

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
            camera2.setPosition({player.getPosition().x, player.getPosition().y + camera2Offset, player.getPosition().z});
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

        shaderBind(shader);

        shaderMat4Load(shader, shaderGetUniformLocation(shader, "view"), viewMatrix);
        shaderMat4Load(shader, shaderGetUniformLocation(shader, "projection"), projectionMatrix);

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

        shaderMat4Load(shader, shaderGetUniformLocation(shader, "model"), modelMatrix);

        shaderSetTextures(shader, rd.textures, std::span(skyboxTexture));
        meshDraw(skybox);

        gltools::enable(GL_CULL_FACE);

        if (camMode == CAMERA_MODE::THIRD_PERSON) {
            player.draw(shader, rd.models, rd.meshes, rd.textures);
        }

        mat4::identity(modelMatrix);
        mat4::translate(modelMatrix, {0.0f, -0.5f, 0.0f});
        mat4::scale(modelMatrix, {20.0f, 1.0f, 20.0f});

        shaderMat4Load(shader, shaderGetUniformLocation(shader, "model"), modelMatrix);

        shaderSetTextures(shader, rd.textures, std::span(floorTexture));
        meshDraw(floor);

        shaderUnbind();

        // draw enemy
        shaderBind(staticShader);

        shaderMat4Load(staticShader, shaderGetUniformLocation(staticShader, "view"), viewMatrix);
        shaderMat4Load(staticShader, shaderGetUniformLocation(staticShader, "projection"), projectionMatrix);

        // if the enemy is hit, turn it red, else its green
        if (enemy.isHit()) {
            shaderVec3Load(staticShader, shaderGetUniformLocation(staticShader, "inColor"), {1.0f, 0.0f, 0.0f});
        }
        else {
            shaderVec3Load(staticShader, shaderGetUniformLocation(staticShader, "inColor"), {0.0f, 1.0f, 0.0f});
        }

        enemy.draw(staticShader, rd.meshes);

        shaderUnbind();

        // draw bounding boxes
        if (camMode == CAMERA_MODE::THIRD_PERSON) {

            shaderBind(staticShader);

            shaderMat4Load(staticShader, shaderGetUniformLocation(staticShader, "view"), viewMatrix);
            shaderMat4Load(staticShader, shaderGetUniformLocation(staticShader, "projection"), projectionMatrix);

            player.drawBounds(staticShader, playerHitbox);
            enemy.drawBounds(staticShader, enemyHitbox);

            shaderUnbind();
        }

        if (camMode == CAMERA_MODE::FIRST_PERSON) {

            gltools::disable(GL_DEPTH_TEST);

            // drawing crosshair
            shaderBind(crosshairShader);

            shaderMat4Load(crosshairShader, shaderGetUniformLocation(crosshairShader, "projection"), ortho);

            mat4::identity(modelMatrix);
            mat4::translate(modelMatrix, {1366.0f / 2.0f, 768.0f / 2.0f, 0.0f});
            mat4::scale(modelMatrix, {12.0f, 12.0f, 0.0f});

            shaderMat4Load(crosshairShader, shaderGetUniformLocation(crosshairShader, "model"), modelMatrix);

            shaderVec3Load(crosshairShader, shaderGetUniformLocation(crosshairShader, "inColor"), {1.0f, 1.0f, 1.0f});
            meshDraw(crosshair);

            shaderUnbind();

            // drawing weapon
           if (weapon == WEAPON_CHOICE::SHOTGUN) {

               shaderBind(weaponShader);

               shaderMat4Load(weaponShader, shaderGetUniformLocation(weaponShader, "projection"), ortho);

               mat4::identity(modelMatrix);
               mat4::translate(modelMatrix, {shotgun.position.x, shotgun.position.y, 0.0f});
               mat4::scale(modelMatrix, {shotgun.scale.x, shotgun.scale.y, 1.0f});

               shaderMat4Load(weaponShader, shaderGetUniformLocation(weaponShader, "model"), modelMatrix);

               shotgun.draw(weaponShader, rd.meshes, rd.textures);

               shaderUnbind();

               gltools::enable(GL_DEPTH_TEST);

            } else if (weapon == WEAPON_CHOICE::PISTOL) {

                shaderBind(weaponShader);

                shaderMat4Load(weaponShader, shaderGetUniformLocation(weaponShader, "projection"), ortho);

                mat4::identity(modelMatrix);
                mat4::translate(modelMatrix, {pistol.position.x, pistol.position.y, 0.0f});
                mat4::scale(modelMatrix, {pistol.scale.x, pistol.scale.y, 1.0f});

                shaderMat4Load(weaponShader, shaderGetUniformLocation(weaponShader, "model"), modelMatrix);

                pistol.draw(weaponShader, rd.meshes, rd.textures);

                shaderUnbind();

                gltools::enable(GL_DEPTH_TEST);
            }
        }

        // Text rendering
        textDraw(cF, std::span(rd.fonts), std::span(rd.textures), std::span(rd.shaders), ortho);
        textDraw(cT, std::span(rd.fonts), std::span(rd.textures), std::span(rd.shaders), ortho);
        textDraw(w1, std::span(rd.fonts), std::span(rd.textures), std::span(rd.shaders), ortho);
        textDraw(w2, std::span(rd.fonts), std::span(rd.textures), std::span(rd.shaders), ortho);

        gltools::swapBuffers(eng.getWindow());
        gltools::pollEvents();
    }
}
