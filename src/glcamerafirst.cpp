#include "glcamerafirst.h"
#include <GLFW/glfw3.h>

void camerafirst::getMouseCursorPos(GLFWwindow* window, double& mX, double& mY) {

    glfwGetCursorPos(window, &mX, &mY);
}

void camerafirst::update(mat4& view, gltime t) {
    this->front.x = cos(this->pitch) *  sin(this->yaw);
    this->front.y = sin(this->pitch);
    this->front.z = -cos(this->pitch) * cos(this->yaw);

    this->front = vec3::normalize(this->front);
    this->right = vec3::normalize(vec3::cross(this->front, this->worldUp));
    this->up = vec3::normalize(vec3::cross(this->right, this->front));

    mat4::identity(view);
    mat4::lookAt(view,
                 this->position,
                 this->position + this->front,
                 this->up
    );
}

void camerafirst::inputKeyboard(GLFWwindow* window, input in, gltime& t) {

    // KEYBOARD

    if (in.isKeyPressed(window, GLFW_KEY_W))
        this->position += this->front * this->speed;
    if (in.isKeyPressed(window, GLFW_KEY_S))
        this->position -= this->front * this->speed;
    if (in.isKeyPressed(window, GLFW_KEY_A))
        this->position -= this->right * this->speed;
    if (in.isKeyPressed(window, GLFW_KEY_D))
        this->position += this->right * this->speed;
    if (in.isKeyPressed(window, GLFW_KEY_LEFT))
        this->yaw -= this->rotationSpeed * t.getDelta();
    if (in.isKeyPressed(window, GLFW_KEY_RIGHT))
        this->yaw += this->rotationSpeed * t.getDelta();
    if (in.isKeyPressed(window, GLFW_KEY_UP))
        this->pitch += this->rotationSpeed * t.getDelta();
    if (in.isKeyPressed(window, GLFW_KEY_DOWN))
        this->pitch -= this->rotationSpeed * t.getDelta();
    if (in.isKeyPressed(window, GLFW_KEY_SPACE))
        this->position.y += 1.0f * t.getDelta();
    if (in.isKeyPressed(window, GLFW_KEY_LEFT_SHIFT))
        this->position.y -= 1.0f * t.getDelta();
}

void camerafirst::inputMouse(GLFWwindow* window, GLboolean constrainPitch) {

    double mouseX, mouseY;

    camerafirst::getMouseCursorPos(window, mouseX, mouseY);

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
