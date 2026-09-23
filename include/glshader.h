#ifndef GLSHADER_H
#define GLSHADER_H

#include <span>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glmath.h"
#include "gltexture.h"
#include "glbuffer.h"

struct shader {
    GLuint id;
};

void shaderLoad(shader& shader, const char* vertexShaderSource, const char* fragShaderSource);
void shaderFree(shader& shader);
void shaderBind(const shader& shader);
void shaderUnbind(void);
int shaderGetUniformLocation(const shader& shader, const char* name);
void shaderSetTextureUnitToSampler(const shader& shader, std::string name, unsigned int value);
void shaderIntLoad(const shader& shader, int location, int value);
void shaderFloatLoad(const shader& shader, int location, float value);
void shaderVec2Load(const shader& shader, int location, vec2 value);
void shaderVec3Load(const shader& shader, int location, vec3 value);
void shaderMat3Load(const shader& shader, int location, mat3 value);
void shaderMat4Load(const shader& shader, int location, mat4 value);

void shaderSetTexture(const shader& shader, const std::vector<texture>& textures, size_t textureRef, unsigned int textureUnit, const char* sampler);
void shaderSetTextures(const shader& shader, std::span<const texture> textures, std::span<const size_t> textureRefs);

#endif
