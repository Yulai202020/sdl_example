#ifndef PLAYERUI_H
#define PLAYERUI_H

#include "structs.h"
#include "animation.h"

#include <string>
#include <SDL3/SDL.h>

class PlayerUI {
public:
    SDL_AppResult init(float* hp, float max_health);
    SDL_AppResult render();
    SDL_AppResult update(float delta);
    void cleanup();
private:
    float* hp;
    float max_health;
    SDL_FRect dest_rect = {10, 10, 300, 50};
    SDL_Texture* hp_bar;
    std::string moneyText;
};

#endif
