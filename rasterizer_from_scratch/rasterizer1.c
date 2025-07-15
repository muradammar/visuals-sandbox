/*
this file contains the functionality for drawing and filling triangles onto
a 2D screen from 3D points.

- an algorithm similar to Bresenham's algorithm is used for line drawing
- scanline rendering is used to fill triangles
- orthographic projections are used to map the 3D points onto the 2D screen
*/

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

//3D vector
typedef struct {
    float x, y, z;
} Vec3;

//2D vector
typedef struct {
    float x, y;
} Vec2;

typedef struct {

    int vertices_count;
    int faces_count;

    Vec3* vertices;
    int (*faces)[4]; //3 vertex indices, 1 face normal index
    Vec3* face_normals;

} Object;

//create an orthographic projection of a 3D point onto a 2D plane
Vec2 orthographic_proj(Vec3 v) {
    Vec2 out;

    out.x = (int)((150.0 * v.x) + WIDTH / 2);
    out.y = (int)((HEIGHT / 2) - (150.0 * v.y));

    return out;
}

//assumes the camera is somewhere in +z looking toward -z
Vec2 perspective_proj(Vec3 v, Vec3 camera_pos) {

    Vec3 v_proj;

    v_proj.x = v.x * (camera_pos.z  / (camera_pos.z + v.z));
    v_proj.y = v.y * (camera_pos.z  / (camera_pos.z + v.z));
    v_proj.z = 0;

    //scaled point can be orthographically projected
    return orthographic_proj(v_proj);
}

//helper: set a pixel on a surface
void set_pixel(SDL_Surface *surface, int x, int y, Uint32 color) {
    if (x < 0 || x >= surface->w || y < 0 || y >= surface->h) return;

    Uint8 *pixel_ptr = (Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;
    *(Uint32 *)pixel_ptr = color;
}

bool backface_culling(Vec3 face_normal, Vec3 view) {

    //compute dot product of two vectors
    float dot_prod = (face_normal.x * view.x) + (face_normal.y * view.y) + (face_normal.z * view.z);
    bool is_facing = dot_prod < 0.0;

    return is_facing;
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
void fillTriangle(SDL_Surface* surface, int x1, int y1, int x2, int y2, int x3, int y3, Uint32 fill_color, Uint32 line_color) {

    //bubble sort algorithm for symmmetry, sort by ascending-y
    //p1 should have smallest y and p3 should have largest y
    if (y1 > y2) { swapXOR(&x1, &x2); swapXOR(&y1, &y2); }
    if (y1 > y3) { swapXOR(&x1, &x3); swapXOR(&y1, &y3); }
    if (y2 > y3) { swapXOR(&x2, &x3); swapXOR(&y2, &y3); }

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

            //shorten the bounds of the triangle cause its causing the lines not to be rendered
            /* need to see if there's a better way to do this */
            // if (ax > bx) { ax=ax-3 ; bx=bx+3; } else { ax=ax+3 ; bx=bx-3; }

            line(surface, ax, y, bx, y, fill_color);
        }
    }

    //fill the upper half of the triangle
    if (y2 != y3) {
        int upper_height = y3 - y2;

        for (int y=y2 ; y<=y3 ; y++) {

            //sweep the horizontal line
            int ax = x1 + (x3 - x1) * (y - y1) / total_height;
            int bx = x2 + (x3 - x2) * (y - y2) / upper_height;

            // if (ax > bx) { ax=ax-3 ; bx=bx+3; } else { ax=ax+3 ; bx=bx-3; }

            line(surface, ax, y, bx, y, fill_color);
        }
    }

    line(surface, x1, y1, x2, y2, line_color);
    line(surface, x2, y2, x3, y3, line_color);
    line(surface, x3, y3, x1, y1, line_color);
}

/*
for each polygon/face
    for each pixel in polygon
        if pixel in polygon
            if polygon's pixel-depth < current pixel-depth
                current pixel depth = polygon pixel depth
*/

float signed_triangle_area (int ax, int ay, int bx, int by, int cx, int cy) {
    return (0.5)*(by-ay)*(bx+ax) + (cy-by)*(cx+bx) + (ay-cy)*(ax+cx);
}

void update_zbuffer(SDL_Surface* surface, int width, int height, float z_buffer[width][height], int polygon_count, Vec3 faces[polygon_count][3], Object obj, Vec3 camera_pos) {

    for (int i=0 ; i<polygon_count ; i++) {

        //determine the min bounding box for this triangle
        int xmin, ymin, xmax, ymax;

        //project the face onto the screen to get xy coords of each point in the triangle
        Vec2 p1 = perspective_proj(faces[i][0], camera_pos);
        Vec2 p2 = perspective_proj(faces[i][1], camera_pos);
        Vec2 p3 = perspective_proj(faces[i][2], camera_pos);
        
        //3-element bubble sort
        xmax = (int)((p1.x > p2.x) ? (p1.x > p3.x) ? p1.x : p3.x : (p2.x > p3.x) ? p2.x : p3.x);
        ymax = (int)((p1.y > p2.y) ? (p1.y > p3.y) ? p1.y : p3.y : (p2.y > p3.y) ? p2.y : p3.y);
        xmin = (int)((p1.x < p2.x) ? (p1.x < p3.x) ? p1.x : p3.x : (p2.x < p3.x) ? p2.x : p3.x);
        ymin = (int)((p1.y < p2.y) ? (p1.y < p3.y) ? p1.y : p3.y : (p2.y < p3.y) ? p2.y : p3.y);

        //iterate through the min bounding box
        for (int j=xmin ; j<xmax ; j++) {
            for (int k=ymin ; k<ymax ; k++) {

                //use barycentric coords to determine if in triangle
                float area = signed_triangle_area(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);

                float alpha = signed_triangle_area(j, k, p2.x, p2.y, p3.x, p3.y) / area;
                float beta = signed_triangle_area(j, k, p3.x, p3.y, p1.x, p1.y) / area;
                float gamma = signed_triangle_area(j, k, p1.x, p1.y, p2.x, p2.y) / area;

                if (alpha > 0.0 && alpha < 1.0 && beta < 0.0 && beta > 1.0 && gamma < 0.0 && gamma < 1.0) {
                    
                    float z1 = faces[i][0].z;
                    float z2 = faces[i][1].z;
                    float z3 = faces[i][2].z;

                    float pixel_depth = (alpha * z1 + beta * z2 + gamma * z3);

                    if (pixel_depth < z_buffer[i][j]) {
                        z_buffer[i][j] = pixel_depth;
                        Uint32 color = (0xFFFFFF) * (100 - pixel_depth);
                        set_pixel(surface, i, j, color);
                    }
                }
            }
        }
    }
}
