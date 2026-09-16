#include "text.h"

void text::draw(const buffer<font>& fonts, const buffer<texture>& textures, const buffer<shader>& shaders, GLuint VAO, GLuint VBO, const mat4& projection) {

    const auto& font = fonts.get(this->fontRef);
    const auto& shader = shaders.get(font.shaderRef);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader.bind();

    texture::active(GL_TEXTURE0);
    shader.intLoad(shader.getUniformLocation("text"), 0);
    shader.mat4Load(shader.getUniformLocation("projection"), projection);
    shader.vec3Load(
    shader.getUniformLocation("textColor"), {1.0f, 1.0f, 1.0f});

    glBindVertexArray(VAO);

    float x = position.x;
    float y = position.y;


    for (char c : string) {

        const glyph& ch = font.glyphs[
            static_cast<unsigned char>(c)
        ];

        float xpos =
        x + ch.bearingX * scale;

        float ypos =
        y - (ch.height - ch.bearingY) * scale;

        float w =
        ch.width * scale;

        float h =
        ch.height * scale;

        float vertices[6][4] = {

            { xpos,     ypos + h, 0.0f, 0.0f },
            { xpos,     ypos,     0.0f, 1.0f },
            { xpos + w, ypos,     1.0f, 1.0f },

            { xpos,     ypos + h, 0.0f, 0.0f },
            { xpos + w, ypos,     1.0f, 1.0f },
            { xpos + w, ypos + h, 1.0f, 0.0f }
        };

        textures.get(ch.textureRef).bind();

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

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

        x += (ch.advance >> 6) * scale;
    }

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(0);
    shader.unbind();
}
