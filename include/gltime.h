#ifndef GLTIME_H
#define GLTIME_H

#include <GLFW/glfw3.h>

class gltime {

    public:
        void update(void);

        const float getDelta() const { return delta; }
        const float getLast() const { return last; }
        const float getTotal() const { return total; }

    private:
        float last = 0.0f;
        float delta = 0.0f;
        float total = 0.0f;
};

#endif
