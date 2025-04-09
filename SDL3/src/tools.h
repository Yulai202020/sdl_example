#ifndef TOOLS_H
#define TOOLS_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "game.h"

SDL_Texture* LoadTexture(const char* path);
void RenderTexture(SDL_Texture* texture, float x, float y, float width, float height);
void RenderTexture_rect(SDL_Texture* texture, int x, int y, int width, int height, SDL_FRect* src_rect, SDL_FlipMode flip);
#endif
