#include "player.h"
#include "tools.h"
#include "game.h"
#include "structs.h"
#include "entities.h"
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

SDL_AppResult Player::init() {
    texture = LoadTexture("assets/player.png");

    y = WINDOW_HEIGHT - PLAYER_TEXTURE_HEIGHT;
    x = 0;

    animations.idle = { 6, 100, 0 };
	animations.walk = { 6, 100, 1 };
	animations.attack_horizontal = { 6, 100, 2 };
	animations.attack_down = { 6, 100, 4 };
	animations.attack_top = { 6, 100, 6 };

    animations.death = { 7, 150, 1 };

    return SDL_APP_CONTINUE;
}

SDL_AppResult Player::handleEvents(SDL_Event* event) {
    switch (event->type) {
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            switch (event->button.button) {
                case SDL_BUTTON_LEFT:
                    isAttacking = true;
                    isDefending = false;
                    animationHandler.setCurrentIndex(0);
                    break;
                case SDL_BUTTON_RIGHT:
                    isDefending = true;
                    isAttacking = false;
                    animationHandler.setCurrentIndex(0);
                    break;
                default:
                    break;
            }
            break;

        case SDL_EVENT_MOUSE_BUTTON_UP:
            switch (event->button.button) {
                case SDL_BUTTON_LEFT:
                    isAttacking = false;
                    break;
                case SDL_BUTTON_RIGHT:
                    isDefending = false;
                    break;
                default:
                    break;
            }
            break;

        default:
            break;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult Player::update(float delta) {
    if (!isDead()) {
        const bool* keyboard_state = SDL_GetKeyboardState(NULL);
        isWalking = false;

        // Handle attacking cooldown
        if (attack_cooldown > 0) {
            attack_cooldown = std::max(0.0f, attack_cooldown - delta);
        }

        // Handle defending cooldown
        if (defending_cooldown > 0) {
            defending_cooldown = std::max(0.0f, defending_cooldown - delta);
        }

        // Handle jumping movement
        if (keyboard_state[SDL_SCANCODE_W]) {
            isWalking = true;
            y -= ENTITY_SPEED * delta;
        }

        if (keyboard_state[SDL_SCANCODE_S]) {
            isWalking = true;
            y += ENTITY_SPEED * delta;
        }

        // Handle left/right movement
        if (keyboard_state[SDL_SCANCODE_A]) {
            flip = SDL_FLIP_HORIZONTAL;
            isWalking = true;
            x -= ENTITY_SPEED * delta;
        }

        if (keyboard_state[SDL_SCANCODE_D]) {
            flip = SDL_FLIP_NONE;
            isWalking = true;
            x += ENTITY_SPEED * delta;
        }

        if (isWalking) {
            animationHandler.showAnimation(animations.walk, src_rect, PLAYER_TEXTURE_WIDTH);
        } else if (isAttacking) {
            animationHandler.showAnimation(animations.attack_horizontal, src_rect, PLAYER_TEXTURE_WIDTH);

            if (animationHandler.currentIndex == animations.attack_horizontal.frames - 1) {
                Entity* entity = getNearestEntity(x, y, entities);

                if (entity && !entity->isDead) {
                    bool isLookingRight = flip == SDL_FLIP_NONE;
                    if ((isLookingRight && x < entity->x) || (!isLookingRight && x > entity->x) || (x == entity->x)) {
                        float damage = 50.0f*delta;
                        entity->Damage(x + PLAYER_TEXTURE_WIDTH/2 , y + PLAYER_TEXTURE_HEIGHT/2, damage); // count from center of player texture
                    }
                }
            }
        } else {
            animationHandler.showAnimation(animations.idle, src_rect, PLAYER_TEXTURE_WIDTH);
        }
    } else {
        isWalking = false;
        isAttacking = false;
        isDefending = false;
        playDeathAnimation = true;
    }

    if (playDeathAnimation) {
        int rc = animationHandler.showAnimationOneTime(animations.death, src_rect, PLAYER_TEXTURE_WIDTH);

        if (rc) {
            playDeathAnimation = false;

            return SDL_APP_SUCCESS;
        }
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult Player::render() {
    if (!isDead()) {
        RenderTexture_rect(texture, x, y, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT, &src_rect, flip);
    }

    if (playDeathAnimation) {
        RenderTexture_rect(death_texture, x, y, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT, &src_rect, SDL_FLIP_NONE);
    }

    return SDL_APP_CONTINUE;
}

void Player::cleanup() {
    SDL_DestroyTexture(texture);
}