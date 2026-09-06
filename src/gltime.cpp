#include "gltime.h"

void gltime::update(gltime::data& t) {
    float now = (float)glfwGetTime();
    t.delta = now - t.last;
    t.last = now;
    t.total += t.delta;
}
