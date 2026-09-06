#include "glbuffer.h"

void bufferGPU::bind(bufferGPU::data& buffer) {

    glBindBuffer(buffer.target, buffer.id);
}

void bufferGPU::upload(bufferGPU::data& buffer, const void* data, size_t size, GLenum usage) {

    bind(buffer);
    glBufferData(buffer.target, size, data, usage);
}
