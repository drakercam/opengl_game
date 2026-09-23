#include "player.h"
#include "glshader.h"

player::player(vec3 position, size_t modelRef, size_t hitboxRef) {
    this->position = position;
    this->velocity = {0.0f, 0.0f, 0.0f};
    this->modelRef = modelRef;
    this->hitboxRef = hitboxRef;
    this->rotation = 0.0f;
    this->speed = 4.0f;
    this->scale = 0.5f;

    this->bounds = {
        {-0.15f, 0.0f, -0.15f},
        { 0.15f, 0.65f,  0.15f}
    };

    this->shootRay.origin = position + vec3{0.0f, 0.8f, 0.0f};
    this->shootRay.direction = {0.0f, 0.0f, -1.0f};

    this->colliding = false;
}

void player::update(GLFWwindow* window, input& in, gltime& time, const vec3& cameraFront, const vec3& cameraRight) {

    float delta = time.getDelta();

    vec3 movement{0.0f, 0.0f, 0.0f};

    vec3 forward = cameraFront;
    forward.y = 0.0f;
    forward = vec3::normalize(forward);

    vec3 right = cameraRight;
    right.y = 0.0f;
    right = vec3::normalize(right);

    if (in.isKeyPressed(window, GLFW_KEY_W)) movement += forward;
    if (in.isKeyPressed(window, GLFW_KEY_S)) movement -= forward;
    if (in.isKeyPressed(window, GLFW_KEY_A)) movement -= right;
    if (in.isKeyPressed(window, GLFW_KEY_D)) movement += right;

    /* * Prevent diagonal movement from being faster. */
    if (movement.x != 0.0f || movement.z != 0.0f) movement = vec3::normalize(movement);

    this->position += movement * this->speed * time.getDelta();

    // Gravity.
    const float gravity = -9.81f;
    this->velocity.y += gravity * delta;

    // Jump.
    if (in.isKeyPressed(window, GLFW_KEY_SPACE) &&
        this->position.y <= 0.0f) {

        const float jumpVelocity = 5.0f;
        this->velocity.y = jumpVelocity;
    }

    // Vertical movement.
    this->position.y += this->velocity.y * delta;

    // Floor collision.
    if (this->position.y < 0.0f) {
        this->position.y = 0.0f;
        this->velocity.y = 0.0f;
    }

    this->shootRay.origin = position + vec3{0.0f, 0.8f, 0.0f};
    this->shootRay.direction = vec3::normalize(cameraFront);
}

void player::draw(const shader& s, std::vector<model>& models, std::vector<mesh>& meshes, std::vector<texture>& textures) {
    mat4 modelMatrix;
    mat4::identity(modelMatrix);
    mat4::translate(modelMatrix, this->position);
    mat4::rotate(modelMatrix, ops::degreesToRadians(this->rotation), {0.0f, 1.0f, 0.0f});
    mat4::scale(modelMatrix, {this->scale, this->scale, this->scale});
    shaderMat4Load(s, shaderGetUniformLocation(s, "model"), modelMatrix);

    modelDraw(models.at(this->modelRef), s, std::span(meshes), std::span(textures));
}

void player::drawBounds(shader& s, mesh& cube) {
    shaderBind(s);
    aabb box = this->getBounds();

    vec3 center = (box.min + box.max) * 0.5f;
    vec3 size = box.max - box.min;

    mat4 modelMatrix;
    mat4::identity(modelMatrix);
    mat4::translate(modelMatrix, center);
    mat4::scale(modelMatrix, size);

    shaderMat4Load(s, shaderGetUniformLocation(s, "model"), modelMatrix);
    shaderVec3Load(s, shaderGetUniformLocation(s, "inColor"), {1.0f, 0.0f, 0.0f});

    meshDrawWireFrame(cube);
    shaderUnbind();
}

vec3 player::getPosition() const {
    return this->position;
}

size_t player::getModelRef() const {
    return this->modelRef;
}

enemy::enemy(vec3 position, size_t sphereRef, size_t hitboxRef) {
    this->position = position;
    this->velocity = {0.0f, 0.0f, 0.0f};
    this->sphereRef = sphereRef;
    this->hitboxRef = hitboxRef;
    this->rotation = 0.0f;
    this->speed = 4.0f;
    this->scale = 1.0f;

    this->bounds = {
        {-0.5f, -0.5f, -0.5f},
        { 0.5f, 0.5f,  0.5f}
    };
    this->hit = false;
}

void enemy::draw(shader& s, std::vector<mesh>& spheres) {
    auto& sphere = spheres.at(this->sphereRef);

    shaderBind(s);

    // draw enemy
    mat4 modelMatrix;
    mat4::identity(modelMatrix);
    mat4::translate(modelMatrix, this->position);
    mat4::scale(modelMatrix, {this->scale, this->scale, this->scale});

    shaderMat4Load(s, shaderGetUniformLocation(s, "model"), modelMatrix);

    meshDraw(sphere);
    shaderUnbind();
}

void enemy::drawBounds(shader& s, mesh& cube) {
    shaderBind(s);
    aabb box = this->getBounds();

    vec3 center = (box.min + box.max) * 0.5f;
    vec3 size = box.max - box.min;

    mat4 modelMatrix;
    mat4::identity(modelMatrix);
    mat4::translate(modelMatrix, center);
    mat4::scale(modelMatrix, size);

    shaderMat4Load(s, shaderGetUniformLocation(s, "model"), modelMatrix);
    shaderVec3Load(s, shaderGetUniformLocation(s, "inColor"), {1.0f, 0.0f, 0.0f});

    meshDrawWireFrame(cube);
    shaderUnbind();
}

vec3 enemy::getPosition() const {
    return this->position;
}

size_t enemy::getSphereRef() const {
    return this->sphereRef;
}
