#ifndef GLCAMERA_H
#define GLCAMERA_H

#include "glmath.h"
#include "gltime.h"
#include <GLFW/glfw3.h>

namespace camera {

    struct data {
        vec3 position;
        vec3 target;
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

        data(const vec3 pos, const vec3 target, const vec3 dir, const vec3 front, const vec3 right, const vec3 up) {
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
        }
    };

    void getMouseCursorPos(GLFWwindow* window, double& mX, double& mY);
    void update(mat4& view, data& cam, gltime::data t);
    void input(GLFWwindow* window, data& cam, gltime::data& t);
    void inputMouse(GLFWwindow* window, data& cam, GLboolean constrainPitch = true);
}

#endif
