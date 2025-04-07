#include <stdio.h>
#include <stdbool.h>

#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>


SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;

const char* title = "Title";

const int window_width = 800;
const int window_height = 600;

bool isRunning = true;

// tools

SDL_Texture* LoadTexture(const char* path) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path);

    return texture;
}

void RenderTexture(SDL_Texture* texture, int x, int y, int width, int height) {
    SDL_FRect spritePortion = { x, y, width, height };
    SDL_FRect texturePosition = { x, y, width, height };
    if (texture) {
        SDL_RenderTexture(renderer, texture, NULL, &texturePosition);
    }
}

// 

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

    texture = LoadTexture("assets/player.png");

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
    RenderTexture(texture, 0, 0, 64, 64);

    SDL_RenderPresent(renderer);
}

void update() {

}

void cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main() {
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint64 frameStart;
    int frameTime;

    if (init() != 0) {
        return 1;
    }

    // main loop
    while (isRunning) {
        frameStart = SDL_GetTicks();

        handlerEvents();
        update();
        render();

        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }

    }

    // clean up
    cleanup();

    return 0;
}