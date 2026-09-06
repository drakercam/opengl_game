#include "glcamera.h"

void camera::getMouseCursorPos(GLFWwindow* window, double& mX, double& mY) {

    glfwGetCursorPos(window, &mX, &mY);
}

void camera::update(mat4& view, camera::data& cam, gltime::data t) {
    cam.front.x = cos(cam.pitch) *  sin(cam.yaw);
    cam.front.y = sin(cam.pitch);
    cam.front.z = -cos(cam.pitch) * cos(cam.yaw);

    cam.front = vec3::normalize(cam.front);
    cam.right = vec3::normalize(vec3::cross(cam.front, cam.worldUp));
    cam.up = vec3::normalize(vec3::cross(cam.right, cam.front));

    mat4::identity(view);
    mat4::lookAt(view,
                 cam.position,
                 cam.position + cam.front,
                 cam.up
    );
}

void camera::input(GLFWwindow* window, camera::data& cam, gltime::data& t) {

    // KEYBOARD

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.position += cam.front * cam.speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.position -= cam.front * cam.speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.position -= cam.right * cam.speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.position += cam.right * cam.speed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        cam.yaw -= cam.rotationSpeed * t.delta;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        cam.yaw += cam.rotationSpeed * t.delta;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cam.pitch += cam.rotationSpeed * t.delta;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cam.pitch -= cam.rotationSpeed * t.delta;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cam.position.y += 1.0f * t.delta;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cam.position.y -= 1.0f * t.delta;
}

void camera::inputMouse(GLFWwindow* window, camera::data& cam, GLboolean constrainPitch) {

    double mouseX, mouseY;

    camera::getMouseCursorPos(window, mouseX, mouseY);

    if (cam.firstMouse) {

        cam.lastMouseX = mouseX;
        cam.lastMouseY = mouseY;
        cam.firstMouse = false;
        return;
    }

    float xOffset = mouseX - cam.lastMouseX;
    float yOffset = cam.lastMouseY - mouseY;

    cam.lastMouseX = mouseX;
    cam.lastMouseY = mouseY;

    xOffset *= cam.mouseSensitivity;
    yOffset *= cam.mouseSensitivity;

    cam.yaw += xOffset;
    cam.pitch += yOffset;

    const float pitchLimit = ops::degreesToRadians(89.0f);

    if (constrainPitch) {

        if (cam.pitch > pitchLimit) cam.pitch = pitchLimit;
        if (cam.pitch < -pitchLimit) cam.pitch = -pitchLimit;
    }
}
