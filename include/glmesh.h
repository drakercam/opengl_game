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

    vertex(vec3 pos = {0.0f, 0.0f, 0.0f}, vec3 normal = {0.0f, 0.0f, 0.0f}, vec2 uv = {0.0f, 0.0f}, vec3 tangent = {}, vec3 bitangent = {})
    : pos(pos), normal(normal), uv(uv), tangent(tangent), bitangent(bitangent) {
    }
};

class mesh {

public:
    mesh(const std::vector<vertex>& v, const std::vector<unsigned int>& i, const std::vector<size_t>& t = {}) {

        this->vertices.setElementsData(v);
        this->indices.setElementsData(i);
        this->textureRefs.setElementsData(t);

        // setup mesh
        glGenVertexArrays(1, &this->VAO);
        glBindVertexArray(this->VAO);

        VBO.upload(this->vertices.getElementsData(), this->vertices.size() * sizeof(vertex), GL_STATIC_DRAW);
        EBO.upload(this->indices.getElementsData(), sizeof(unsigned int) * this->indices.size(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, pos));
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
        glEnableVertexAttribArray(1);
        // uv attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, uv));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);

        std::cout << "CREATE mesh VAO=" << VAO << '\n';
    }

    ~mesh() {
        std::cout << "DESTROY mesh VAO=" << VAO << '\n';
        glDeleteVertexArrays(1, &this->VAO);
    }

    void draw(const shader& s) const;
    void drawWireFrame(const shader& s) const;
    const buffer<size_t>& getTextureRefs(void) const { return textureRefs; }

private:
    GLuint VAO = 0;
    buffer<vertex> vertices;
    buffer<GLuint> indices;
    buffer<size_t> textureRefs;
    bufferGPU VBO{GL_ARRAY_BUFFER};
    bufferGPU EBO{GL_ELEMENT_ARRAY_BUFFER};

};

#endif
