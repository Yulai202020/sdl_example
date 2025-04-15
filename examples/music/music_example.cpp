#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <iostream>

#define WINDOW_TITLE "music"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define LOOPS -1

SDL_Window* window;
SDL_Renderer* renderer;

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

int main(int argc, char* argv[]) {
    if (init()) {
        return 1;
    }

    if (init_mixer()) {
        return 1;
    }

    Mix_Music* music = Mix_LoadMUS("music.mp3");
    if (!music) {
        std::cerr << "Mix_LoadMUS Error: " << SDL_GetError() << std::endl;
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    Mix_PlayMusic(music, LOOPS);

    float time = 0;
    bool isMusicRunning = true;
    bool isRunning = true;
    const float musicLength = Mix_MusicDuration(music);

    Mix_SetMusicPosition(time);

    Uint64 last_tick = 0;
    Uint64 current_tick = 0;
    float delta;

    while (isRunning) {
        last_tick = current_tick;
        current_tick = SDL_GetTicks();
        delta = (current_tick - last_tick) / 1000.0f;

        SDL_Event event;
        SDL_PollEvent(&event);

        switch (event.type) {
            case SDL_EVENT_QUIT:
                isRunning = false;
                break;
            case SDL_EVENT_KEY_DOWN:
                switch (event.key.key) {
                    case SDLK_SPACE:
                        if (isMusicRunning) {
                            Mix_HaltMusic();
                        } else {
                            Mix_PlayMusic(music, 0);
                        }

                        isMusicRunning = !isMusicRunning;
                        break;
                    case SDLK_RIGHT:
                        time += 10;
                        break;
                    case SDLK_LEFT:
                        time -= 10;
                        break;
                    default:
                        break;
                }

                Mix_SetMusicPosition(time);
                break;
            default:
                break;
        }

        if (time < 0) {
            time = 0;
        }

        if (time > musicLength) {
            time = musicLength;
        }

        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
        SDL_RenderPresent(renderer);

        if (isMusicRunning) {
            time += delta;
        }
    }

    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();
    return 0;
}
