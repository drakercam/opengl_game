#ifndef GLCAMERATHIRD_H
#define GLCAMERATHIRD_H

#ifndef GLCAMERA_H
#define GLCAMERA_H

#include "glmath.h"
#include "gltime.h"
#include "input.h"
#include <GLFW/glfw3.h>

class camerathird {

public:
    camerathird(const vec3 pos, const vec3 target, const vec3 dir, const vec3 front, const vec3 right, const vec3 up) {
        this->position = pos;
        this->target = target;
        this->direction = dir;
        this->front = front;
        this->right = right;
        this->up = up;
        this->worldUp = {0.0f, 1.0f, 0.0f};
        this->yaw = 0.0f;
        this->pitch = 0.0f;
        this->speed = 0.05f;
        this->rotationSpeed = 1.5f;
        this->mouseSensitivity = 0.003f;

        this->distance = 5.0f;
    }

    void getMouseCursorPos(GLFWwindow* window, double& mX, double& mY);
    void update(mat4& view, gltime t);
    void inputMouse(GLFWwindow* window, GLboolean constrainPitch = true);
    vec3 getPosition(void) const { return position; }
    void setTarget(const vec3& target) {
        this->target = target;
    }

    void setTarget(const vec3&& target) {
        this->target = target;
    }

    vec3 getFront() const { return front; }
    vec3 getRight() const { return right; }

private:
    vec3 position;
    vec3 target;    // target position
    float distance; // distance to target
    vec3 direction;
    vec3 front;
    vec3 right;
    vec3 up;
    vec3 worldUp;

    float yaw;
    float pitch;
    float speed;
    float rotationSpeed;

    float mouseSensitivity;
    bool firstMouse;
    double lastMouseX;
    double lastMouseY;

};

#endif


#endif
