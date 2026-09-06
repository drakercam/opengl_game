#ifndef GLMESH_H
#define GLMESH_H

#include "glshader.h"
#include <cstddef>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glbuffer.h"
#include "gltexture.h"

#define MAX_BONE_INFLUENCE 4

struct vertex {

    vec3 pos;
    vec3 normal;
    vec2 uv;
    vec3 tangent;
    vec3 bitangent;

    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];

    vertex(vec3 pos, vec3 normal, vec2 uv, vec3 tangent = {}, vec3 bitangent = {})
    : pos(pos), normal(normal), uv(uv), tangent(tangent), bitangent(bitangent) {
    }
};

namespace mesh {

    struct data {
        GLuint VAO = 0;
        buffer::data<vertex> vertices;
        buffer::data<GLuint> indices;
        buffer::data<GLuint> textureRefs;
        bufferGPU::data VBO{GL_ARRAY_BUFFER};
        bufferGPU::data EBO{GL_ELEMENT_ARRAY_BUFFER};

        data(const std::vector<vertex>& v, const std::vector<unsigned int>& i, const std::vector<GLuint>& t = {}) {

            this->vertices.elements = v;
            this->indices.elements = i;
            this->textureRefs.elements = t;

            // setup mesh
            glGenVertexArrays(1, &this->VAO);
            glBindVertexArray(this->VAO);

            bufferGPU::upload(VBO, buffer::elementsData(this->vertices), buffer::size(this->vertices) * sizeof(vertex), GL_STATIC_DRAW);
            bufferGPU::upload(EBO, buffer::elementsData(this->indices), sizeof(unsigned int) * buffer::size(this->indices), GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, pos));
            glEnableVertexAttribArray(0);
            // color attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
            glEnableVertexAttribArray(1);
            // uv attribute
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, uv));
            glEnableVertexAttribArray(2);

            glBindVertexArray(0);
        }

        ~data() {

            glDeleteVertexArrays(1, &this->VAO);
        }
    };

    void draw(const data& mesh, const shader::data& s, const buffer::data<texture::data>& textures);
}

#endif
