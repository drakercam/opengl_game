#include "enemy.h"

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
