#include "player.h"
#include "tools.h"
#include "game.h"
#include "structs.h"
#include "animation.h"

#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>

void Player::damage(float damage) {
    hp -= damage;
}

bool Player::isDead() {
    return hp <= 0;
}

float Player::gethp() {
    return hp;
}

bool Player::isFalling() {
    return y < (WINDOW_HEIGHT - PLAYER_HITBOX_HEIGHT) || jumpingTime > 0;
}

SDL_AppResult Player::init() {
    texture = LoadTexture("assets/player.png");

    current_frame = 0;
    frame_timer = 0.0f;

    y = WINDOW_HEIGHT - PLAYER_HITBOX_HEIGHT;
    x = 0;

    animations.idle = { 6, 100, 0 };
	animations.walk = { 6, 100, 1 };
	animations.attack_horizontal = { 6, 100, 2 };
	animations.attack_down = {6, 100, 4};
	animations.attack_top = { 6, 100, 6 };

    return SDL_APP_CONTINUE;
}

SDL_AppResult Player::render() {
    animation.showAnimation(animations.idle, src_rect, PLAYER_TEXTURE_WIDTH);

    if (isFalling()) {
    } else if (isDefending) {
    } else {
    }

    RenderTexture_rect(texture, x, y, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT, &src_rect);

    return SDL_APP_CONTINUE;
}

SDL_AppResult Player::update(float delta) {
    const _Bool* keyboard_state = SDL_GetKeyboardState(NULL);

    // Handle attacking cooldown
    if (attack_cooldown > 0) {
        attack_cooldown = std::max(0.0f, attack_cooldown - delta);
    }

    // Handle defending cooldown
    if (defending_cooldown > 0) {
        defending_cooldown = std::max(0.0f, defending_cooldown - delta);
    }

    // Check if the player is falling
    if (isFalling()) {
        isDefending = false;
        defending_cooldown = MAX_DEFENDING_COOLDOWN;
    }

    // Handle jumping movement
    if (jumpingTime > 0) {
        y -= ENTITY_SPEED * delta;
        jumpingTime -= delta;
    } else if (isFalling()) {
        y += FALLING_SPEED * delta;
    } else if (keyboard_state[SDL_SCANCODE_W]) {
        jumpingTime = MAX_JUMPING_TIME;
    }

    // Handle left/right movement
    if (keyboard_state[SDL_SCANCODE_A]) {
        x -= ENTITY_SPEED * delta;
    }

    if (keyboard_state[SDL_SCANCODE_D]) {
        x += ENTITY_SPEED * delta;
    }

    // Toggle defending state with shift key if not in cooldown
    if (keyboard_state[SDL_SCANCODE_LSHIFT] && defending_cooldown == 0.0f) {
        isDefending = !isDefending;
        defending_cooldown = MAX_DEFENDING_COOLDOWN;
    }

    frame_timer += delta;
    if (frame_timer >= animations.idle.animationDelay / 1000.0f) {
        current_frame = (current_frame + 1) % animations.idle.frames;
        frame_timer = 0.0f;
    }

    return SDL_APP_CONTINUE;
}

void Player::cleanup() {
    SDL_DestroyTexture(texture);
    // SDL_DestroyTexture(texture_idle);
    // SDL_DestroyTexture(texture_jumping);
    // SDL_DestroyTexture(texture_defending);
}