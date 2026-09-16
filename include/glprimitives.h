#ifndef GLPRIMITIVES_H
#define GLPRIMITIVES_H

#include "glshader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glbuffer.h"
#include "glmesh.h"

class triangle {
public:
    triangle(const std::vector<size_t>& t) {

        this->textureRefs.setElementsData(t);

        std::vector<vertex> vertices{

            // Front face (+Z)
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {}, {}},
            {{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {}, {}},
            {{ 0.0f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 1.0f}, {}, {}}
        };

        this->vertices.setElementsData(vertices);
        glGenVertexArrays(1, &this->VAO);
        glBindVertexArray(this->VAO);

        VBO.upload(vertices.data(), vertices.size() * sizeof(vertex), GL_STATIC_DRAW);

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

    ~triangle() {

        glDeleteVertexArrays(1, &this->VAO);
    }

    void draw(const shader& s, const buffer<texture>& textures);
    void drawWireFrame(const shader& s, const buffer<texture>& textures);

private:
    GLuint VAO = 0;
    buffer<vertex> vertices;
    buffer<size_t> textureRefs;
    bufferGPU VBO{GL_ARRAY_BUFFER};
    bufferGPU EBO{GL_ELEMENT_ARRAY_BUFFER};
};

class rectangle {

public:
    rectangle(const std::vector<size_t>& t) {

        this->textureRefs.setElementsData(t);

        std::vector<vertex> vertices{

            // Front face (+Z)
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {}, {}},
            {{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {}, {}},
            {{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {}, {}},
            {{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {}, {}}
        };

        std::vector<unsigned int> indices = {
            0, 1, 3, 1, 2, 3
        };

        this->vertices.setElementsData(vertices);
        this->indices.setElementsData(indices);

        glGenVertexArrays(1, &this->VAO);
        glBindVertexArray(this->VAO);

        VBO.upload(vertices.data(), vertices.size() * sizeof(vertex), GL_STATIC_DRAW);
        EBO.upload(indices.data(), sizeof(unsigned int) * indices.size(), GL_STATIC_DRAW);

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

    ~rectangle() {

        glDeleteVertexArrays(1, &this->VAO);
    }

    void draw(const shader& s, const buffer<texture>& textures);
    void drawWireFrame(const shader& s, const buffer<texture>& textures);


private:
    GLuint VAO = 0;
    buffer<vertex> vertices;
    buffer<unsigned int> indices;
    buffer<size_t> textureRefs;
    bufferGPU VBO{GL_ARRAY_BUFFER};
    bufferGPU EBO{GL_ELEMENT_ARRAY_BUFFER};
};

class cube {

public:

    cube(const std::vector<size_t>& t) {

        this->textureRefs.setElementsData(t);

        std::vector<vertex> vertices{

            // Front face (+Z)
            {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {}, {}},
            {{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {}, {}},
            {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {}, {}},
            {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {}, {}},

            // Back face (-Z)
            {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {}, {}},
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {}, {}},
            {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {}, {}},
            {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {}, {}},

            // Left face (-X)
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {}, {}},
            {{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {}, {}},
            {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {}, {}},
            {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {}, {}},
            // Right face (+X)
            {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {}, {}},
            {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {}, {}},
            {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {}, {}},
            {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {}, {}},

            // Top face (+Y)
            {{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {}, {}},
            {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {}, {}},
            {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {}, {}},
            {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {}, {}},

            // Bottom face (-Y)
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {}, {}},
            {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {}, {}},
            {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {}, {}},
            {{-0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {}, {}}
        };

        std::vector<unsigned int> indices = {

            // Front
            0, 1, 2,
            2, 3, 0,

            // Back
            4, 5, 6,
            6, 7, 4,

            // Left
            8, 9, 10,
            10, 11, 8,

            // Right
            12, 13, 14,
            14, 15, 12,

            // Top
            16, 17, 18,
            18, 19, 16,

            // Bottom
            20, 21, 22,
            22, 23, 20
        };

        this->vertices.setElementsData(vertices);
        this->indices.setElementsData(indices);

        glGenVertexArrays(1, &this->VAO);
        glBindVertexArray(this->VAO);

        VBO.upload(vertices.data(), vertices.size() * sizeof(vertex), GL_STATIC_DRAW);
        EBO.upload(indices.data(), sizeof(unsigned int) * indices.size(), GL_STATIC_DRAW);

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

    ~cube() {

        glDeleteVertexArrays(1, &this->VAO);
    }

    void draw(const shader& s, const buffer<texture>& textures);
    void drawWireFrame(const shader& s, const buffer<texture>& textures);


private:
    GLuint VAO = 0;
    buffer<vertex> vertices;
    buffer<unsigned int> indices;
    buffer<size_t> textureRefs;
    bufferGPU VBO{GL_ARRAY_BUFFER};
    bufferGPU EBO{GL_ELEMENT_ARRAY_BUFFER};

};

#endif
