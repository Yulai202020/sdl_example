#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <algorithm>
#include <iostream>
#include <stdexcept>

#define WINDOW_TITLE "music"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define LOOPS -1

SDL_Window* window;
SDL_Renderer* renderer;
Mix_Music* currentMusic;

float musicTime = 0.0f;
bool isMusicRunning = true;

Mix_Music* loadMusic(const char* path) {
    Mix_Music* music = Mix_LoadMUS(path);

    if (!music) {
        SDL_Log("Mix_LoadMUS Error: %s", SDL_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return NULL;
    }

    return music;
}

float getMusicLength(Mix_Music* music) {
    return Mix_MusicDuration(music);
}

float getMusicTime(Mix_Music* music) {
    return Mix_GetMusicPosition(music);
}

int init() {
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL Init error %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Log("Window error %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Window error %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    return 0;
}

int init_mixer() {
    SDL_AudioSpec audioSpec;
    SDL_zero(audioSpec);
    audioSpec.freq = 44100;
    audioSpec.format = MIX_DEFAULT_FORMAT;
    audioSpec.channels = 2;

    if (Mix_OpenAudio(0, &audioSpec) < 0) {
        SDL_Log("Mix_OpenAudio Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    return 0;
}

SDL_AppResult handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
            break;
        case SDL_EVENT_KEY_DOWN:
            switch (event.key.key) {
                case SDLK_ESCAPE:
                    return SDL_APP_SUCCESS;
                    break;
                case SDLK_SPACE:
                    if (isMusicRunning) {
                        Mix_HaltMusic();
                    } else {
                        Mix_PlayMusic(currentMusic, 0);
                    }

                    isMusicRunning = !isMusicRunning;
                    Mix_SetMusicPosition(musicTime);
                    break;
                case SDLK_RIGHT:
                    musicTime += 10;
                    Mix_SetMusicPosition(musicTime);
                    break;
                case SDLK_LEFT:
                    musicTime -= 10;
                    Mix_SetMusicPosition(musicTime);
                    break;
                default:
                    break;
            }

            break;
        default:
            break;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult update(float delta) {
    musicTime = getMusicTime(currentMusic);

    return SDL_APP_CONTINUE;
}

SDL_AppResult render() {
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void cleanup() {
    Mix_FreeMusic(currentMusic);
    Mix_CloseAudio();
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (init()) {
        return 1;
    }

    if (init_mixer()) {
        return 1;
    }

    currentMusic = loadMusic("music.mp3");
    if (!currentMusic) {
        return 1;
    }

    Mix_PlayMusic(currentMusic, LOOPS);

    bool isRunning = true;

    Mix_SetMusicPosition(musicTime);

    Uint64 last_tick = 0;
    Uint64 current_tick = 0;
    float delta;

    while (isRunning) {
        last_tick = current_tick;
        current_tick = SDL_GetTicks();
        delta = (current_tick - last_tick) / 1000.0f;

        if (handleEvents() != SDL_APP_CONTINUE) {
            isRunning = false;
        }

        if (update(delta) != SDL_APP_CONTINUE) {
            isRunning = false;
        }

        if (render() != SDL_APP_CONTINUE) {
            isRunning = false;
        }
    }

    cleanup();

    return 0;
}
