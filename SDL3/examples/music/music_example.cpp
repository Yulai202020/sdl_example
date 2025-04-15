#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <iostream>

#define LOOPS -1

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("music", 800, 600, 0);
    if (!window) {
        SDL_Log("Window error %s\n", SDL_GetError());
        return 1;
    }

    // init renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Window error %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_AudioSpec audioSpec;
    SDL_zero(audioSpec);
    audioSpec.freq = 44100;              // Set th frequency to 44100 Hz
    audioSpec.format = MIX_DEFAULT_FORMAT; // Use the default audio format (usually PCM)
    audioSpec.channels = 2;              // Stereo sound (2 channels)

    if (Mix_OpenAudio(0, &audioSpec) < 0) {  // Use 0 for the default device
        std::cerr << "Mix_OpenAudio Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
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
    const int musicLength = Mix_MusicDuration(music);

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
                if (event.key.key == SDLK_SPACE) {
                    if (isMusicRunning) {
                        Mix_HaltMusic();
                    } else {
                        Mix_PlayMusic(music, 0);
                    }

                    isMusicRunning = !isMusicRunning;
                }

                if (event.key.key == SDLK_RIGHT) {
                    time += 10;
                }

                if (event.key.key == SDLK_LEFT) {
                    time -= 10;
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
