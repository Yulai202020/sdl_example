#include <SDL3/SDL.h>
#include <cmath>
#include <iostream>

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

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL3 Oval Example", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // Draw filled oval
        SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
        drawOval(renderer, 400, 300, 150, 10, 10000, false);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
