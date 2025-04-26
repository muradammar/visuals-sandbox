#include <stdio.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define CoR 0.9
#define TIMESTEP_MS 200
#define WIDTH 900
#define HEIGHT 600
#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0x00000000
#define COLOR_GREY 0xefefefef

/*
this code models a bouncing ball with basic kinematics applied in timesteps. 
Non-elastic behaviour can be modelled by tweaking the coefficient of restitution (CoR)*/

//only 1D coords right now
struct Ball
{
    //position
    float xpos; 
    float ypos;

    float xvel;
    float yvel;

    float xaccel;
    float yaccel;

    float radius;
};

//update balls position and velocity given acceleration
void updatePos(struct Ball* ball) {

    ball->xvel = ball->xvel + (ball->xaccel * TIMESTEP_MS/1000);
    ball->xpos = ball->xpos + (ball->xvel * TIMESTEP_MS/1000);

    ball->yvel = ball->yvel + (ball->yaccel * TIMESTEP_MS/1000);
    ball->ypos = ball->ypos + (ball->yvel * TIMESTEP_MS/1000);

    //check if boundaries are hit
    if (ball->xpos <= 0) {

        //reverse position, apply CoR (assuming velocity of fllor is always 0)
        ball->xvel = -ball->xvel*CoR;
        ball->xpos = 0;

    }

    if (ball->ypos <= 0) {

        //reverse position, apply CoR (assuming velocity of fllor is always 0)
        ball->yvel = -ball->yvel*CoR;
        ball->ypos = 0;

    }
}

void drawCircle(SDL_Surface* surface, struct Ball ball, Uint32 color) {

    for (double x = ball.xpos - ball.radius; x < ball.xpos + ball.radius; x++) {
        for (double y = ball.ypos - ball.radius; y < ball.ypos + ball.radius; y++) {
            
            double dist_from_center = (x - ball.xpos) * (y - ball.ypos);

            //fill in all pixels within the radius of the ball
            if (dist_from_center <= pow(ball.radius, 2)) {

                SDL_Rect pixel = (SDL_Rect){x, y, 1, 1};
                SDL_FillRect(surface, &pixel, color);
            }
        }
    }
}

//global vars
bool sim_running = true;
SDL_Event ev1;

int main(int argc, char *argv[]) {

    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        printf("Error Initializing SDL");
    }

    SDL_Window* window = SDL_CreateWindow("Bouncy Ball", SDL_WINDOWPOS_CENTERED, 
    SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    SDL_Rect erase_rect = {0, 0, WIDTH, HEIGHT};

    struct Ball ball;
    ball.xpos = 450;
    ball.ypos = 500;
    ball.xvel = 0;
    ball.yvel = 0;
    ball.xaccel = 0;
    ball.yaccel = -9.81;
    ball.radius = 50;

    while ( sim_running ) {

        //close window and stop running sem when window closed
        while ( SDL_PollEvent ( &ev1 ) != 0) {
            if (ev1.type == SDL_QUIT) {\

                sim_running = false;
                SDL_Quit();
                break;
            }
        }

        //Sleep(TIMESTEP_MS);
        updatePos(&ball); 
        SDL_UpdateWindowSurface(window);
        SDL_FillRect(surface, &erase_rect, COLOR_BLACK);
        drawCircle(surface, ball, COLOR_WHITE);
        SDL_Delay(20);
        printf("%f\n", ball.ypos);
        
    }
}