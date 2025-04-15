#ifndef PLAYER_H
#define PLAYER_H

#include "playerUI.h"
#include "structs.h"
#include "animation.h"

#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>

#define MAX_HEALTH 20
#define FALLING_SPEED 300
#define ENTITY_SPEED 100

#define PLAYER_TEXTURE_WIDTH 192
#define PLAYER_TEXTURE_HEIGHT 192

class Player {
public:
    // animation
    SDL_Texture* texture = nullptr;
    SDL_FRect src_rect = {0, 0, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT};
    PlayerAnimationData animations;
    Animation animationHandler;
    SDL_FlipMode flip;

    float hp = MAX_HEALTH;

    // get smth
    void damage(float damage);
    bool isDead();

    // render
    SDL_AppResult init();
    SDL_AppResult render();
    SDL_AppResult handleEvents(SDL_Event* event);
    SDL_AppResult update(float delta);
    void cleanup();

private:
    PlayerUI playerUI;
    float x = 0;
    float y = 0;
    bool isWalking = false;

    bool playDeathAnimation = false;

    float attack_cooldown;
    float defending_cooldown;
    bool isDefending = false;
    bool isAttacking = false;
};

#endif
