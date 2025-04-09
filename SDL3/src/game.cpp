#include "game.h"
#include "player.h"
#include "tools.h"

#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

Player player = Player();

Game::Game(const char* title, int width, int height, bool fullscreen) {
    int flags = 0;

    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Init error %s\n", SDL_GetError());
        return;
    }

    // Create window
    window = SDL_CreateWindow(title, width, height, flags);
    if (!window) {
        SDL_Log("Window error %s\n", SDL_GetError());
        return;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Renderer error %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Initialize player
    player.init();
}

void Game::handleEvents() {
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

void Game::render() {
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    player.render();

    SDL_RenderPresent(renderer);
}

void Game::update(float delta) {
    SDL_AppResult result = player.update(delta);

    if (result == SDL_APP_SUCCESS) {
        isRunning = false;
    } else if (result == SDL_APP_CONTINUE) {
    }
}

void Game::cleanup() {
    player.cleanup();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

Game game = Game(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, false);