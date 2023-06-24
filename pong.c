#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600
#define BALL_SIZE 20
#define BALL_SPEEDX 10
#define BALL_SPEEDY 10
#define PADDLE_WIDTH 20
#define PADDLE_HEIGHT 150
#define PADDLE_SPEED BALL_SPEEDY

struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
} state;

SDL_Rect ball;
SDL_Rect paddles[2];
int score[2] = {0, 0};

int checkCollision(SDL_Rect paddle, SDL_Rect ball){
    if ((ball.x <= paddle.x + PADDLE_WIDTH) && 
            (ball.x + BALL_SIZE >= paddle.x) && 
            (ball.y <= paddle.y + PADDLE_HEIGHT) && 
            (ball.y + BALL_SIZE >= paddle.y)) {
        return 1;
    }
    return 0;
}

int checkIntersection(void){
    // Initial points
    int x1 = ball.x + ball.w / 2;
    int y1 = ball.y + ball.h / 2;
    int x2 = ball.x + (SCREEN_WIDTH - ball.x);
    int y2 = ball.y + ball.h / 2 ;
    int x3 = SCREEN_WIDTH - PADDLE_WIDTH;
    int y3 = 0;
    int x4 = SCREEN_WIDTH - PADDLE_WIDTH;
    int y4 = SCREEN_HEIGHT;

    // Calculate intersection point y coordinates
    int intersection_Y = ((x1*y2 - y1*x2) * (y3-y4) - (y1-y2) * (x3*y4 - y3*x4)) / ((x1-x2) * (y3-y4) - (y1-y2) * (x3-x4));

    return intersection_Y;
}


void drawBall(void){
    static int dir_X = 1; // 1: left to right, -1: right to left
    static int dir_Y = 1; // 1: top to bottom, -1: bottom to top

    // Set the initial position of the ball
    if (ball.x == 0 && ball.y == 0) {
        // Not Starting at origin to avoid collision at start
        ball.x = 200;
        ball.y = 200;
    }

    // Set ball dimensions 
    ball.w = BALL_SIZE;
    ball.h = BALL_SIZE;

    // Move ball
    ball.x += BALL_SPEEDX * dir_X;
    ball.y += BALL_SPEEDY * dir_Y;

    //Check if the ball hits on of the paddles
    if (checkCollision(paddles[0], ball) || checkCollision(paddles[1], ball)) {
        dir_X *= -1; // Reverse the direction
    }
    // Check if the ball hits the sides of the screen
    if (ball.x <= 0) {
        dir_X *= -1; // Reverse the direction
        score[1] += 1; // Update score B
    }else if (ball.x >= SCREEN_WIDTH - BALL_SIZE) {
        dir_X *= -1; 
        score[0] += 1; 
    }
    // Check if the ball hits the top or bottom of the screen
    if (ball.y <= 0 || ball.y >= SCREEN_HEIGHT - BALL_SIZE) {
        dir_Y *= -1; 
    }

    // Draw the ball
    SDL_SetRenderDrawColor(state.renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(state.renderer, &ball);
}

void drawPaddles(void){
    // Set initial position of paddle A
    if (paddles[0].x == 0 && paddles[0].y == 0) {
        // Set initial position of paddle A
        paddles[0].x = 0;
        paddles[0].y =0; 
    }
    // Set initial position of paddle B
    if (paddles[1].x == 0 && paddles[1].y == 0) {
        // Set initial position of paddle B
        paddles[1].x = SCREEN_WIDTH - PADDLE_WIDTH;
        paddles[1].y = 0; 
    }

    // Set dimensions of the paddles
    paddles[0].w = PADDLE_WIDTH;
    paddles[0].h = PADDLE_HEIGHT;
    paddles[1].w = PADDLE_WIDTH;
    paddles[1].h = PADDLE_HEIGHT;

    //Controls for paddle A
    const uint8_t *keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_UP]) {
        if (paddles[0].y <= 0) {
            paddles[0].y = 0;
        }else {
            paddles[0].y -= PADDLE_SPEED;
        }
    }
    if (keystate[SDL_SCANCODE_DOWN]) {
        if (paddles[0].y >= SCREEN_HEIGHT - PADDLE_HEIGHT) {
            paddles[0].y = SCREEN_HEIGHT - PADDLE_HEIGHT;
        }else {
            paddles[0].y += PADDLE_SPEED;
        }
    }

    // Automated paddle B
    if (paddles[1].y != checkIntersection()) {
        if (paddles[1].y < checkIntersection()) {
            if (paddles[1].y < SCREEN_HEIGHT - PADDLE_HEIGHT) {
                paddles[1].y += PADDLE_SPEED;
            }
        } else {
            paddles[1].y -= PADDLE_SPEED;
        }
    }

    // Draw paddle A 
    SDL_SetRenderDrawColor(state.renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(state.renderer, &paddles[0]);

    // Draw paddle B
    SDL_SetRenderDrawColor(state.renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(state.renderer, &paddles[1]);
}


int main(void){
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Create window
    state.window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    // Create renderer
    state.renderer = SDL_CreateRenderer(state.window, -1, SDL_RENDERER_ACCELERATED);

    // Main loop
    int quit = 0;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
        SDL_RenderClear(state.renderer);

        // Draw and move ball
        drawBall();

        // Draw and move paddles
        drawPaddles();

        // Update the screen
        SDL_RenderPresent(state.renderer);

        // Delay to control the speed
        SDL_Delay(30);
    }

    // Clean up
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();

    // Display score (TODO: make aesthetic)
    printf("Score A: %d Score B: %d\n", score[0], score[1]);
    return 0;
}

