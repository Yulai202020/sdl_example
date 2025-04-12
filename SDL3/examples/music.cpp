#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <iostream>

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL Init Error: " << SDL_GetError() << std::endl;
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

    Mix_PlayMusic(music, 0);
    SDL_Delay(100000);

    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();
    return 0;
}
