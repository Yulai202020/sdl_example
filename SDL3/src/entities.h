#ifndef ENTITY_H
#define ENTITY_H

#include "structs.h"
#include "animation.h"

#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#define ENTITY_TEXTURE_WIDTH 128
#define ENTITY_TEXTURE_HEIGHT 128

class Entity {
public:
    SDL_Texture* texture;
    SDL_FRect src_rect = {0, 0, ENTITY_TEXTURE_WIDTH, ENTITY_TEXTURE_HEIGHT};
    EntityAnimationData animations;
    Animation animationHandler;

    // position
    float x, y;

    float hp;
    bool playDeathAnimation = false;
    bool isDead = false;

    Entity(float health, int posX, int posY) : hp(health), x(posX), y(posY) {}

    float getDistance(int attacker_x, int attacker_y) const;
    bool Damage(int attacker_x, int attacker_y, float damage);

    SDL_AppResult init();
    SDL_AppResult update(float delta);
    SDL_AppResult render();
    void cleanup();
};

Entity* getNearestEntity(int x, int y, const std::vector<Entity*>& entities);
void initEntities(std::vector<Entity*>* entities);

#endif
