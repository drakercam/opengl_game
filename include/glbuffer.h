#ifndef GLALGORITHMS_H
#define GLALGORITHMS_H

#include <iostream>
#include <vector>
#include "gltools.h"
#include "glmath.h"

namespace bufferGPU {

    struct data {
        GLuint id;
        GLenum target;

        data (GLenum target) : target(target) {

            glGenBuffers(1, &this->id);
        }

        ~data() {

            glDeleteBuffers(1, &id);
        }
    };
    void bind(data& buffer);
    void upload(data& buffer, const void* data, size_t size, GLenum usage);
};

namespace buffer {

    template<typename TYPE>
    struct data {
        std::vector<TYPE> elements;
    };

    template<typename TYPE, typename... ARGS>
    inline void emplace(data<TYPE>& buffer, ARGS&&... args) {
        buffer.elements.emplace_back(std::forward<ARGS>(args)...);
    }

    template<typename TYPE>
    inline const TYPE& get(const data<TYPE>& buffer, size_t index) {
        return buffer.elements.at(index);
    }

    template<typename TYPE>
    inline const size_t size(const data<TYPE>& buffer) {
        return buffer.elements.size();
    }

    template<typename TYPE>
    inline const TYPE* elementsData(const data<TYPE>& buffer) {
        return buffer.elements.data();
    }
}

#endif
