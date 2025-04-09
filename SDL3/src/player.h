#ifndef PLAYER_H
#define PLAYER_H

#include "structs.h"
#include "animation.h"

#include <SDL3/SDL.h>
#include <vector>
#include <algorithm>  // For std::max
#include <cmath>      // For std::sqrt

#define PLAYER_TEXTURE_WIDTH 192
#define PLAYER_TEXTURE_HEIGHT 192

#define FALLING_SPEED 300
#define ENTITY_SPEED 100
#define MAX_JUMPING_TIME 0.5f
#define MAX_ATTACKING_COOLDOWN 2.5f
#define MAX_DEFENDING_COOLDOWN 0.5f

class Player {
public:
    // animation
    SDL_Texture* texture = nullptr;
    SDL_FRect src_rect = {0, 0, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT};
    PlayerAnimationData animations;
    Animation animationHandler;
    SDL_FlipMode flip;

    // position
    float x = 0;
    float y = 0;
    bool isWalking = false;

    bool playDeathAnimation = false;

    // fight
    float attack_cooldown;
    float defending_cooldown;
    bool isDefending = false;
    bool isAttacking = false;

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
    float hp = 20.0f;
};

extern Player player;

#endif
