#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include <iostream>
#include <stb_image.h>
#include <glad/glad.h>

struct texture {
    GLuint id;
    std::string name;
    std::string path;
};

void textureLoad(texture& texture, const char* imagePath);
void textureLoadGlyph(texture& texture, const unsigned char* data, int width, int height);
void textureActive(int textureUnit);
void textureBind(const texture& texture);
void textureUnbind(void);

#endif
