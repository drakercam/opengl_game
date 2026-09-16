#ifndef TEXT_H
#define TEXT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <array>
#include "glbuffer.h"
#include "gltexture.h"
#include "glshader.h"

#include <string>
#include <cstddef>

#include "glmath.h"

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
};

class text {

public:
    text(size_t fontRef, const char* string, vec2 position, float scale) {
        this->fontRef = fontRef;
        this->string = string;
        this->position = position;
        this->scale = scale;
    }

    void draw(const buffer<font>& fonts, const buffer<texture>& textures, const buffer<shader>& shaders, GLuint VAO, GLuint VBO, const mat4& projection);

private:
    size_t fontRef;

    std::string string;

    vec2 position;

    float scale;

};

#endif
