#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int window_width = 800;
    int window_height = 600;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Create an SDL window
    SDL_Window* window = SDL_CreateWindow("SDL2 Draw Rectangle with WASD Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);  // Use software renderer
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Set the drawing color to black for the background (RGBA format)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black color

    // Define the initial position of the rectangle
    int rect_width = 100;
    int rect_height = 100;
    SDL_Rect rect = { 150, 100, rect_width, rect_height };

    // Event loop: wait until the user closes the window
    SDL_Event e;
    int quit = 0;
    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }

            // Handle key press events
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_w: // Move up
                        if (rect.y >= 10) {
                            rect.y -= 10;
                        }
                        break;
                    case SDLK_a: // Move left
                        if (rect.x >= 10) {
                            rect.x -= 10;
                        }
                        break;
                    case SDLK_s: // Move down
                        if (rect.y <= window_height - rect_height - 10) {
                            rect.y += 10;
                        }
                        break;
                    case SDLK_d: // Move right
                        if (rect.x <= window_width - rect_width - 10) {
                            rect.x += 10;
                        }
                        break;
                }
            }
        }

        // Clear the screen with black color
        SDL_RenderClear(renderer);

        // Set the drawing color to red (RGBA format) for the filled rectangle
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red color for the filled rectangle
        SDL_RenderFillRect(renderer, &rect);  // Draw filled rectangle

        // Set the drawing color to blue (RGBA format) for the outline
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // Blue color for the outline
        SDL_RenderDrawRect(renderer, &rect);  // Draw rectangle outline

        // Present the renderer (to display the rectangles)
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
