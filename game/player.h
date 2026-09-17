#ifndef PLAYER_H
#define PLAYER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine.h"

struct aabb {
    vec3 min;
    vec3 max;
};

class player {

public:
    player(vec3 position, size_t modelRef);

    void update(GLFWwindow* window, input& in, gltime& time, const vec3& cameraFront, const vec3& cameraRight);
    void draw(shader& s, buffer<model>& models, buffer<mesh>& meshes, buffer<texture>& textures);

    vec3 getPosition() const;
    size_t getModelRef() const;

private:

    vec3 position;
    vec3 velocity;

    float rotation;
    float speed;
    float scale;

    size_t modelRef;

    aabb bounds;
    bool grounded;

};

#endif
