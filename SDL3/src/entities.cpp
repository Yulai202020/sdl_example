#include <vector>
#include <algorithm>
#include <cmath>

#include "structs.h"
#include "animation.h"
#include "entities.h"
#include "tools.h"
#include "game.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

float Entity::getDistance(int attacker_x, int attacker_y) const {
    int x_delta = attacker_x - (x + ENTITY_TEXTURE_WIDTH/2);
    int y_delta = attacker_y - (y + ENTITY_TEXTURE_HEIGHT/2);

    float c = std::sqrt(x_delta * x_delta + y_delta * y_delta);

    return c;
}

bool Entity::Damage(int attacker_x, int attacker_y, float damage) {
    float distance = getDistance(attacker_x, attacker_y);
    
    if (distance <= 100) {
        hp -= damage;

        if (hp <= 0) {
            playDeathAnimation = true;
            isDead = true;
            animationHandler.setCurrentIndex(0);
        }
        return true;
    } else {
        return false;
    }
}

SDL_AppResult Entity::init() {
    texture = LoadTexture("assets/sheep.png");

    animations.idle = { 8, 100, 0 };
    animations.death = { 7, 150, 1 };

    return SDL_APP_CONTINUE;
}

SDL_AppResult Entity::update(float delta) {
    if (!isDead) {
        animationHandler.showAnimation(animations.idle, src_rect, ENTITY_TEXTURE_WIDTH);
    }

    if (playDeathAnimation) {
        int rc = animationHandler.showAnimationOneTime(animations.death, src_rect, ENTITY_TEXTURE_WIDTH);

        if (rc) {
            playDeathAnimation = false;
        }
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult Entity::render() {
    if (!isDead) {
        RenderTexture_rect(texture, x, y, ENTITY_TEXTURE_WIDTH, ENTITY_TEXTURE_HEIGHT, &src_rect, SDL_FLIP_NONE);
    }

    if (playDeathAnimation) {   
        RenderTexture_rect(death_texture, x, y, ENTITY_TEXTURE_WIDTH, ENTITY_TEXTURE_HEIGHT, &src_rect, SDL_FLIP_NONE);
    }

    return SDL_APP_CONTINUE;
}

void Entity::cleanup() {
    SDL_DestroyTexture(texture);
}

Entity* getNearestEntity(int x, int y, const std::vector<Entity*>& entities) {
    float min_dist = std::numeric_limits<float>::max();
    Entity* nearestEntity = nullptr;

    for (Entity* entity : entities) {
        if (entity->isDead) {
            continue;
        }

        float distance = entity->getDistance(x, y);
        if (distance < min_dist) {
            min_dist = distance;
            nearestEntity = entity;
        }
    }

    return nearestEntity;
}

void initEntities(std::vector<Entity*>* entities) {
    Entity* entity = new Entity(20.0f, 100, 100);
    entity->init();
    entities->push_back(entity);
}