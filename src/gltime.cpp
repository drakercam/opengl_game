#include "gltime.h"

void gltime::update() {
    float now = (float)glfwGetTime();
    this->delta = now - this->last;
    this->last = now;
    this->total += this->delta;
}
