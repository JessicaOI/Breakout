#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_keyboard.h>
#include <SDL_render.h>
#include <SDL_scancode.h>
#include <string>
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int MAX_FPS = 60;
const int BALL_SPEED = 120;
const int BALL_SIZE = 13;
const int PADDLE_WIDTH = 100;
const int PADDLE_HEIGHT = 20;
const int PADDLE_SPEED = 200;

struct Rect {
    SDL_Rect rect = {0, 0, BALL_SIZE, BALL_SIZE};
    int vx = BALL_SPEED;
    int vy = BALL_SPEED;
    SDL_Color color = {0xFF, 0x00, 0x00, 0xFF}; // Cambiado a rojo
};

Rect ball = {{0, 0, BALL_SIZE, BALL_SIZE}, BALL_SPEED, BALL_SPEED, {0xFF, 0x00, 0x00, 0xFF}};
SDL_Rect paddle = {SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2, SCREEN_HEIGHT - PADDLE_HEIGHT - 10, PADDLE_WIDTH, PADDLE_HEIGHT};

void renderRect(SDL_Renderer* renderer, Rect& rect) {
    SDL_SetRenderDrawColor(renderer, rect.color.r, rect.color.g, rect.color.b, rect.color.a);
    SDL_RenderFillRect(renderer, &rect.rect);
}

void renderPaddle(SDL_Renderer* renderer, SDL_Rect& paddle) {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF); // Blanco
    SDL_RenderFillRect(renderer, &paddle);
}

void renderText(SDL_Renderer* renderer, const char* message, int x, int y) {
    // Render each character as a simple rectangle (this is a placeholder)
    // In a real scenario, you could load a bitmap font and render it
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF); // Blanco
    int char_width = 8;
    int char_height = 16;
    for (const char* p = message; *p; ++p) {
        SDL_Rect r = {x, y, char_width, char_height};
        SDL_RenderFillRect(renderer, &r);
        x += char_width + 2; // Move to the next character position
    }
}

void update(float dT, bool& gameOver) {
    // Rebote en los bordes de la pantalla
    if (ball.rect.x < 0 || ball.rect.x + ball.rect.w > SCREEN_WIDTH) {
        ball.vx *= -1;
    }
    if (ball.rect.y < 0) {
        ball.vy *= -1;
    }

    // Si la pelota toca la parte inferior de la pantalla
    if (ball.rect.y + ball.rect.h > SCREEN_HEIGHT) {
        if (SDL_HasIntersection(&ball.rect, &paddle)) {
            ball.vy *= -1;
        } else {
            gameOver = true;
        }
    }

    ball.rect.x += ball.vx * dT;
    ball.rect.y += ball.vy * dT;
}

void handleInput(float dT) {
    const Uint8* ks = SDL_GetKeyboardState(NULL);

    if (ks[SDL_SCANCODE_LEFT]) {
        paddle.x -= PADDLE_SPEED * dT;
    }
    if (ks[SDL_SCANCODE_RIGHT]) {
        paddle.x += PADDLE_SPEED * dT;
    }

    if (paddle.x < 0) paddle.x = 0;
    if (paddle.x > SCREEN_WIDTH - PADDLE_WIDTH) paddle.x = SCREEN_WIDTH - PADDLE_WIDTH;
}

int main() {
    SDL_SetMainReady(); // Call this to properly initialize SDL in non-default environments
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Bouncing Ball with Paddle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Error creating window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Error creating renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    bool quit = false;
    bool gameOver = false;
    SDL_Event e;

    Uint32 frameStartTimestamp, frameEndTimestamp, lastFrameTime = SDL_GetTicks(), lastUpdateTime = 0;
    float frameDuration = (1.0f / MAX_FPS) * 1000.0f;
    int FPS = MAX_FPS;

    while (!quit) {
        frameStartTimestamp = SDL_GetTicks();

        // delta time
        Uint32 currentFrameTime = SDL_GetTicks();
        float dT = (currentFrameTime - lastFrameTime) / 1000.0f;
        lastFrameTime = currentFrameTime;

        // poll events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // handle input
        if (!gameOver) {
            handleInput(dT);
        }

        // update
        if (!gameOver) {
            update(dT, gameOver);
        }

        // render
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        renderRect(renderer, ball);
        renderPaddle(renderer, paddle);

        if (gameOver) {
            renderText(renderer, "Perdiste", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 12);
        }

        SDL_RenderPresent(renderer);

        frameEndTimestamp = SDL_GetTicks();
        float actualFrameDuration = frameEndTimestamp - frameStartTimestamp;

        if (actualFrameDuration < frameDuration) {
            SDL_Delay(frameDuration - actualFrameDuration);
        }

        // fps calculation
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastUpdateTime > 1000) {
            FPS = static_cast<int>((1.0f / actualFrameDuration) * 1000.0f);
            SDL_SetWindowTitle(window, ("FPS: " + std::to_string(FPS)).c_str());
            lastUpdateTime = currentTime;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
