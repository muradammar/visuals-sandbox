#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WIDTH 900
#define HEIGHT 600
#define MAX_VERTS 10000

// Global vars
bool sim_running = true;
SDL_Event ev1;

//3D vector
typedef struct {
    float x, y, z;
} Vec3;

//2D vector
typedef struct {
    float x, y;
} Vec2;

//create an orthographic projection of a 3D point onto a 2D plane
Vec2 orthographic_proj(Vec3 v) {
    Vec2 out;

    // Map [0,1] cube to [-1,1] NDC for correct projection
    v.x = v.x * 2.0f - 1.0f;
    v.y = v.y * 2.0f - 1.0f;

    out.x = (int)((v.x + 1.0f) * 0.5f * WIDTH);
    out.y = (int)(1.0f - ((v.y + 1.0f) * 0.5f) * HEIGHT); // flip y

    return out;
}




//helper: set a pixel on a surface
void set_pixel(SDL_Surface *surface, int x, int y, Uint32 color) {
    if (x < 0 || x >= surface->w || y < 0 || y >= surface->h) return;

    Uint8 *pixel_ptr = (Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;
    *(Uint32 *)pixel_ptr = color;
}

//helper: return the min of 3 ints
int min3(int x, int y, int z) {

    int min;

    if (x <= y) {
        min = x;
    } else {
        min = y;
    }

    if (z <= min) {
        min = z;
    }

    return min;
}

//helper: return the max of 3 ints
int max3(int x, int y, int z) {

    int max;

    if (x >= y) {
        max = x;
    } else {
        max = y;
    }

    if (z >= max) {
        max = z;
    }

    return max;
}

//helper: bitwise XOR variable swapper
void swapXOR(int* x, int *y) {
    *x = *x ^ *y;
    *y = *x ^ *y;
    *x = *x ^ *y;
}


//draws a line given two points in cartesian coords
void line(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint32 color) {
    
    //transpose for steep lines
    bool is_steep = abs(y2 - y1) > abs(x2 - x1);

    if (is_steep) {

        //bitwise XOR variable swap
        swapXOR(&x1, &y1);
        swapXOR(&x2, &y2);
    }

    //ensure always drawing from left to right
    if (x1 > x2) {
        swapXOR(&x1, &x2);
        swapXOR(&y1, &y2);     
    }

    //this loop requires x2 to be greater than x1 (left to right)
    for (int x=x1; x <=x2; x++ ) {

        //possible bug if vertical line
        float t = (x - x1) / (float)(x2 - x1);

        int y = y1 + (y2 - y1) * t;

        //if transposed, reverse
        if (is_steep) {
            set_pixel(surface, y, x, color);

        } else {
            set_pixel(surface, x, y, color);

        }
    }
}

//draw a triangle given 3 cartesian coords without filling it in
void drawTriangle(SDL_Surface* surface, int x1, int y1, int x2, int y2, int x3, int y3, Uint32 color) {

    line(surface, x1, y1, x2, y2, color);
    line(surface, x2, y2, x3, y3, color);
    line(surface, x3, y3, x1, y1, color);

}

//draw a triangle given 3 cartesian coords and fill it in (scanline rendering)
void fillTriangle(SDL_Surface* surface, int x1, int y1, int x2, int y2, int x3, int y3, Uint32 color) {

    //bubble sort algorithm for symmmetry, sort by ascending-y
    //p1 should have smallest y and p3 should have largest y
    if (x1 > x2) { swapXOR(&x1, &x2); swapXOR(&y1, &y2); }
    if (x1 > x3) { swapXOR(&x1, &x3); swapXOR(&y1, &y3); }
    if (x2 > x3) { swapXOR(&x2, &x3); swapXOR(&y2, &y3); }

    line(surface, x1, y1, x2, y2, color);
    line(surface, x2, y2, x3, y3, color);
    line(surface, x3, y3, x1, y1, color);

    /*
    now the line between p1, and p3 (p13) is guaranteed to be the vertically longest straight line.
    a left and right boundary can be formed. one side is p13. the other side is p123. p123 must be broken down into 
    2 different lines (p12 and p23). 
    */

    int total_height = y3 - y1;

    //fill the lower half of the triangle
    if (y1 != y2) {
        int lower_height = y2 - y1;
        
        for (int y=y1 ; y<=y2 ; y++) {

            //sweep the horizontal line
            int ax = x1 + (x3 - x1) * (y - y1) / total_height;
            int bx = x1 + (x2 - x1) * (y - y1) / lower_height;

            line(surface, ax, y, bx, y, color);
        }
    }

    //fill the upper half of the triangle
    if (y2 != y3) {
        int upper_height = y3 - y2;

        for (int y=y2 ; y<=y3 ; y++) {

            //sweep the horizontal line
            int ax = x2 + (x3 - x1) * (y - y1) / total_height;
            int bx = x2 + (x2 - x1) * (y - y1) / upper_height;

            line(surface, ax, y, bx, y, color);
        }
    }

}

//unfinished
// void fillTriangle(SDL_Surface* Surface, int x1, int y1, int x2, int y2, int x3, int y3, Uint32 color); {

//     //create a bbox within which to check pixels
//     int xmin = min3(x1, x2, x3);
//     int ymin = min3(y1, y2, y3);
//     int xmax = max3(x1, x2, x3);
//     int ymax = max3(y1, y2, y3);

//     //iterate through each pixel
//     for (int i=xmin ; i<xmax ; i++ ) {
//         for int( j=ymin ; j<ymax ; j++) {
//             //algorithm to check if in triangle
//         }
//     }
// }

//---------------obj loader---------------------
Vec3 vertices[MAX_VERTS];
int vertex_count = 0;

void load_obj(SDL_Surface* surface, const char* filename, Uint32 color) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Failed to open OBJ file: %s\n", filename);
        return;
    }


    char line[128];
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == 'v' && line[1] == ' ') {

            printf("%c", line[0]);
            Vec3 v;
            sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z);
            vertices[vertex_count++] = v;
        } else if (line[0] == 'f') {
            int a, b, c;
            // Support formats like: f 3//1 7//1 8//1
            sscanf(line, "f %d//%*d %d//%*d %d//%*d", &a, &b, &c);

            Vec2 p1 = orthographic_proj(vertices[a - 1]);
            Vec2 p2 = orthographic_proj(vertices[b - 1]);
            Vec2 p3 = orthographic_proj(vertices[c - 1]);

            drawTriangle(surface, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color);
        }

    }

    fclose(f);
}

//----------------------------------------------

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

    //colors
    Uint32 black = SDL_MapRGB(surface->format, 0, 0, 0);
    Uint32 white = SDL_MapRGB(surface->format, 255, 255, 255);
    Uint32 red = SDL_MapRGB(surface->format, 255, 0, 0);

    //main loop
    while (sim_running) {

        printf("Entering main loop\n");

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


            
            SDL_UnlockSurface(surface);
        }

        fillTriangle(surface, 100, 100, 200, 500, 300, 250, white);
        SDL_UpdateWindowSurface(window);

        //1000 ms / 60 fps ~= 16 ms per frame
        SDL_Delay(16);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
