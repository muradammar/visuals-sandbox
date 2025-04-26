#include <stdio.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define CoR 1
#define TIMESTEP_MS 250.0
#define WIDTH 900
#define HEIGHT 600

//only 1D coords right now
struct Ball
{
    float pos; 
    float vel;
    float accel;
};

//update balls position and velocity given acceleration
void updatePos(struct Ball* ball) {

    ball->vel = ball->vel + (ball->accel * TIMESTEP_MS/1000);
    ball->pos = ball->pos + (ball->vel * TIMESTEP_MS/1000);

    //check if boundaries are hit
    if (ball->pos <= 0) {

        //reverse position, apply CoR (assuming velocity of fllor is always 0)
        ball->vel = -ball->vel*CoR;
        ball->pos = 0;

    }

}

int main(int argc, char *argv[]) {

    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        printf("Error Initializing SDL");
    }

    SDL_Window* window = SDL_CreateWindow("Bouncy Ball", SDL_WINDOWPOS_CENTERED, 
    SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

    Sleep(10000);

    SDL_Quit();

    struct Ball ball;
    ball.pos = 10;
    ball.vel = 0;
    ball.accel = -9.81;

    while (true) {

        Sleep(TIMESTEP_MS);
        updatePos(&ball);
        printf("%f\n", ball.pos);
        
    }
}