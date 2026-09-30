#ifndef GAME_H
#define GAME_H

#include "engine.h"

#include "player.h"
#include "enemy.h"
#include "weapon.h"

enum CAMERA_MODE {
	FIRST_PERSON,
	THIRD_PERSON
};

enum WEAPON_CHOICE {
	PISTOL,
	SHOTGUN
};

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
            texts.clear();
        }
    };

public:
    void init();
	void processInput(const SDL_Event& event);
    void update();
    void render();
    void terminate();
    
    gltime getTime(void)const { return time; }

private:

    engine eng;
    renderData renderdata;
    
    // Game State Data
    CAMERA_MODE camMode{CAMERA_MODE::FIRST_PERSON};
    WEAPON_CHOICE weaponChoice{WEAPON_CHOICE::SHOTGUN};
    
    // Player + Enemy + Weapons
    vec3 playerPosition{0.0f};
    vec3 enemyPosition{2.0f, 0.5f, 2.0f};
    player playerState{playerPosition, 0, 1};
    enemy enemyState{enemyPosition, 6, 2};
    weapon pistol{1, 0, {1000.0f, 250.0f}, {732.0f, 500.0f}};
    weapon shotgun{2, 0, {1000.0f, 250.0f}, {732.0f, 500.0f}};
    
    // cameras
	vec3 cameraPosition{0.0f, 0.75f, -5.0f};
	float cameraOffset{1.5f};
    vec3 cameraTarget{0.0f, playerPosition.y + cameraOffset, 0.0f};
    vec3 up{0.0f, 1.0f, 0.0f};
    vec3 cameraDirection{vec3::normalize(cameraPosition - cameraTarget)};
    
    camerafirst camFirstPerson{cameraPosition, cameraTarget, cameraDirection, {0.0f, 0.0f, -1.0f}, vec3::normalize(vec3::cross(up, cameraDirection)), up};
    camerathird camThirdPerson{cameraPosition, cameraTarget, cameraDirection, {0.0f, 0.0f, -1.0f}, vec3::normalize(vec3::cross(up, cameraDirection)), up};
    
    // matrices
	mat4 modelMatrix;
    mat4 viewMatrix;
    mat4 projectionMatrix;
    mat4 orthographicMatrix;
	
	// other
    gltime time;
    bool running{true};
};

#endif
