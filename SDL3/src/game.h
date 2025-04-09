#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "entities.h"
#include "tools.h"

#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "game"

class Game {
public:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    bool isRunning = true;

    Game(const char* title, int width, int height, bool fullscreen);
    void handleEvents();
    void render();
    void update(float delta);
    void cleanup();
};

// Declare the game object as external
extern Game game;

extern Player player;
extern std::vector<Entity*> entities;
extern SDL_Texture* death_texture;

#endif // GAME_H
