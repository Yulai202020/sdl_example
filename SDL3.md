# sdl3

## init

```c
// Initialize SDL
if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_Log("Init error %s\n", SDL_GetError());
    return;
}

// Create window
window = SDL_CreateWindow(title, width, height, flags | SDL_WINDOW_RESIZABLE);
if (!window) {
    SDL_Log("Window error %s\n", SDL_GetError());
    return;
}

// Create renderer
renderer = SDL_CreateRenderer(window, NULL);
if (!renderer) {
    SDL_Log("Renderer error %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return;
}
```

## render somethink

```c
// clear window
SDL_RenderClear(renderer);

// set color drawing
SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black

SDL_RenderFillRect(renderer, SDL_FRect\*) // render and fill rect
SDL_RenderRect(renderer, SDL_FRect\*) // render rect without filling (just border)

SDL_RenderLine(render, x1, y1, x2, y2) // render line

SDL_RenderLines(render, SDL_FPoints[], int) // render lines

SDL_RenderPoint(render, x, y) // render point
SDL_RenderPointx(render, SDL_FPoints[], int) // render points

// render geometry

SDL_Vertex vertex[3]; // triangle

vertex[0].position = {100, 100}; // vertex at 100 100
vertex[0].color = {1, 0, 0, 1};

vertex[1].position = {200, 200}; // vertex at 200 200
vertex[1].color = {0, 1, 0, 1};

vertex[2].position = {50, 200}; // vertex at 50 200
vertex[2].color = {0, 0, 1, 1};

SDL_RenderGeometry(render, NULL, vertex, 3, NULL, 0);

// here its means change background color to white
SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white

// draw anythink
SDL_RenderPresent(renderer);
```

## cleanup

```c
// delete renderer and window
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);

// quit sdl
SDL_Quit();
```

# ttf

## create font

## cleanup

```c
// delete font
TTF_CloseFont(TTF_Font*);

// quit ttf
TTF_Quit();
```

# image

## cleanup

```c
SDL_DestroyTexture(SDL_Texture*)
```
