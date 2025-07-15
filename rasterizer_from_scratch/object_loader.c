/*
gcc object_loader.c rasterizer1.c transformer.c -IC:\SDL2\include -LC:\SDL2\lib -lmingw32 -lSDL2main -lSDL2 -o object.exe
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "rasterizer1.h"
#include "transformer.h"

#define WIDTH 900
#define HEIGHT 600
#define MAX_VERTS 10000

typedef struct {

    int vertices_count;
    int faces_count;

    Vec3* vertices;
    int (*faces)[4]; //3 vertex indices, 1 face normal index
    Vec3* face_normals;

} Object;

//reads a .obj file into an Object struct
Object load_obj(const char* filename, int max_vertices) {

    Object obj = {0, 0};

    //open the .obj file for reading
    FILE* fptr = fopen(filename, "r");

    if (fptr == NULL) {
        printf("Error opening file\n");
        return obj;
    }

    //first get the number of vertices and faces
    char line[128];
    while (fgets(line, sizeof(line), fptr)) {

        if(line[0] == 'v' && line[1] == ' ') {
            obj.vertices_count++;
        }

        if(line[0] == 'f') {
            obj.faces_count++;
        }
    }

    //dynamically resize objects arrays
    obj.vertices = (Vec3*)malloc(obj.vertices_count * sizeof(Vec3));
    obj.faces = malloc(obj.faces_count * sizeof(int[4]));
    obj.face_normals = (Vec3*)malloc(obj.faces_count * sizeof(Vec3));

    //re-read the file this time actually reading in the data
    rewind(fptr);
    int vertex_counter = 0;
    int face_counter = 0;
    int face_normal_counter = 0;
    while (fgets(line, sizeof(line), fptr)) {

        if (line[0]=='v' && line[1] == ' ') {
            Vec3 v;
            sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z);
            obj.vertices[vertex_counter++] = v;
        }

        else if (line[0] == 'v' && line[1] == 'n') {
            Vec3 v;
            sscanf(line, "vn %f %f %f", &v.x, &v.y, &v.z);
            obj.face_normals[face_normal_counter++] = v;
        }

        else if (line[0] == 'f') {
            int a, b, c, d;
            sscanf(line, "f %d/%*d/%d %d/%*d/%*d %d/%*d/%*d", &a, &d, &b, &c);
            sscanf(line, "f %d//%d %d//%*d %d//%*d", &a, &d, &b, &c);
            obj.faces[face_counter][0] = a - 1;
            obj.faces[face_counter][1] = b - 1;
            obj.faces[face_counter][2] = c - 1;
            obj.faces[face_counter][3] = d - 1;
            face_counter++;
        }
    }

    // for (int i=0 ; i<obj.faces_count ; i++) {
    //     printf("%f %f %f\n", obj.face_normals[obj.faces[i][4]].x, obj.face_normals[obj.faces[i][4]].y, obj.face_normals[obj.faces[i][4]].z);
    // }

    return obj;
}



void drawObj(SDL_Surface* surface, Object obj, Uint32 line_color, Uint32 fill_color, Vec3 view) {

    
    // Draw all stored faces
    for (int i = 0; i < obj.faces_count; i++) {
        //only draw if facing camera
        if (backface_culling(obj.face_normals[obj.faces[i][3]], view)) {
            int a = obj.faces[i][0];
            int b = obj.faces[i][1];
            int c = obj.faces[i][2];

            // Vec2 p1 = orthographic_proj(obj.vertices[a]);
            // Vec2 p2 = orthographic_proj(obj.vertices[b]);
            // Vec2 p3 = orthographic_proj(obj.vertices[c]);

            Vec3 camera_pos = {0, 0, 2};
            Vec2 p1 = perspective_proj(obj.vertices[a], camera_pos);
            Vec2 p2 = perspective_proj(obj.vertices[b], camera_pos);
            Vec2 p3 = perspective_proj(obj.vertices[c], camera_pos);

            // drawTriangle(surface, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, line_color);
            fillTriangle(surface, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, fill_color, line_color);
            }
        }
    
    for (int i=0 ; i<obj.vertices_count ; i++) {
        rotate(&obj.vertices[i], 0, 1, 0);
    }

    for (int i=0 ; i<obj.faces_count ; i++) {

        int v_idx1 = obj.faces[i][0];
        int v_idx2 = obj.faces[i][1];
        int v_idx3 = obj.faces[i][2];
        int fn_idx = obj.faces[i][3];

        adjust_face(obj.vertices[v_idx1], obj.vertices[v_idx2], obj.vertices[v_idx3], &obj.face_normals[fn_idx]);
        normalize(&obj.face_normals[fn_idx]);
    }
}

//     for (int i=0 ; i<obj.faces_count ; i++) {

//         int v_idx1 = obj.faces[i][0];
//         int v_idx2 = obj.faces[i][1];
//         int v_idx3 = obj.faces[i][2];
//         int fn_idx = obj.faces[i][3];

//         rotate_x(&obj.vertices[v_idx1], 1);
//         rotate_x(&obj.vertices[v_idx2], 1);
//         rotate_x(&obj.vertices[v_idx3], 1);

        // adjust_face(obj.vertices[v_idx1], obj.vertices[v_idx2], obj.vertices[v_idx3], &obj.face_normals[fn_idx]);
        // normalize(&obj.face_normals[fn_idx]);
//     }
// }

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

    Vec3 view = {0.0, 0.0, -1.0};

    Object obj1 = load_obj("cube.obj", MAX_VERTS);
    printf("object loaded");

    float z_buffer[HEIGHT][WIDTH];

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

            // Fill black and init z_buffer
            for (int y = 0; y < HEIGHT; y++) {
                for (int x = 0; x < WIDTH; x++) {
                    set_pixel(surface, x, y, black);
                }
            }

            drawObj(surface, obj1, white, black, view);

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
