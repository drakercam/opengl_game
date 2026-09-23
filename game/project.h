#ifndef GAME_H
#define GAME_H

#include "engine.h"

#include "player.h"
#include "weapon.h"

class game {

    struct renderData {
        std::vector<mesh> meshes;
        std::vector<shader> shaders;
        std::vector<texture> textures;
        std::vector<model> models;
        std::vector<font> fonts;
        std::vector<text> texts;

        void clear() {
            meshes.clear();
            shaders.clear();
            textures.clear();
            models.clear();
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
