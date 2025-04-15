#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <math.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "3D Cube (SDL3 + OpenGL)"

SDL_Window* window;
SDL_GLContext glContext;

float angle = 0.f;
float scale = 1.f;

void drawCube(float angle, float scale) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.f, 0.f, -5.f);
    glRotatef(angle, -0.5f, -1.f, -.5f);
    glScalef(scale, scale, scale);

    glBegin(GL_QUADS);

    // Front face (red)
    glColor3f(1.f, 0.f, 0.f);
    glVertex3f(-1.f, -1.f,  1.f);
    glVertex3f( 1.f, -1.f,  1.f);
    glVertex3f( 1.f,  1.f,  1.f);
    glVertex3f(-1.f,  1.f,  1.f);

    // Back face (green)
    glColor3f(0.f, 1.f, 0.f);
    glVertex3f(-1.f, -1.f, -1.f);
    glVertex3f(-1.f,  1.f, -1.f);
    glVertex3f( 1.f,  1.f, -1.f);
    glVertex3f( 1.f, -1.f, -1.f);

    // Top face (blue)
    glColor3f(0.f, 0.f, 1.f);
    glVertex3f(-1.f,  1.f, -1.f);
    glVertex3f(-1.f,  1.f,  1.f);
    glVertex3f( 1.f,  1.f,  1.f);
    glVertex3f( 1.f,  1.f, -1.f);

    // Bottom face (yellow)
    glColor3f(1.f, 1.f, 0.f);
    glVertex3f(-1.f, -1.f, -1.f);
    glVertex3f( 1.f, -1.f, -1.f);
    glVertex3f( 1.f, -1.f,  1.f);
    glVertex3f(-1.f, -1.f,  1.f);

    // Right face (cyan)
    glColor3f(0.f, 1.f, 1.f);
    glVertex3f( 1.f, -1.f, -1.f);
    glVertex3f( 1.f,  1.f, -1.f);
    glVertex3f( 1.f,  1.f,  1.f);
    glVertex3f( 1.f, -1.f,  1.f);

    // Left face (magenta)
    glColor3f(1.f, 0.f, 1.f);
    glVertex3f(-1.f, -1.f, -1.f);
    glVertex3f(-1.f, -1.f,  1.f);
    glVertex3f(-1.f,  1.f,  1.f);
    glVertex3f(-1.f,  1.f, -1.f);

    glEnd();
}

int init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize: %s", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Log("Window could not be created: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        SDL_Log("OpenGL context could not be created: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    return 0;
}

void init_gl() {
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float) WINDOW_WIDTH / WINDOW_HEIGHT;
    glFrustum(-aspect, aspect, -1.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
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

SDL_AppResult update(float delta) {
    const bool* keyboard_state = SDL_GetKeyboardState(NULL);

    if (keyboard_state[SDL_SCANCODE_A]) {
        angle += 90.0f * delta;
        scale += 1.0f * delta;
    }

    if (keyboard_state[SDL_SCANCODE_D]) {
        angle -= 90.0f * delta;
        scale -= 1.0f * delta;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult render() {
    drawCube(angle, scale);
    SDL_GL_SwapWindow(window);

    return SDL_APP_CONTINUE;
}

void cleanup() {
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (init()) {
        return 1;
    }

    init_gl();

    bool isRunning = true;
    Uint64 lastTicks = SDL_GetTicks();

    while (isRunning) {
        Uint64 currentTicks = SDL_GetTicks();
        float deltaTime = (currentTicks - lastTicks) / 1000.0f;
        lastTicks = currentTicks;

        if (handleEvents() != SDL_APP_CONTINUE) {
            isRunning = false;
        }

        if (update(deltaTime) != SDL_APP_CONTINUE) {
            isRunning = false;
        }

        if (render() != SDL_APP_CONTINUE) {
            isRunning = false;
        }
    }

    cleanup();
    return 0;
}
