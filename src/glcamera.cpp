#include "glcamera.h"

void camera::getMouseCursorPos(GLFWwindow* window, double& mX, double& mY) {

    glfwGetCursorPos(window, &mX, &mY);
}

void camera::update(mat4& view, gltime t) {
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

void camera::input(GLFWwindow* window, gltime& t) {

    // KEYBOARD

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        this->position += this->front * this->speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        this->position -= this->front * this->speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        this->position -= this->right * this->speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        this->position += this->right * this->speed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        this->yaw -= this->rotationSpeed * t.getDelta();
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        this->yaw += this->rotationSpeed * t.getDelta();
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        this->pitch += this->rotationSpeed * t.getDelta();
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        this->pitch -= this->rotationSpeed * t.getDelta();
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        this->position.y += 1.0f * t.getDelta();
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        this->position.y -= 1.0f * t.getDelta();
}

void camera::inputMouse(GLFWwindow* window, GLboolean constrainPitch) {

    double mouseX, mouseY;

    camera::getMouseCursorPos(window, mouseX, mouseY);

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
