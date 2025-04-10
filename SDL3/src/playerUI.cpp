#include "playerUI.h"
#include "tools.h"
#include "game.h"

#include <string>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

SDL_AppResult PlayerUI::init(float* hp, float max_health) {
    this->hp = hp;
    this->max_health = max_health;

    return SDL_APP_CONTINUE;
};

SDL_AppResult PlayerUI::update(float delta) {
    float hp_procentage = *hp/max_health;
    dest_rect.w = hp_procentage*300;

    SDL_DestroyTexture(hp_bar);
    moneyText = std::to_string((int) *hp) + "/" + std::to_string((int) max_health);
    hp_bar = createText(moneyText.c_str(), 255, 255, 255, 255);

    return SDL_APP_CONTINUE;
}

SDL_AppResult PlayerUI::render() {
    SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(game.renderer, &dest_rect);

    int textWidth = moneyText.length()*50;
    RenderTexture(hp_bar, 10+(300-textWidth)/2, 10, textWidth, 50);

    return SDL_APP_CONTINUE;
}

void PlayerUI::cleanup() {
    SDL_DestroyTexture(hp_bar);
}