#include <stdbool.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "game"
#define BASE_FONT "font.ttf"
#define TTF_GetError    SDL_GetError

// window
SDL_Window* window;
SDL_Renderer* renderer;

// game
bool isRunning = true;

class Text {
    public:
        SDL_Texture* texture;
        int lenght;
};

Text text;

// tools
SDL_Texture* LoadTexture(const char* path) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path);

    return texture;
}

void RenderTexture(SDL_Texture* texture, float x, float y, float width, float height) {
    SDL_FRect spritePortion = { x, y, width, height };
    SDL_FRect texturePosition = { x, y, width, height };
    if (texture) {
        SDL_RenderTexture(renderer, texture, NULL, &texturePosition);
    }
}

SDL_Texture* createText(const char* text, Uint8 font_size, Uint8 red, Uint8 green, Uint8 blue, Uint8 opacity) {
    SDL_Color color = {red, green, blue, opacity};

    TTF_Font* font = TTF_OpenFont("font.ttf", font_size);
    if (!font) {
        SDL_Log("Failed to load font:", TTF_GetError());
        return NULL;
    }

    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, strlen(text), color);
    if (!textSurface) {
        SDL_Log("Failed to render text surface");
        TTF_CloseFont(font);
        return NULL;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
    }

    SDL_DestroySurface(textSurface);
    TTF_CloseFont(font);

    return textTexture;
}


// main functions

int init() {
    // init sdl
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Init error %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() < 0) {
        SDL_Log("TTF_Init() Error:");
        return -1;
    }

    // init window
    window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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

    const char* mytext = "telhgxt";
    text.lenght = strlen(mytext);

    text.texture = createText(mytext, 48, 255, 255, 0, 255);
    if (!text.texture) {
        return 1;
    }

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

    RenderTexture(text.texture, 0, 0, WINDOW_WIDTH, WINDOW_WIDTH / text.lenght * 2);

    SDL_RenderPresent(renderer);
}

void update(float delta) {
}

void cleanup() {
    SDL_DestroyTexture(text.texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

int main() {
    Uint64 last_tick = 0;
    Uint64 current_tick = 0;
    float delta;

    if (init() != 0) {
        return 1;
    }

    // main loop
    while (isRunning) {
        last_tick = current_tick;
        current_tick = SDL_GetTicks();
        delta = (current_tick - last_tick) / 1000.0f;

        handlerEvents();
        update(delta);
        render();
    }

    // clean up
    cleanup();

    return 0;
}