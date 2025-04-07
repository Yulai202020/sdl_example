#include <iostream>
#include <stdio.h>
#include <stdbool.h>
#include <cmath>

#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

// window
SDL_Window* window;
SDL_Renderer* renderer;

// entity
const int fallingSpeed = 300;
const int entitySpeed = 100;
const float maxJumpingTime = 0.5f;

// game
const char* title = "Title";

const int window_width = 800;
const int window_height = 600;

bool isRunning = true;

class Player {
    public:
        // texture
        const int width = 64;
        const int hieght = 64;
        SDL_Texture* texture;

        // position
        float x = 0;
        float y = 0;
        float jumpingTime = 0;

        // fight
        float attack_cooldown = 0.0f;
        bool isDefending = false;

        void damage(float damage) {
            hp -= damage;
        }

        bool isDead() {
            return hp <= 0;
        }

        bool isFalling() {
            return y < window_height - width || jumpingTime > 0;
        }
    private:
        float hp = 20.0f;
};

Player player1 = Player();
Player player2 = Player();

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
    window = SDL_CreateWindow(title, window_width, window_height, 0);
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

    player1.texture = LoadTexture("assets/player.png");
    player2.texture = LoadTexture("assets/player.png");
    player2.x = window_height;

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
    RenderTexture(player1.texture, player1.x, player1.y, 64, 64);
    RenderTexture(player2.texture, player2.x, player2.y, 64, 64);

    SDL_RenderPresent(renderer);
}

void update(float delta) {
    const _Bool* keyboard_state = SDL_GetKeyboardState(NULL);

    if (player1.attack_cooldown > 0) {
        player1.attack_cooldown = std::max(0.0f, player1.attack_cooldown - delta);
    }

    if (player2.attack_cooldown > 0) {
        player2.attack_cooldown = std::max(0.0f, player2.attack_cooldown - delta);
    }

    // player1
    if (player1.jumpingTime > 0){
        player1.y -= entitySpeed * delta;
        player1.jumpingTime -= delta;
    } else if (player1.isFalling()) {
        player1.y += fallingSpeed * delta;
    } else if (keyboard_state[SDL_SCANCODE_W]) {
        player1.jumpingTime = maxJumpingTime;
    }

    // moving right and left
    if (keyboard_state[SDL_SCANCODE_A]) {
        player1.x -= entitySpeed*delta;
    }

    if (keyboard_state[SDL_SCANCODE_D]) {
        player1.x += entitySpeed*delta;
    }

    // defending|undefending
    if (keyboard_state[SDL_SCANCODE_LSHIFT]) {
        player1.isDefending = !player1.isDefending;
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

                float damage = std::min(0.0f, 5.0f);
                player2.damage(damage); // damage cant be negative

                if (player2.isDead()) {
                    isRunning = false;
                }
            } else {
                SDL_Log("miss");
            }

            player1.attack_cooldown = 5.0f;
        }
    }

    // player2
    if (player2.jumpingTime > 0){
        player2.y -= entitySpeed * delta;
        player2.jumpingTime -= delta;
    } else if (player2.isFalling()) {
        player2.y += fallingSpeed * delta;
    } else if (keyboard_state[SDL_SCANCODE_W]) {
        player2.jumpingTime = maxJumpingTime;
    }
    // moving right and left
    if (keyboard_state[SDL_SCANCODE_J]) {
        player2.x -= entitySpeed*delta;
    }

    if (keyboard_state[SDL_SCANCODE_L]) {
        player2.x += entitySpeed*delta;
    }

    if (keyboard_state[SDL_SCANCODE_RSHIFT]) {
        player2.isDefending = !player2.isDefending;
    }
}

void cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main() {
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

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