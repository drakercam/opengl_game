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

template<typename TYPE>
class buffer {

public:
    template<typename... ARGS>
    void emplace(ARGS&&... args) {
        this->elements.emplace_back(std::forward<ARGS>(args)...);
    }

    TYPE& get(const size_t index) {
        return this->elements.at(index);
    }

    const TYPE& get(const size_t index) const {
        return this->elements.at(index);
    }

    const size_t size() const {
        return this->elements.size();
    }

    std::vector<TYPE>& getElements() {
        return this->elements;
    }

    const TYPE* getElementsData() const {
        return this->elements.data();
    }

    void setElementsData(const std::vector<TYPE> elements) {
        this->elements = elements;
    }

    void clear(void) {
        this->elements.clear();
    }

private:
    std::vector<TYPE> elements;

};

#endif
