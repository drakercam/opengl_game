#ifndef GLTIME_H
#define GLTIME_H

#include <GLFW/glfw3.h>

namespace gltime {

    struct data {
        float last;
        float delta;
        float total;

        data() {
            last = 0.0f;
            delta = 0.0f;
            total = 0.0f;
        }
    };

    void update(data& t);
}

#endif
