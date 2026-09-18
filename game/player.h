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
    void setPosition(const vec3& p) { this->position = p; }
    size_t getModelRef() const;
    size_t getHitboxRef() const { return hitboxRef; }
    aabb getBounds() const {    // world space hitbox
        return {
            this->bounds.min + this->position,
            this->bounds.max + this->position
        };
    }
    const ray& getRay() const { return shootRay; }

    void setCollision(const bool c) { this->colliding = c; }
    bool isColliding(void) const { return this->colliding; }

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

    bool colliding;

    // ray for firing
    ray shootRay;
};

class enemy {

public:
    enemy(vec3 position, size_t sphereRef, size_t hitboxRef);

    //void update(GLFWwindow* window, input& in, gltime& time, const vec3& cameraFront, const vec3& cameraRight);
    void draw(shader& s, buffer<sphere>& spheres);
    void drawBounds(shader& s, cube& cube);

    vec3 getPosition() const;
    size_t getSphereRef() const;
    size_t getHitboxRef() const { return hitboxRef; }
    aabb getBounds() const {    // world space hitbox
        return {
            this->bounds.min + this->position,
            this->bounds.max + this->position
        };
    }
    bool isHit(void) const { return hit; }
    void setHit(const bool h) { this->hit = h; }

private:

    vec3 position;
    vec3 velocity;

    float rotation;
    float speed;
    float scale;

    size_t sphereRef;
    size_t hitboxRef;

    aabb bounds;

    bool hit;
};

#endif
