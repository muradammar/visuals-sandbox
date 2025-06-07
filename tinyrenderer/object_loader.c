#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "rasterizer1.h"

#define WIDTH 900
#define HEIGHT 600
#define MAX_VERTS 10000

void load_obj(SDL_Surface* surface, const char* filename, int max_vertices, Uint32 color) {
    FILE* fptr = fopen(filename, "r");

    //error handling
    if (fptr == NULL) {
        printf("Error opening file\n");
        return;
    }

    //printf("file opened");

    Vec3 vertices[max_vertices];
    int vertices_count = 0;

    //read obj file line-by-line
    char line[128];
    while (fgets(line, sizeof(line), fptr)) {

        //read in all points
        if (line[0] == 'v' && line[1] == ' ') {
            Vec3 v;
            sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z);

            vertices[vertices_count] = v;
            vertices_count++;

            //printf("point: %f %f %f\n", v.x, v.y, v.z);
        }

        

        /*
        faces are formatted as such: x/y/z a/b/c d/e/f

        where the first entry in every entity is the vertex of a triangle. 
        the middle entry is the texture if any. and the final entry is the normal
        vector. The final entry should be the same across all entities as they have the same
        normal. 

        ex: f 3//1 7//1 8//1 is triangle 3->7->1 with a normal in the direction
        of normal vector #1
        */

        //read in faces
        else if (line[0] == 'f') {
            int a, b, c;

            sscanf(line, "f %d//%*d %d//%*d %d//%*d", &a, &b, &c);

            //project the 3d points a, b, c onto 2D space and draw the corresp. triangle
            Vec2 p1 = orthographic_proj(vertices[a - 1]);
            Vec2 p2 = orthographic_proj(vertices[b - 1]);
            Vec2 p3 = orthographic_proj(vertices[c - 1]);

            drawTriangle(surface, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color);

            //printf("%f %f %f %f %f %f\n",  p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
        }
    }

    fclose(fptr);
    return;
}

int main(int argc, char *argv[]) {

    if (SDL_Init(SDL_INIT_VIDEO)) {
        printf("Error Initializing SDL: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Object Viewer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Surface* surface = SDL_GetWindowSurface(window);

    //colors
    Uint32 black = SDL_MapRGB(surface->format, 0, 0, 0);
    Uint32 white = SDL_MapRGB(surface->format, 255, 255, 255);
    Uint32 red = SDL_MapRGB(surface->format, 255, 0, 0);

    bool sim_running = true;
    SDL_Event ev1;

    //main loop
    while (sim_running) {

        //printf("Entering main loop\n");

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

            load_obj(surface, "cube.obj", MAX_VERTS, white);

            SDL_UnlockSurface(surface);
        }

        SDL_UpdateWindowSurface(window);

        //1000 ms / 60 fps ~= 16 ms per frame
        SDL_Delay(16);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
