#ifndef RASTERIZER1
#define RASTERIZER1

#include <SDL2/SDL.h>


typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    float x, y;
} Vec2;

Vec2 orthographic_proj(Vec3 v);
void set_pixel(SDL_Surface *surface, int x, int y, Uint32 color);
void line(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint32 color);
void drawTriangle(SDL_Surface* surface, int x1, int y1, int x2, int y2, int x3, int y3, Uint32 color);
void fillTriangle(SDL_Surface* surface, int x1, int y1, int x2, int y2, int x3, int y3, Uint32 color);
void load_obj(SDL_Surface* surface, const char* filename, int max_vertices, Uint32 color);

#endif
