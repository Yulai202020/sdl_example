#include <SDL3/SDL.h>
#include <cmath>
#include <vector>

#define WINDOW_TITLE "SDL3 Oval Example"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

SDL_Window* window;
SDL_Renderer* renderer;

void drawOval(SDL_Renderer* renderer, int cx, int cy, int rx, int ry, int segments, bool filled = false) {
    const float step = 2.0f * M_PI / segments;

    if (filled) {
        for (int y = -ry; y <= ry; y++) {
            int dx = (int)(rx * sqrt(1.0 - (double)(y * y) / (ry * ry)));
            SDL_RenderLine(renderer, cx - dx, cy + y, cx + dx, cy + y);
        }
    } else {
        SDL_FPoint points[segments+1];

        for (int i = 0; i < segments + 1; ++i) {
            float angle = i * step;

            float x = cx + rx * cosf(angle);
            float y = cy + ry * sinf(angle);

            points[i] = {x, y};
        }

        SDL_RenderLines(renderer, points, segments+1);
    }
}

int init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    return 0;
}

SDL_AppResult handleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                return SDL_APP_SUCCESS;
                break;
            case SDL_EVENT_KEY_DOWN:
                switch (event.key.key) {
                    case SDLK_ESCAPE:
                        return SDL_APP_SUCCESS;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult update() {
    return SDL_APP_CONTINUE;
}

SDL_AppResult render() {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    // Draw filled oval
    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
    drawOval(renderer, 400, 300, 150, 100, 1, true);

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (init()) {
        return 1;
    }

    bool isRunning = true;

    while (isRunning) {
        if (handleEvents() != SDL_APP_CONTINUE) {
            isRunning = false;
        }

        if (update() != SDL_APP_CONTINUE) {
            isRunning = false;
        }

        if (render() != SDL_APP_CONTINUE) {
            isRunning = false;
        };
    }

    cleanup();
    return 0;
}
