#include <iostream>
#include <vector>
#include <cmath>

#include <stdbool.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#define FALLING_SPEED 300
#define ENTITY_SPEED 100
#define MAX_JUMPING_TIME 0.5f
#define MAX_ATTACKING_COOLDOWN 2.5f
#define MAX_DEFENDING_COOLDOWN 0.5f

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "game"
#define BASE_FONT "font.ttf"

// window
SDL_Window* window;
SDL_Renderer* renderer;

// game
bool isRunning = true;

class Player {
    public:
        // texture
        const int width = 64;
        const int hieght = 64;
        SDL_Texture* texture;
        SDL_Texture* texture_idle;
        SDL_Texture* texture_jumping;
        SDL_Texture* texture_defending;

        // position
        float x = 0;
        float y = 0;
        float jumpingTime = 0;

        // fight
        float attack_cooldown = 0.0f;
        float defending_cooldown = 0.0f;
        bool isDefending = false;

        void damage(float damage) {
            hp -= damage;
        }

        bool isDead() {
            return hp <= 0;
        }

        float gethp() {
            return hp;
        }

        bool isFalling() {
            return y < WINDOW_HEIGHT - width || jumpingTime > 0;
        }
    private:
        float hp = 20.0f;
};

Player player1 = Player();
Player player2 = Player();

std::vector<Player*> players = { &player1, &player2 };

// tools

SDL_Texture* LoadTexture(const char* path) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path);

    return texture;
}

void RenderTexture(SDL_Texture* texture, float x, float y, float width, float height) {
    SDL_FRect spritePortion = { x, y, width, height };
    SDL_FRect texturePosition = { x, y, width, height };
    if (texture) {
        SDL_RenderTexture(renderer, texture, NULL, &texturePosition);
    }
}

// main functions

int init() {
    // init sdl
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Init error %s\n", SDL_GetError());
        return 1;
    }

    // init window
    window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        SDL_Log("Window error %s\n", SDL_GetError());
        return 1;
    }

    // init renderer
    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Window error %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    for (Player* player : players) {
        player->texture_idle = LoadTexture("assets/player.png");
        player->texture_jumping = LoadTexture("assets/player_jumping.png");
        player->texture_defending = LoadTexture("assets/player_defending.png");
    }

    player1.texture = player1.texture_idle;
    player1.y = WINDOW_HEIGHT - player1.hieght;

    player2.texture = player1.texture_idle;
    player2.x = WINDOW_WIDTH - player2.width;
    player2.y = WINDOW_HEIGHT - player2.hieght;

    return 0;
}

void handlerEvents() {
    SDL_Event event;

    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_EVENT_QUIT:
            isRunning = false;
            break;
        case SDL_EVENT_KEY_DOWN:
            switch (event.key.key) {
                case SDLK_ESCAPE:
                    isRunning = false;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void render() {
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (Player* player : players) {
        if (player->isFalling()) {
            player->texture = player->texture_jumping;
        } else if (player->isDefending) {
            player->texture = player->texture_defending;
        } else {
            player->texture = player->texture_idle;
        }
    }

    RenderTexture(player1.texture, player1.x, player1.y, 64, 64);
    RenderTexture(player2.texture, player2.x, player2.y, 64, 64);

    SDL_RenderPresent(renderer);
}

void update(float delta) {
    const _Bool* keyboard_state = SDL_GetKeyboardState(NULL);

    // attacking cooldown
    for (Player* player : players) {
        if (player->attack_cooldown > 0) {
            player->attack_cooldown = std::max(0.0f, player->attack_cooldown - delta);
        }

        if (player->defending_cooldown > 0) {
            player->defending_cooldown = std::max(0.0f, player->defending_cooldown - delta);
        }

        if (player->isFalling()) {
            player->isDefending = false;
            player->defending_cooldown = MAX_DEFENDING_COOLDOWN;
        }
    }

    // player1
    if (player1.jumpingTime > 0){
        player1.y -= ENTITY_SPEED * delta;
        player1.jumpingTime -= delta;
    } else if (player1.isFalling()) {
        player1.y += FALLING_SPEED * delta;
    } else if (keyboard_state[SDL_SCANCODE_W]) {
        player1.jumpingTime = MAX_JUMPING_TIME;
    }

    // moving right and left
    if (keyboard_state[SDL_SCANCODE_A]) {
        player1.x -= ENTITY_SPEED*delta;
    }

    if (keyboard_state[SDL_SCANCODE_D]) {
        player1.x += ENTITY_SPEED*delta;
    }

    // defending|undefending
    if (keyboard_state[SDL_SCANCODE_LSHIFT] && player1.defending_cooldown == 0.0f) {
        player1.isDefending = !player1.isDefending;
        player1.defending_cooldown = MAX_DEFENDING_COOLDOWN;
    }

    // attack
    if (keyboard_state[SDL_SCANCODE_LCTRL]) {
        player1.isDefending = false;

        if (!player2.isDefending && player1.attack_cooldown == 0.0f) {
            float x_delta = player2.x-player1.x;
            float y_delta = player2.y-player1.y;

            float c = std::sqrt(x_delta*x_delta + y_delta*y_delta);

            if (c < 40 + player1.width) {
                SDL_Log("hit");

                if (player1.jumpingTime > 0) {
                    player2.damage(1.0f);
                }

                float damage = std::max(0.0f, 5.0f);
                player2.damage(damage); // damage cant be negative

                if (player2.isDead()) {
                    isRunning = false;
                }
            } else {
                SDL_Log("miss");
            }

            player1.attack_cooldown = MAX_ATTACKING_COOLDOWN;
        }
    }

    // player2
    if (player2.jumpingTime > 0){
        player2.y -= ENTITY_SPEED * delta;
        player2.jumpingTime -= delta;
    } else if (player2.isFalling()) {
        player2.y += FALLING_SPEED * delta;
    } else if (keyboard_state[SDL_SCANCODE_I]) {
        player2.jumpingTime = MAX_JUMPING_TIME;
    }

    // moving right and left
    if (keyboard_state[SDL_SCANCODE_J]) {
        player2.x -= ENTITY_SPEED*delta;
    }

    if (keyboard_state[SDL_SCANCODE_L]) {
        player2.x += ENTITY_SPEED*delta;
    }

    if (keyboard_state[SDL_SCANCODE_RSHIFT] && player2.defending_cooldown == 0.0f) {
        player2.isDefending = !player2.isDefending;
        player2.defending_cooldown = 1.0f;
    }

    if (keyboard_state[SDL_SCANCODE_RCTRL]) {
        player2.isDefending = false;

        if (!player1.isDefending && player2.attack_cooldown == 0.0f) {
            float x_delta = player1.x-player2.x;
            float y_delta = player1.y-player2.y;

            float c = std::sqrt(x_delta*x_delta + y_delta*y_delta);

            if (c < 40 + player2.width) {
                SDL_Log("hit");

                if (player2.jumpingTime > 0) {
                    player1.damage(1.0f);
                }

                float damage = std::max(0.0f, 5.0f);
                player1.damage(damage); // damage cant be negative

                if (player1.isDead()) {
                    isRunning = false;
                }
            } else {
                SDL_Log("miss");
            }

            player2.attack_cooldown = 5.0f;
        }
    }
}

void cleanup() {
    for (Player* player : players) {
        SDL_DestroyTexture(player->texture_idle);
        SDL_DestroyTexture(player->texture_jumping);
        SDL_DestroyTexture(player->texture_defending);
        SDL_DestroyTexture(player->texture);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main() {
    Uint64 last_tick = 0;
    Uint64 current_tick = 0;
    float delta;

    if (init() != 0) {
        return 1;
    }

    // main loop
    while (isRunning) {
        last_tick = current_tick;
        current_tick = SDL_GetTicks();
        delta = (current_tick - last_tick) / 1000.0f;

        handlerEvents();
        update(delta);
        render();
    }

    // clean up
    cleanup();

    return 0;
}