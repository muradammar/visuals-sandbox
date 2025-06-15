#ifndef RASTERIZER1
#define RASTERIZER1

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    float x, y;
} Vec2;

Vec2 orthographic_proj(Vec3 v);
Vec2 perspective_proj(Vec3 v, Vec3 camera_pos);
bool backface_culling(Vec3 face_normal, Vec3 view);
void set_pixel(SDL_Surface *surface, int x, int y, Uint32 color);
void line(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint32 color);
void drawTriangle(SDL_Surface* surface, int x1, int y1, int x2, int y2, int x3, int y3, Uint32 color);
void fillTriangle(SDL_Surface* surface, int x1, int y1, int x2, int y2, int x3, int y3, Uint32 fill_color, Uint32 line_color);


#endif
