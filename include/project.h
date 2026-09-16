#ifndef GAME_H
#define GAME_H

#include <freetype/freetype.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "filehandling.h"
#include "glbuffer.h"
#include "glmath.h"
#include "glmesh.h"
#include "glshader.h"
#include "gltools.h"
#include "gltexture.h"
#include "gltime.h"
#include "glcamerafirst.h"
#include "glcamerathird.h"
#include "glprimitives.h"
#include "glmodel.h"
#include "player.h"
#include "text.h"

class glengine {

    struct gameData {
        buffer<mesh> meshes;
        buffer<shader> shaders;
        buffer<texture> textures;
        buffer<model> models;
        buffer<triangle> triangles;
        buffer<cube> cubes;
        buffer<rectangle> rectangles;
        buffer<font> fonts;

        void clear() {
            meshes.clear();
            shaders.clear();
            textures.clear();
            models.clear();
            triangles.clear();
            cubes.clear();
            rectangles.clear();
            fonts.clear();
        }
    };

    struct dataGL {
        GLFWwindow* window;
        FT_Library ft;

        GLuint textVAO;
        GLuint textVBO;
    };

public:
    int initialize();
    void initializeResources();

    void update(gltime& time);
    void run();

    void terminate();

    ~glengine() {}

private:

    dataGL dgl;
    gameData gd;

    font initializeTextRenderResources() {
        glGenVertexArrays(1, &dgl.textVAO);
        glGenBuffers(1, &dgl.textVBO);

        glBindVertexArray(dgl.textVAO);

        glBindBuffer(GL_ARRAY_BUFFER, dgl.textVBO);

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

        // initialize free type
        if (FT_Init_FreeType(&dgl.ft)) {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        }

        FT_Face face;

        if (FT_New_Face(
            dgl.ft,
            "../resources/Yellow Banana.otf",
            0,
            &face
        )) {

            std::cout
            << "ERROR::FREETYPE: Failed to load font\n";
        }

        FT_Set_Pixel_Sizes(face, 0, 48);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        font font;

        for (unsigned char c = 0; c < 128; ++c) {

            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {

                std::cout
                << "ERROR::FREETYPE: Failed to load Glyph "
                << static_cast<int>(c)
                << '\n';

                continue;
            }

            FT_GlyphSlot glyph = face->glyph;

            size_t textureRef = gd.textures.size();

            gd.textures.emplace(
                glyph->bitmap.buffer,
                glyph->bitmap.width,
                glyph->bitmap.rows
            );

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

        return font;
    }

};

#endif
