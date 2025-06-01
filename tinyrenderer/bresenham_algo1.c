#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WIDTH 900
#define HEIGHT 600

// Global vars
bool sim_running = true;
SDL_Event ev1;

//Colors


// Helper: set a pixel on a surface
void set_pixel(SDL_Surface *surface, int x, int y, Uint32 color) {
    if (x < 0 || x >= surface->w || y < 0 || y >= surface->h) return;

    Uint8 *pixel_ptr = (Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;
    *(Uint32 *)pixel_ptr = color;
}

void line(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint32 color) {
    
    //transpose for steep lines
    bool is_steep = abs(y2 - y1) > abs(x2 - x1);

    if (is_steep) {

        //bitwise XOR variable swap
        y1 = x1 ^ y1;
        x1 = x1 ^ y1;
        y1 = x1 ^ y1;

        y2 = x2 ^ y2;
        x2 = x2 ^ y2;
        y2 = x2 ^ y2;
    }

    //what is this?
    if (x1 > x2) {
        y1 = y1 ^ y2;
        y2 = y1 ^ y2;
        y1 = y1 ^ y2;

        x1 = x1 ^ x2;
        x2 = x1 ^ x2;
        x1 = x1 ^ x2;       
    }


    for (int x=x1; x <=x2; x++ ) {

        float t = (x - x1) / (float)(x2 - x1);

        int y = y1 + (y2 - y1) * t;

        //if transposed, reverse
        if (is_steep) {
            printf("trying to print");
            set_pixel(surface, y, x, color);

        } else {
            set_pixel(surface, x, y, color);

        }
    }
}

int main(int argc, char *argv[]) {

    if (SDL_Init(SDL_INIT_VIDEO)) {
        printf("Error Initializing SDL: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Bouncy Ball",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Surface* surface = SDL_GetWindowSurface(window);

    //Colors
    Uint32 black = SDL_MapRGB(surface->format, 0, 0, 0);
    Uint32 white = SDL_MapRGB(surface->format, 255, 255, 255);
    Uint32 red = SDL_MapRGB(surface->format, 255, 0, 0);

    // Main loop
    while (sim_running) {
        // Event polling
        while (SDL_PollEvent(&ev1)) {
            if (ev1.type == SDL_QUIT) {
                sim_running = false;
            }
        }

        // Lock surface before manipulating pixels
        if (SDL_LockSurface(surface) == 0) {

            // Fill black
            for (int y = 0; y < HEIGHT; y++) {
                for (int x = 0; x < WIDTH; x++) {
                    set_pixel(surface, x, y, black);
                }
            }

            //Draw
            line(surface, 100, 500, 150, 100, white);
            line(surface, 100, 100, 500, 200, white);
            

            SDL_UnlockSurface(surface);
        }

        SDL_UpdateWindowSurface(window);

        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
