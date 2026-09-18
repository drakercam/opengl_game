#include "glcamerathird.h"
#include <GLFW/glfw3.h>
#include <print>

void camerathird::getMouseCursorPos(GLFWwindow* window, double& mX, double& mY) {

    glfwGetCursorPos(window, &mX, &mY);
}

void camerathird::update(mat4& view, gltime t) {

    // looking
    this->front.x = cos(this->pitch) * sin(this->yaw);
    this->front.y = sin(this->pitch);
    this->front.z = -cos(this->pitch) * cos(this->yaw);

    this->front = vec3::normalize(this->front);

    this->right = vec3::normalize(vec3::cross(this->front, this->worldUp));
    this->up = vec3::normalize(vec3::cross(this->right, this->front));

    vec3 horizontalFront{this->front.x, 0.0f, this->front.z};
    horizontalFront = vec3::normalize(horizontalFront);

    vec3 shoulderRight = vec3::normalize(vec3::cross(horizontalFront, this->worldUp));

    float distance = 2.0f;
    float xOffset = -1.0f;
    float aimDistance = 2.0f;
    float heightOffset = -0.5f;

    vec3 playerPosition = this->target;
    vec3 cameraOffset = horizontalFront * distance + shoulderRight * xOffset + this->worldUp * heightOffset;

    this->position = playerPosition - cameraOffset;

    vec3 lookTarget = playerPosition + this->front * distance;

    mat4::identity(view);
    mat4::lookAt(view, this->position, lookTarget, this->worldUp);
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
        if (this->pitch < -pitchLimit) this->pitch = -30.0f;
    }
}
