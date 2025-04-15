#include <SDL3/SDL.h>
#include <cmath>

#define WINDOW_TITLE "SDL3 Oval Example"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

SDL_Window* window;
SDL_Renderer* renderer;

// Draw an oval (ellipse)
void drawOval(SDL_Renderer* renderer, int centerX, int centerY, int radiusX, int radiusY, int segments, bool filled = false) {
    const float step = 2.0f * M_PI / segments;

    if (filled) {
        for (int y = -radiusY; y <= radiusY; ++y) {
            float dy = static_cast<float>(y) / radiusY;
            int lineRadiusX = static_cast<int>(radiusX * sqrt(1 - dy * dy));
            for (int x = -lineRadiusX; x <= lineRadiusX; ++x) {
                SDL_RenderPoint(renderer, centerX + x, centerY + y);
            }
        }
    } else {
        for (float angle = 0; angle < 2 * M_PI; angle += step) {
            int x = static_cast<int>(centerX + radiusX * cos(angle));
            int y = static_cast<int>(centerY + radiusY * sin(angle));
            SDL_RenderPoint(renderer, x, y);
        }
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
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_EVENT_QUIT:
                return SDL_APP_SUCCESS;
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
    drawOval(renderer, 400, 300, 150, 10, 10000, false);

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
