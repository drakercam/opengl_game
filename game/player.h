#ifndef PLAYER_H
#define PLAYER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <limits>

#include "engine.h"

class player {

public:
    player(vec3 position, size_t modelRef, size_t hitboxRef);

    void update(GLFWwindow* window, input& in, gltime& time, const vec3& cameraFront, const vec3& cameraRight);
    void draw(shader& s, buffer<model>& models, buffer<mesh>& meshes, buffer<texture>& textures);
    void drawBounds(shader& s, cube& cube);

    vec3 getPosition() const;
    size_t getModelRef() const;
    size_t getHitboxRef() const { return hitboxRef; }
    aabb getBounds() const {    // world space hitbox
        return {
            this->bounds.min + this->position,
            this->bounds.max + this->position
        };
    }

private:

    vec3 position;
    vec3 velocity;

    float rotation;
    float speed;
    float scale;

    size_t modelRef;
    size_t hitboxRef;

    aabb bounds;
    bool grounded;

};

#endif
