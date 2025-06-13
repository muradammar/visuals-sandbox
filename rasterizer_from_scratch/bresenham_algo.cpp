#include <stdio.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WIDTH 900
#define HEIGHT 600

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

    while ( sim_running ) {

        //close window and stop running sem when window closed
        while ( SDL_PollEvent ( &ev1 ) != 0) {
            if (ev1.type == SDL_QUIT) {

                sim_running = false;
                SDL_Quit();
                break;
            }
        }
    }
}