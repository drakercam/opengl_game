#include "player.h"

player::player(vec3 position, size_t modelRef, size_t hitboxRef) {
    this->position = position;
    this->velocity = {0.0f, 0.0f, 0.0f};
    this->modelRef = modelRef;
    this->hitboxRef = hitboxRef;
    this->rotation = 0.0f;
    this->speed = 4.0f;
    this->scale = 0.5f;

    this->bounds = {
        {-0.25f, 0.0f, -0.25f},
        { 0.25f, 1.0f,  0.25f}
    };
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
}

void player::draw(shader& s, buffer<model>& models, buffer<mesh>& meshes, buffer<texture>& textures) {
    mat4 modelMatrix;
    mat4::identity(modelMatrix);
    mat4::translate(modelMatrix, this->position);
    mat4::rotate(modelMatrix, ops::degreesToRadians(this->rotation), {0.0f, 1.0f, 0.0f});
    mat4::scale(modelMatrix, {this->scale, this->scale, this->scale});
    s.mat4Load(s.getUniformLocation("model"), modelMatrix);

    models.get(this->modelRef).draw(s, meshes, textures);
}

void player::drawBounds(shader& s, cube& cube) {
    aabb box = this->getBounds();

    vec3 center = (box.min + box.max) * 0.5f;
    vec3 size = box.max - box.min;

    mat4 modelMatrix;
    mat4::identity(modelMatrix);
    mat4::translate(modelMatrix, center);
    mat4::scale(modelMatrix, size);

    s.mat4Load(s.getUniformLocation("model"), modelMatrix);
    s.vec3Load(s.getUniformLocation("inColor"), {1.0f, 0.0f, 0.0f});

    cube.drawWireFrame(s);
}

vec3 player::getPosition() const {
    return this->position;
}

size_t player::getModelRef() const {
    return this->modelRef;
}
