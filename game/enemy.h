#ifndef ENEMY_H
#define ENEMY_H

#include <SDL3/SDL.h>
#include "glapi.h"
#include <algorithm>
#include <limits>
#include "engine.h"

class enemy {

public:
    enemy(vec3 position, size_t sphereRef, size_t hitboxRef);

    //void update(GLFWwindow* window, input& in, gltime& time, const vec3& cameraFront, const vec3& cameraRight);
    void draw(shader& s, std::vector<mesh>& spheres);
    void drawBounds(shader& s, mesh& cube);

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
