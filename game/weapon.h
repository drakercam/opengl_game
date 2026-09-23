#ifndef WEAPON_H
#define WEAPON_H

#include "engine.h"
#include "glshader.h"

struct weapon {
    size_t textureRef;
    size_t quadRef;
    vec2 position;
    vec2 scale;

    weapon(size_t textureRef, size_t quadRef, vec2 position, vec2 scale) {
        this->textureRef = textureRef;
        this->quadRef = quadRef;
        this->position = position;
        this->scale = scale;
    }

    void draw(const shader& shader, const std::vector<mesh>& rectangles, const std::vector<texture>& textures) {
        auto& quad = rectangles.at(quadRef);

        shaderSetTexture(shader, textures, this->textureRef, 0, "tex");

        meshDraw(quad);
    }
};

#endif
