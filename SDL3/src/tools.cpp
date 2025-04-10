#include "tools.h"
#include "game.h"

#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

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

SDL_Texture* createText(const char* text, Uint8 red, Uint8 green, Uint8 blue, Uint8 opacity) {
    SDL_Color color = {red, green, blue, opacity};

    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, strlen(text), color);
    if (!textSurface) {
        SDL_Log("Failed to render text surface");
        TTF_CloseFont(font);
        return NULL;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(game.renderer, textSurface);
    if (!textTexture) {
        SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
    }

    SDL_DestroySurface(textSurface);

    return textTexture;
}