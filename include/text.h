#ifndef TEXT_H
#define TEXT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <array>
#include <span>
#include <cstring>
#include <string>
#include <cstddef>

#include "glmath.h"
#include "glbuffer.h"
#include "gltexture.h"
#include "glshader.h"
#include "gltools.h"

struct glyph {

    size_t textureRef;

    int width;
    int height;

    int bearingX;
    int bearingY;

    unsigned int advance;
};

struct font {

    std::array<glyph, 128> glyphs;

    size_t shaderRef;

    GLuint VAO;
    GLuint VBO;
};

void fontLoad(font& font, const std::string filepath, size_t shaderRef, FT_Library& ft, std::vector<texture>& textures);


struct text {
    size_t fontRef;
    std::string string;
    vec2 position;
    float scale;
};

void textLoad(text& text, const char* string, vec2 position, float scale, size_t fontRef);
void textDraw(const text& text, const std::span<const font> fonts, const std::span<const texture> textures, const std::span<const shader> shaders, const mat4& projection);

#endif
