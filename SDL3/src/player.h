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


#define PLAYER_HITBOX_WIDTH 152
#define PLAYER_HITBOX_HEIGHT 152

#define FALLING_SPEED 300
#define ENTITY_SPEED 100
#define MAX_JUMPING_TIME 0.5f
#define MAX_ATTACKING_COOLDOWN 2.5f
#define MAX_DEFENDING_COOLDOWN 0.5f

class Player {
public:
    SDL_Texture* texture = nullptr;
    SDL_FRect src_rect = {0, 0, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT};
    PlayerAnimationData animations;
    Animation animation;

    int current_frame = 0;
    float frame_timer = 0.0f;

    // position
    float x = 0;
    float y = 0;
    float jumpingTime = 0;

    // fight
    float attack_cooldown;
    float defending_cooldown;
    bool isDefending = false;

    void damage(float damage);
    bool isDead();
    float gethp();
    bool isFalling();

    SDL_AppResult init();
    SDL_AppResult render();
    SDL_AppResult update(float delta);
    void cleanup();

private:
    float hp = 20.0f;
};

extern Player player;

#endif
