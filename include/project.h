#ifndef GAME_H
#define GAME_H

#include <freetype/freetype.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine.h"

#include "player.h"

class game {

    struct renderData {
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

public:
    void init();

    void update(gltime& time);
    void run();

    void terminate();

    ~game() {}

private:

    engine eng;
    renderData rd;

};

#endif
