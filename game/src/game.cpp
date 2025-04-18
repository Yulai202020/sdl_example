#include "game.h"
#include "player.h"
#include "entities.h"
#include "tools.h"

#include <cmath>
#include <vector>
#include <algorithm>

#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

Player player = Player();
SDL_Texture* death_texture;
std::vector<Entity*> entities;
TTF_Font* font;

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

    if (TTF_Init() < 0) {
        return;
    }

    font = TTF_OpenFont("assets/fonts/Tiny5.ttf", 48);
    if (!font) {
        SDL_Log("Failed to load font:", SDL_GetError());
        return;
    }

    // Create window
    window = SDL_CreateWindow(title, width, height, flags | SDL_WINDOW_RESIZABLE);
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
    death_texture = LoadTexture("assets/death.png");
    initEntities(&entities);
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

    player.handleEvents(&event);
}

void Game::render() {
    SDL_RenderClear(renderer);

    player.render();

    for (Entity* entity : entities) {
        entity->render();
    }

    SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
    SDL_RenderPresent(renderer);
}

void Game::update(float delta) {
    SDL_AppResult result = player.update(delta);

    if (result == SDL_APP_SUCCESS) {
        isRunning = false;
    } else if (result == SDL_APP_CONTINUE) {
    }

    for (Entity* entity : entities) {
        entity->update(delta);
    }
}

void Game::cleanup() {
    player.cleanup();

    for (Entity* entity : entities) {
        entity->cleanup();
        delete entity;
    }

    SDL_DestroyTexture(death_texture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_Quit();
}

Game game = Game(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, false);