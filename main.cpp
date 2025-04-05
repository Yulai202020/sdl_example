#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Player {
    public:

        Player(int coord_x, int coord_y) {
            x = coord_x;
            y = coord_y;
        }
        int x, y;

        const static int player_jump = 32;
        const static int player_step = 5;
        const static int player_width = 64;
        const static int player_height = 64;

        void renderTexture(SDL_Renderer* renderer) {
            SDL_Rect destRect = { x, y, player_width, player_height };
            if (playerTexture) {
                SDL_RenderCopy(renderer, playerTexture, NULL, &destRect);
            }
        }

        bool setTexture(SDL_Texture* newTexture) {
            if (!newTexture) {
                std::cout << "Failed to load player texture: " << IMG_GetError() << std::endl;
                return false;
            }
            playerTexture = newTexture;
            return true;
        }

        SDL_Texture* getTexture() {
            return playerTexture;
        }

        void clean() {
            SDL_DestroyTexture(playerTexture);
            playerTexture = nullptr;
        }
    private:
        SDL_Texture* playerTexture = nullptr;
};

class Game {
    public:
        Game(const char* title, int xpos, int ypos, int width, int height, bool fullscreen): window_width(width), window_height(height), 
              player(0, window_height - 64) {
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

            isRunning = true;

            player = Player(0, 0);
            player.setTexture(IMG_LoadTexture(renderer, "assets/player.png"));

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
                            if (player.y == window_height - player.player_height) {
                                player.y -= player.player_jump;
                                player.setTexture(IMG_LoadTexture(renderer, "assets/player_jumping.png"));
                            }
                            break;
                        case SDLK_a:
                            if (player.x >= player.player_step) {
                                player.x -= player.player_step;
                                player.setTexture(IMG_LoadTexture(renderer, "assets/player_running_left.png"));
                            }
                            break;
                        case SDLK_d:
                            if (player.x <= window_width - player.player_width) {
                                player.x += player.player_step;
                                player.setTexture(IMG_LoadTexture(renderer, "assets/player_running_right.png"));
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
            if (player.y < window_height - player.player_height) {
                player.y++;  // simulate gravity

                if (player.y == window_height - player.player_height) {
                    player.setTexture(IMG_LoadTexture(renderer, "assets/player.png"));
                } else {
                    player.setTexture(IMG_LoadTexture(renderer, "assets/player_jumping.png"));
                }
            }
        }

        void render() {
            SDL_RenderClear(renderer);

            // render player
            player.renderTexture(renderer);

            SDL_RenderPresent(renderer);
        }

        void clean() {
            if (player.getTexture()) {
                player.clean();
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
        bool isRunning;
        int window_width;
        int window_height;
        SDL_Window* window;
        SDL_Renderer* renderer;
        Player player;
};

int main(int argc, char* argv[]) {
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

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
