#ifndef TEXT_H
#define TEXT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <array>
#include "glbuffer.h"
#include "gltexture.h"
#include "glshader.h"

#include <cstring>
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

    GLuint VAO;
    GLuint VBO;

    font(const std::string filepath, size_t shaderRef, FT_Library& ft, buffer<texture>& textures) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(float) * 6 * 4,
                     nullptr,
                     GL_DYNAMIC_DRAW
        );

        glEnableVertexAttribArray(0);

        glVertexAttribPointer(
            0,
            4,
            GL_FLOAT,
            GL_FALSE,
            4 * sizeof(float),
                              nullptr
        );

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        FT_Face face;

        if (FT_New_Face(ft, filepath.c_str(), 0, &face)) {

            std::cout
            << "ERROR::FREETYPE: Failed to load font\n";
        }

        FT_Set_Pixel_Sizes(face, 0, 48);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (unsigned char c = 0; c < 128; ++c) {

            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {

                std::cout
                << "ERROR::FREETYPE: Failed to load Glyph "
                << static_cast<int>(c)
                << '\n';

                continue;
            }

            FT_GlyphSlot glyph = face->glyph;

            size_t textureRef = textures.size();

            textures.emplace(
                glyph->bitmap.buffer,
                glyph->bitmap.width,
                glyph->bitmap.rows
            );

            glyphs[c] = {
                textureRef,

                static_cast<int>(glyph->bitmap.width),
                static_cast<int>(glyph->bitmap.rows),

                glyph->bitmap_left,
                glyph->bitmap_top,

                static_cast<unsigned int>(glyph->advance.x)
            };
        }

        FT_Done_Face(face);

        this->shaderRef = shaderRef;
    }
};

class text {

public:
    text(const char* string, vec2 position, float scale, size_t fontRef) {
        this->fontRef = fontRef;
        this->string = string;
        this->position = position;
        this->scale = scale;
    }

    void draw(const buffer<font>& fonts, const buffer<texture>& textures, const buffer<shader>& shaders, const mat4& projection);

private:
    size_t fontRef;

    std::string string;

    vec2 position;

    float scale;

};

#endif
