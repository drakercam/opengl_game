#ifndef GLMESH_H
#define GLMESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <span>

#include "glbuffer.h"

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

struct meshData {
    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;
};

struct mesh {
    GLuint VAO = 0;
    unsigned int indexCount = 0;

    bufferGPU VBO{GL_ARRAY_BUFFER};
    bufferGPU EBO{GL_ELEMENT_ARRAY_BUFFER};
};

void meshLoad(mesh& mesh, std::span<const vertex> vertices, std::span<const unsigned int> indices);
void meshLoad(mesh& mesh, const meshData& data);
void meshFree(mesh& mesh);
void meshDraw(const mesh& mesh);
void meshDrawWireFrame(const mesh& mesh);

meshData triangleMake(void);
meshData rectangleMake(void);
meshData circleMake(size_t segments = 32);
meshData cubeMake(void);
meshData sphereMake(size_t segments = 32, size_t rings = 16);

#endif
