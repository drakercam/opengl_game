#include "input.h"

bool input::isKeyPressed(GLFWwindow* window, int key) {

    return glfwGetKey(window, key) == GLFW_PRESS;
}
