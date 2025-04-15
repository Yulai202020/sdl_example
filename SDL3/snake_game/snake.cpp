#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define CELL_SIZE 30
#define TIME_FOR_MOVE 0.5
#define WINDOW_TITLE "Snake Game"

typedef struct {
    int x;
    int y;
} Coords;

class Tail {
    public:
        Tail(int x, int y): x(x), y(y) {}
        Tail* next = nullptr;
        int x, y;
};

class Player {
    public:
        Player(int x, int y) {
            head = new Tail(x, y);
        }
        bool isgrow = false;
        Tail* head;
};

Player* player = new Player(1, 1);
Coords direction = {1, 0};
Coords nextDirection = {1, 0};
Coords apples[5];

SDL_Window* window;
SDL_Renderer* renderer;

float timer = 0;

int init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Init error %s\n", SDL_GetError());
        return 1;
    }

    // Create window
    window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        SDL_Log("Window error %s\n", SDL_GetError());
        return 1;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Renderer error %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        apples[i].x = rand() % (WINDOW_WIDTH / CELL_SIZE);
        apples[i].y = rand() % (WINDOW_HEIGHT / CELL_SIZE);
    }

    return 0;
}

SDL_AppResult handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_KEY_DOWN:
            switch (event.key.key) {
                case SDLK_ESCAPE:
                    return SDL_APP_SUCCESS;
                case SDLK_W:
                    if (direction.y != 1) {
                        nextDirection = {0, -1};
                    }
                    break;
                case SDLK_S:
                    if (direction.y != -1) {
                        nextDirection = {0, 1};
                    }
                    break;
                case SDLK_A:
                    if (direction.x != 1) {
                        nextDirection = {-1, 0};
                    }
                    break;
                case SDLK_D:
                    if (direction.x != -1) {
                        nextDirection = {1, 0};
                    }
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
    Tail* head = player->head;

    for (int i = 0; i < 5; i++) {
        if (head->x == apples[i].x && head->y == apples[i].y) {
            player->isgrow = true;

            // Respawn the apple somewhere else
            apples[i].x = rand() % (WINDOW_WIDTH / CELL_SIZE);
            apples[i].y = rand() % (WINDOW_HEIGHT / CELL_SIZE);
        }
    }

    if (timer >= TIME_FOR_MOVE) {
        direction = nextDirection;

        int prevX = head->x;
        int prevY = head->y;

        head->x += direction.x;
        head->y += direction.y;

        Tail* current = head->next;

        while (current != nullptr) {
            int tempX = current->x;
            int tempY = current->y;

            current->x = prevX;
            current->y = prevY;

            prevX = tempX;
            prevY = tempY;

            current = current->next;
        }

        if (player->isgrow) {
            Tail* tail = head;
            while (tail->next != nullptr) {
                tail = tail->next;
            }

            tail->next = new Tail(prevX, prevY);
            player->isgrow = false;
        }

        timer = 0;
    }

    if (head->x < 0 || head->x >= WINDOW_WIDTH / CELL_SIZE ||
        head->y < 0 || head->y >= WINDOW_HEIGHT / CELL_SIZE) {
        return SDL_APP_SUCCESS;
    }

    Tail* tail = head->next;
    while (tail != nullptr) {
        if (tail->x == head->x && tail->y == head->y) {
            return SDL_APP_SUCCESS;
        }

        tail = tail->next;
    }

    timer += delta;

    return SDL_APP_CONTINUE;
}

SDL_AppResult render() {
    // here is problem
    SDL_RenderClear(renderer);

    // render head
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    Tail* head = player->head;
    SDL_FRect dest_rect = {head->x*CELL_SIZE, head->y*CELL_SIZE, CELL_SIZE, CELL_SIZE};
    SDL_RenderFillRect(renderer, &dest_rect);

    // render tail
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
    
    Tail* tail = head->next;

    while (tail != nullptr) {
        dest_rect.x = tail->x*CELL_SIZE;
        dest_rect.y = tail->y*CELL_SIZE;

        SDL_RenderFillRect(renderer, &dest_rect);

        tail = tail->next;
    }

    // render apples
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    for (int i = 0; i < 5; i++) {
        SDL_FRect dest_rect = {apples[i].x*CELL_SIZE, apples[i].y*CELL_SIZE, CELL_SIZE, CELL_SIZE};
        SDL_RenderFillRect(renderer, &dest_rect);
    }

    // render background
    SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void cleanup() {
    Tail* current = player->head;
    while (current != nullptr) {
        Tail* next = current->next;
        delete current;
        current = next;
    }

    delete player;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

int main() {
    srand(time(0));

    // Initialize SDL
    if (init()) {
        SDL_Log("Init error %s\n", SDL_GetError());
        return 1;
    }

    int timer = 0;
    bool isRunning = true;

    Uint64 last_tick = 0;
    Uint64 current_tick = 0;
    float delta;

    // main loop
    while (isRunning) {
        last_tick = current_tick;
        current_tick = SDL_GetTicks();
        delta = (current_tick - last_tick) / 1000.0f;

        if (handleEvents() == SDL_APP_SUCCESS) {
            isRunning = false;
            break;
        }

        if (update(delta) == SDL_APP_SUCCESS) {
            isRunning = false;
            break;
        }

        render();
    }

    // cleanup
    cleanup();

    return 0;
}