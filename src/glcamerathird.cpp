#include "glcamerathird.h"
#include <GLFW/glfw3.h>
#include <print>

#define PITCH_MIN -60.0f
#define PITCH_MAX 0.0f

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

    float distance = 4.0f;
    vec3 playerPosition = this->target;

    this->position = playerPosition - this->front * distance;

    mat4::identity(view);
    mat4::lookAt(view, this->position, playerPosition, this->worldUp);
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

    const float pitchMin = ops::degreesToRadians(PITCH_MIN);
    const float pitchMax = ops::degreesToRadians(PITCH_MAX);

    if (constrainPitch) {

        if (this->pitch > pitchMax) this->pitch = pitchMax;
        if (this->pitch < pitchMin) this->pitch = pitchMin;
    }
}
