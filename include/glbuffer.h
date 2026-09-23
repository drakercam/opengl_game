#ifndef GLALGORITHMS_H
#define GLALGORITHMS_H

#include <iostream>
#include <vector>
#include "gltools.h"
#include "glmath.h"

class bufferGPU {

public:
    bufferGPU(GLenum target) : target(target) {

        glGenBuffers(1, &this->id);
    }

    ~bufferGPU() {

        glDeleteBuffers(1, &id);
    }

    void bind();
    void upload(const void* data, size_t size, GLenum usage);

private:
    GLuint id;
    GLenum target;

};

#endif
