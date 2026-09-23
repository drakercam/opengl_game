#include "text.h"

void fontLoad(font& font, const std::string filepath, size_t shaderRef, FT_Library& ft, std::vector<texture>& textures) {
    glGenVertexArrays(1, &font.VAO);
    glGenBuffers(1, &font.VBO);

    glBindVertexArray(font.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, font.VBO);

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

        texture textureData;
        textureLoadGlyph(textureData, glyph->bitmap.buffer, glyph->bitmap.width, glyph->bitmap.rows);

        textures.push_back(textureData);

        font.glyphs[c] = {
            textureRef,

            static_cast<int>(glyph->bitmap.width),
            static_cast<int>(glyph->bitmap.rows),

            glyph->bitmap_left,
            glyph->bitmap_top,

            static_cast<unsigned int>(glyph->advance.x)
        };
    }

    FT_Done_Face(face);

    font.shaderRef = shaderRef;
}

void textLoad(text& text, const char* string, vec2 position, float scale, size_t fontRef) {
    text.fontRef = fontRef;
    text.string = string;
    text.position = position;
    text.scale = scale;
}

void textDraw(const text& text, const std::span<const font> fonts, const std::span<const texture> textures, const std::span<const shader> shaders, const mat4& projection) {

    const auto& font = fonts[text.fontRef];
    const auto& shader = shaders[font.shaderRef];

    gltools::disable(GL_DEPTH_TEST);
    gltools::enable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shaderBind(shader);

    textureActive(GL_TEXTURE0);
    shaderIntLoad(shader, shaderGetUniformLocation(shader, "text"), 0);
    shaderMat4Load(shader, shaderGetUniformLocation(shader, "projection"), projection);
    shaderVec3Load(shader, shaderGetUniformLocation(shader, "textColor"), {1.0f, 1.0f, 1.0f});

    glBindVertexArray(font.VAO);

    float x = text.position.x;
    float y = text.position.y;


    for (char c : text.string) {

        const glyph& ch = font.glyphs[
            static_cast<unsigned char>(c)
        ];

        float xpos =
        x + ch.bearingX * text.scale;

        float ypos =
        y - (ch.height - ch.bearingY) * text.scale;

        float w =
        ch.width * text.scale;

        float h =
        ch.height * text.scale;

        float vertices[6][4] = {

            { xpos,     ypos + h, 0.0f, 0.0f },
            { xpos,     ypos,     0.0f, 1.0f },
            { xpos + w, ypos,     1.0f, 1.0f },

            { xpos,     ypos + h, 0.0f, 0.0f },
            { xpos + w, ypos,     1.0f, 1.0f },
            { xpos + w, ypos + h, 1.0f, 0.0f }
        };

        textureBind(textures[ch.textureRef]);

        glBindBuffer(GL_ARRAY_BUFFER, font.VBO);

        glBufferSubData(
            GL_ARRAY_BUFFER,
            0,
            sizeof(vertices),
                        vertices
        );

        glDrawArrays(
            GL_TRIANGLES,
            0,
            6
        );

        x += (ch.advance >> 6) * text.scale;
    }

    gltools::disable(GL_BLEND);
    gltools::enable(GL_DEPTH_TEST);

    glBindVertexArray(0);
    shaderUnbind();
}
