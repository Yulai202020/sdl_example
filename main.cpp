#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>


class Player {
    public:
        Player(int coord_x, int coord_y) {
            x = coord_x;
            y = coord_y;
        }

        int getPlayerWidth() {
            return player_width;
        }

        int getPlayerHeight() {
            return player_height;
        }

        int x, y;
    private:
        int player_width = 64;
        int player_height = 64;
};

class Game {
    public:
    Game(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
        int flags = 0;
        window_width = width;
        window_height = height;

        if (fullscreen) {
            flags = SDL_WINDOW_FULLSCREEN;
        }

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
            isRunning = false;
            return;
        }

        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            std::cout << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
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

        playerTexture = IMG_LoadTexture(renderer, "assets/player.png");
        if (!playerTexture) {
            std::cout << "Failed to load player texture: " << IMG_GetError() << std::endl;
            isRunning = false;
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
                    case SDLK_w:
                        if (player_y_coord >= window_height - player_height) {
                            player_y_coord -= player_jump;
                        }
                        break;
                    case SDLK_a:
                        if (player_x_coord >= player_step) {
                            player_x_coord -= player_step;
                        }
                        break;  
                    case SDLK_d:
                        if (player_x_coord <= window_width - player_width) {
                            player_x_coord += player_step;
                        }
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

        // physics
        if (player_y_coord < window_height - player_height) {
            player_y_coord++;
        }
    }

    void render() {
        SDL_RenderClear(renderer);

        // render anythink
        SDL_Rect destRect = { player_x_coord, player_y_coord, player_width, player_height };
        if (playerTexture) {
            SDL_RenderCopy(renderer, playerTexture, NULL, &destRect);
        }

        SDL_RenderPresent(renderer);
    }

    void clean() {
        if (playerTexture) {
            SDL_DestroyTexture(playerTexture);
            playerTexture = nullptr;
        }

        if (renderer) {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }

        if (window) {
            SDL_DestroyWindow(window);
            window = nullptr;
        }

        IMG_Quit();
        SDL_Quit();
    }

    bool running() {
        return isRunning;
    }

    private:
    int cnt = 0;
    bool isRunning;
    int window_width;
    int window_height;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* playerTexture = nullptr;

    int player_step = 5;
    int player_jump = 32;
    int player_x_coord = 0;
    int player_y_coord = 0;
    int player_height = 64;
    int player_width = 64;
};

int main(int argc, char* argv[]) {
    const int FPS = 60;
    const int frameDelay = 1000/FPS;

    Uint64 frameStart;
    int frameTime;

    int window_width = 800;
    int window_height = 600;
    bool isFullscreen = false;

    Game game("game", 0, 0, window_width, window_height, isFullscreen);

    while (game.running()) {
        frameStart = SDL_GetTicks();

        game.handleEvents();
        game.update();
        game.render();

        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    game.clean();

    return 0;
}
