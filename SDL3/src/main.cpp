#include <iostream>
#include <vector>
#include <cmath>

#include "game.h"
#include <stdbool.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

int main() {
    Uint64 last_tick = 0;
    Uint64 current_tick = 0;
    float delta;

    // main loop
    while (game.isRunning) {
        last_tick = current_tick;
        current_tick = SDL_GetTicks();
        delta = (current_tick - last_tick) / 1000.0f;

        game.handleEvents();
        game.update(delta);
        game.render();
    }

    // clean up
    game.cleanup();

    return 0;
}