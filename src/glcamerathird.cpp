#include "glcamerathird.h"
#include <GLFW/glfw3.h>

void camerathird::getMouseCursorPos(GLFWwindow* window, double& mX, double& mY) {

    glfwGetCursorPos(window, &mX, &mY);
}

void camerathird::update(mat4& view, gltime t) {

    // looking
    this->front.x = cos(this->pitch) *  sin(this->yaw);
    this->front.y = sin(this->pitch);
    this->front.z = -cos(this->pitch) * cos(this->yaw);

    this->front = vec3::normalize(this->front);

    this->right = vec3::normalize(vec3::cross(this->front, this->worldUp));

    this->up = vec3::normalize(vec3::cross(this->right, this->front));

    this->position = this->target - this->front * this->distance;

    mat4::identity(view);
    mat4::lookAt(view,
                 this->position,
                 this->target,
                 this->up
    );
}

void camerathird::inputMouse(GLFWwindow* window, GLboolean constrainPitch) {

    double mouseX, mouseY;

    camerathird::getMouseCursorPos(window, mouseX, mouseY);

    if (this->firstMouse) {

        this->lastMouseX = mouseX;
        this->lastMouseY = mouseY;
        this->firstMouse = false;
        return;
    }

    float xOffset = mouseX - this->lastMouseX;
    float yOffset = this->lastMouseY - mouseY;

    this->lastMouseX = mouseX;
    this->lastMouseY = mouseY;

    xOffset *= this->mouseSensitivity;
    yOffset *= this->mouseSensitivity;

    this->yaw += xOffset;
    this->pitch += yOffset;

    const float pitchLimit = ops::degreesToRadians(89.0f);

    if (constrainPitch) {

        if (this->pitch > pitchLimit) this->pitch = pitchLimit;
        if (this->pitch < -pitchLimit) this->pitch = -pitchLimit;
    }
}
