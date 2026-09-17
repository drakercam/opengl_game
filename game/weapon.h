#ifndef WEAPON_H
#define WEAPON_H

#include "engine.h"

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

    void draw(const shader& shader, const buffer<rectangle>& rectangles, const buffer<texture>& textures) {
        auto& quad = rectangles.get(quadRef);

        shader.setTexture(textures, this->textureRef, 0, "tex");

        quad.draw(shader);
    }
};

#endif
