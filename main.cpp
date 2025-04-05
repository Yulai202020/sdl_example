#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

class Game {
    public:
    Game(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
        int flags = 0;
        if (fullscreen) {
            flags = SDL_WINDOW_FULLSCREEN;
        }

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
            isRunning = false;
            return;
        }

        window = SDL_CreateWindow("SDL2 Draw Rectangle with WASD Example", xpos, ypos, width, height, flags);
        if (window == NULL) {
            std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer == NULL) {
            std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            return;
        }

        isRunning = true;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // white
    }

    void handleEvents() {
        SDL_Event event;
        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
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

    void update() {
        cnt++;
        // std::cout << cnt << std::endl;
    }

    void render() {
        SDL_RenderClear(renderer);

        // render anythink

        SDL_RenderPresent(renderer);
    }

    void clean() {
        if (renderer) {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }

        if (window) {
            SDL_DestroyWindow(window);
            window = nullptr;
        }

        SDL_Quit();
    }

    bool running() {
        return isRunning;
    }

    private:
    int cnt = 0;
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;
};

int main(int argc, char* argv[]) {
    int window_width = 800;
    int window_height = 600;
    bool isFullscreen = false;

    Game game("title", 0, 0, window_width, window_height, isFullscreen);

    while (game.running()) {
        game.handleEvents();
        game.update();
        game.render();
        std::cout<< game.running() << std::endl;
    }

    game.clean();

    return 0;
}
