#include "tools.h"
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "game.h"

SDL_Texture* LoadTexture(const char* file) {
    SDL_Texture* texture = IMG_LoadTexture(game.renderer, file);

    if (!texture) {
        SDL_Log("Unable to load texture %s: %s", file, SDL_GetError());
    }

    return texture;
}

void RenderTexture(SDL_Texture* texture, float x, float y, float width, float height) {
    SDL_FRect texturePosition = { x, y, width, height };

    if (texture) {
        SDL_RenderTexture(game.renderer, texture, NULL, &texturePosition);
    } else {
        SDL_Log("Error: Texture is null!");
    }
}

void RenderTexture_rect(SDL_Texture* texture, int x, int y, int width, int height, SDL_FRect* src_rect, SDL_FlipMode flip) {
    // Create a destination rectangle (where to render on the screen)
    SDL_FRect dest_rect = { x, y, width, height };

    SDL_RenderTextureRotated(game.renderer, texture, src_rect, &dest_rect, 0, NULL, flip);
}