#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class TextureManager {
    public:
        static SDL_Texture* LoadTexture(const char* path, SDL_Renderer* renderer) {
            SDL_Surface* tmpSurface = IMG_Load(path);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
            SDL_FreeSurface(tmpSurface);

            return texture;
        }

        static void RenderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y, int width, int height) {
            SDL_Rect destRect = { x, y, width, height };
            if (texture) {
                SDL_RenderCopy(renderer, texture, NULL, &destRect);
            }
        }
};

class Map {
    public:
        Map(SDL_Renderer* renderer) {
            grass = TextureManager::LoadTexture("assets/grass.png", renderer);
            dirt = TextureManager::LoadTexture("assets/dirt.png", renderer);
            water = TextureManager::LoadTexture("assets/water.png", renderer);

            renderer = renderer;
        }

        void LoadMap() {

        }

        void DrawMap() {

        }
    private:
        SDL_Rect src, dst;

        SDL_Texture* grass; // 0
        SDL_Texture* dirt;  // 1
        SDL_Texture* water; // 2

        SDL_Renderer* renderer;

        int map[20][25];
};

class Player {
    public:
        int x, y;

        const static int jump_height = 32;
        const static int step_width = 5;
        const static int width = 64;
        const static int height = 64;

        SDL_Texture* texture = nullptr;

        Player(int coord_x, int coord_y) {
            x = coord_x;
            y = coord_y;
        }

        void renderTexture(SDL_Renderer* renderer) {
            TextureManager::RenderTexture(texture, renderer, x, y, width, height);
        }

        void clean() {
            if (texture) {
                SDL_DestroyTexture(texture);
                texture = nullptr;
            }
        }
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

            map = Map(renderer);

            player = Player(0, 0);
            player.texture = TextureManager::LoadTexture("assets/player.png", renderer);

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
                        case SDLK_w || SDLK_SPACE:
                            if (player.y == window_height - player.height) {
                                player.y -= player.jump_height;
                            }
                            break;
                        case SDLK_a:
                            if (player.x >= player.step_width) {
                                player.x -= player.step_width;
                                player.texture = TextureManager::LoadTexture("assets/player_running_left.png", renderer);
                            }
                            break;
                        case SDLK_d:
                            if (player.x <= window_width - player.width) {
                                player.x += player.step_width;
                                player.texture = TextureManager::LoadTexture("assets/player_running_right.png", renderer);
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
            if (player.y < window_height - player.height) {
                player.y++;  // simulate gravity

                if (player.y == window_height - player.height) {
                    player.texture = TextureManager::LoadTexture("assets/player.png", renderer);
                } else {
                    player.texture = TextureManager::LoadTexture("assets/player_jumping.png", renderer);
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
            player.clean();

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
        Map map;
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
