#include "glbuffer.h"

void bufferGPU::bind() {

    glBindBuffer(this->target, this->id);
}

void bufferGPU::upload(const void* data, size_t size, GLenum usage) {

    bind();
    glBufferData(this->target, size, data, usage);
}
