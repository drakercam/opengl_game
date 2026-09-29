#include "game.h"

void game::init() {
	eng.initialize();

    renderdata.shaders.reserve(20);
    renderdata.textures.reserve(150);
    renderdata.fonts.reserve(20);
    renderdata.texts.reserve(20);

    texture stoneBrick, pistolTexture, shotgunTexture, skybox, dirt;
    textureLoad(stoneBrick, "../resources/stone-brick.jpg");
    textureLoad(pistolTexture, "../resources/pistol1.png");
    textureLoad(shotgunTexture, "../resources/shotgun1.png");
    textureLoad(skybox, "../resources/skybox2.jpg");
    textureLoad(dirt, "../resources/dirt.jpg");

    renderdata.textures.push_back(stoneBrick);
    renderdata.textures.push_back(pistolTexture);
    renderdata.textures.push_back(shotgunTexture);
    renderdata.textures.push_back(skybox);
    renderdata.textures.push_back(dirt);

    renderdata.meshes.reserve(100);

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

    renderdata.shaders.push_back(basicShader1);
    renderdata.shaders.push_back(weaponShader);
    renderdata.shaders.push_back(basicShader2);
    renderdata.shaders.push_back(textShader);
    renderdata.shaders.push_back(basicShader3);

    mesh floorMesh, playerHitboxMesh, enemyHitboxMesh, quadMesh, crosshairMesh, skyboxMesh, enemyMesh;
    meshLoad(floorMesh, cubeMake());
    meshLoad(playerHitboxMesh, cubeMake());
    meshLoad(enemyHitboxMesh, cubeMake());
    meshLoad(quadMesh, rectangleMake());
    meshLoad(crosshairMesh, circleMake());
    meshLoad(skyboxMesh, sphereMake());
    meshLoad(enemyMesh, sphereMake());

    renderdata.meshes.push_back(floorMesh);
    renderdata.meshes.push_back(playerHitboxMesh);
    renderdata.meshes.push_back(enemyHitboxMesh);
    renderdata.meshes.push_back(quadMesh);
    renderdata.meshes.push_back(crosshairMesh);
    renderdata.meshes.push_back(skyboxMesh);
    renderdata.meshes.push_back(enemyMesh);

    model penguinModel;
    modelLoad(penguinModel, "../models/penguin/PenguinBaseMesh.obj", renderdata.meshes, renderdata.textures);
    renderdata.models.push_back(penguinModel);

    auto& shader = renderdata.shaders.at(0);
    shaderBind(shader);
    shaderIntLoad(shader, shaderGetUniformLocation(shader, "materialTexture"), 0);
    shaderUnbind();

    auto& weaponshader = renderdata.shaders.at(1);
    shaderBind(weaponshader);
    shaderIntLoad(weaponshader, shaderGetUniformLocation(weaponshader, "tex"), 1);
    shaderUnbind();

    font yellowBanana;
    fontLoad(yellowBanana, "../resources/Yellow Banana.otf", 3, eng.getFTLibrary(), renderdata.textures);
    renderdata.fonts.push_back(yellowBanana);

    // create texts
    text fpcText, tpcText, shotgunText, pistolText;
    textLoad(fpcText,"Press F to switch to first person camera", vec2{50.0f, 680.0f}, 0.5f, 0);
    textLoad(tpcText, "Press T to switch to third person camera", vec2{50.0f, 650.0f}, 0.5f, 0);
    textLoad(shotgunText, "Press 1 to switch to shotgun", vec2{50.0f, 620.0f}, 0.5f, 0);
    textLoad(pistolText, "Press 2 to switch to pistol", vec2{50.0f, 590.0f}, 0.5f, 0);

    renderdata.texts.push_back(fpcText);
    renderdata.texts.push_back(tpcText);
    renderdata.texts.push_back(shotgunText);
    renderdata.texts.push_back(pistolText);
	
	// setup projection matrices
    mat4::identity(projectionMatrix);
    mat4::getProjection(projectionMatrix, ops::degreesToRadians(45.0f), 1366.0f / 768.0f, 0.1f, 100.0f);
    
    mat4::identity(orthographicMatrix);
    mat4::getOrthographic(orthographicMatrix, 0.0f, 1366.0f, 0.0f, 768.0f, -1.0f, 1.0f);
}

void game::processInput(const SDL_Event& event) {
	processEvent(event);
}

void game::update() {
	gltimeUpdate(time);
	
	// camera selection
	if (IsKeyPressed(SDL_SCANCODE_F)) {
		camMode = CAMERA_MODE::FIRST_PERSON;
	}
	else if (IsKeyPressed(SDL_SCANCODE_T)) {
		camMode = CAMERA_MODE::THIRD_PERSON;
	}
	
	// Weapon Selection
	if (IsKeyPressed(SDL_SCANCODE_1)) {
		weaponChoice = WEAPON_CHOICE::SHOTGUN;
	}
	else if (IsKeyPressed(SDL_SCANCODE_2)) {
		weaponChoice = WEAPON_CHOICE::PISTOL;
	}
	
	// Player + Camera + Collision Checks/Updates
	if (camMode == CAMERA_MODE::FIRST_PERSON) {

		vec3 oldPlayerPosition = player.getPosition();
		player.update(time, camFirstPerson.getFront(), camFirstPerson.getRight());
		if (aabb::isColliding(player.getBounds(), enemy.getBounds())) {
			player.setPosition(oldPlayerPosition);
		}

		camFirstPerson.inputMouse();
		camFirstPerson.setPosition({player.getPosition().x, player.getPosition().y + cameraOffset, player.getPosition().z});
		camFirstPerson.update(viewMatrix, time);
	}
	else if (camMode == CAMERA_MODE::THIRD_PERSON) {

		vec3 oldPlayerPosition = player.getPosition();
		player.update(time, camThirdPerson.getFront(), camThirdPerson.getRight());
		if (aabb::isColliding(player.getBounds(), enemy.getBounds())) {
			player.setPosition(oldPlayerPosition);
		}

		camThirdPerson.inputMouse();
		camThirdPerson.setTarget(player.getPosition() + (vec3){0.0f, 0.5f, 0.0f});
		camThirdPerson.update(viewMatrix, time);
	}
	
	// Shooting -- if C is pressed and the enemy has not been tagged
	if (IsKeyPressed(SDL_SCANCODE_C) && !enemy.isHit()) {
		float hitDistancePlayerEnemy;
		if (ray::aabbIntersect(player.getRay(), enemy.getBounds(), hitDistancePlayerEnemy)) {
			std::cout << "enemy hit" << std::endl;
			enemy.setHit(true);
		}
	}
}

void game::render() {
	clearColor({0.0f, 0.0f, 0.0, 1.0f});
    clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// shaders
    auto& shader = renderdata.shaders.at(0);
    auto& weaponShader = renderdata.shaders.at(1);
    auto& crosshairShader = renderdata.shaders.at(2);
    auto& staticShader = renderdata.shaders.at(4);
	
	// meshes
    auto& floor = renderdata.meshes.at(0);
    auto& skybox = renderdata.meshes.at(5);
    auto& playerHitbox = renderdata.meshes.at(1);
    auto& enemyHitbox = renderdata.meshes.at(2);
    auto& crosshair = renderdata.meshes.at(4);

    // text
    auto& cF = renderdata.texts.at(0);
    auto& cT = renderdata.texts.at(1);
    auto& w1 = renderdata.texts.at(2);
    auto& w2 = renderdata.texts.at(3);
	
	// textures
    size_t skyboxTexture[] = {3};
    size_t floorTexture[] = {0};
	
	shaderBind(shader);

	shaderMat4Load(shader, shaderGetUniformLocation(shader, "view"), viewMatrix);
	shaderMat4Load(shader, shaderGetUniformLocation(shader, "projection"), projectionMatrix);

	disableGL(GL_CULL_FACE);

	if (camMode == CAMERA_MODE::FIRST_PERSON) {
		mat4::identity(modelMatrix);
		mat4::translate(modelMatrix, camFirstPerson.getPosition());
		mat4::scale(modelMatrix, {150.0f});
	}
	else if (camMode == CAMERA_MODE::THIRD_PERSON) {
		mat4::identity(modelMatrix);
		mat4::translate(modelMatrix, camThirdPerson.getPosition());
		mat4::scale(modelMatrix, {150.0f});
	}

	shaderMat4Load(shader, shaderGetUniformLocation(shader, "model"), modelMatrix);

	shaderSetTextures(shader, std::span(renderdata.textures), skyboxTexture);
	meshDraw(skybox);

	enableGL(GL_CULL_FACE);

	if (camMode == CAMERA_MODE::THIRD_PERSON) {
		player.draw(shader, renderdata.models, renderdata.meshes, renderdata.textures);
	}

	mat4::identity(modelMatrix);
	mat4::translate(modelMatrix, {0.0f, -0.5f, 0.0f});
	mat4::scale(modelMatrix, {20.0f, 1.0f, 20.0f});

	shaderMat4Load(shader, shaderGetUniformLocation(shader, "model"), modelMatrix);

	shaderSetTextures(shader, std::span(renderdata.textures), floorTexture);
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

	enemy.draw(staticShader, renderdata.meshes);

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

		disableGL(GL_DEPTH_TEST);

		// drawing crosshair
		shaderBind(crosshairShader);

		shaderMat4Load(crosshairShader, shaderGetUniformLocation(crosshairShader, "projection"), orthographicMatrix);

		mat4::identity(modelMatrix);
		mat4::translate(modelMatrix, {1366.0f / 2.0f, 768.0f / 2.0f, 0.0f});
		mat4::scale(modelMatrix, {12.0f, 12.0f, 0.0f});

		shaderMat4Load(crosshairShader, shaderGetUniformLocation(crosshairShader, "model"), modelMatrix);

		shaderVec3Load(crosshairShader, shaderGetUniformLocation(crosshairShader, "inColor"), {1.0f});
		meshDraw(crosshair);

		shaderUnbind();

		// drawing weapon
	   if (weaponChoice == WEAPON_CHOICE::SHOTGUN) {

		   shaderBind(weaponShader);

		   shaderMat4Load(weaponShader, shaderGetUniformLocation(weaponShader, "projection"), orthographicMatrix);

		   mat4::identity(modelMatrix);
		   mat4::translate(modelMatrix, {shotgun.position.x, shotgun.position.y, 0.0f});
		   mat4::scale(modelMatrix, {shotgun.scale.x, shotgun.scale.y, 1.0f});

		   shaderMat4Load(weaponShader, shaderGetUniformLocation(weaponShader, "model"), modelMatrix);

		   shotgun.draw(weaponShader, renderdata.meshes, renderdata.textures);

		   shaderUnbind();

		   enableGL(GL_DEPTH_TEST);

		} else if (weaponChoice == WEAPON_CHOICE::PISTOL) {

			shaderBind(weaponShader);

			shaderMat4Load(weaponShader, shaderGetUniformLocation(weaponShader, "projection"), orthographicMatrix);

			mat4::identity(modelMatrix);
			mat4::translate(modelMatrix, {pistol.position.x, pistol.position.y, 0.0f});
			mat4::scale(modelMatrix, {pistol.scale.x, pistol.scale.y, 1.0f});

			shaderMat4Load(weaponShader, shaderGetUniformLocation(weaponShader, "model"), modelMatrix);

			pistol.draw(weaponShader, renderdata.meshes, renderdata.textures);

			shaderUnbind();

			enableGL(GL_DEPTH_TEST);
		}
	}

	// Text rendering
	textDraw(cF, std::span(renderdata.fonts), std::span(renderdata.textures), std::span(renderdata.shaders), orthographicMatrix);
	textDraw(cT, std::span(renderdata.fonts), std::span(renderdata.textures), std::span(renderdata.shaders), orthographicMatrix);
	textDraw(w1, std::span(renderdata.fonts), std::span(renderdata.textures), std::span(renderdata.shaders), orthographicMatrix);
	textDraw(w2, std::span(renderdata.fonts), std::span(renderdata.textures), std::span(renderdata.shaders), orthographicMatrix);

	swapBuffers(eng.getWindow());
}

void game::terminate() {
	renderdata.clear();

    eng.terminate();
}
