#ifndef TRANSFORMER
#define TRANSFORMER

#include "rasterizer1.h"

void dot(float matrix[3][3], Vec3 *v);
void rotate_x(Vec3 *v, int angle);
void rotate(Vec3 *v, int gamma, int beta, int alpha);
void adjust_face(Vec3 v1, Vec3 v2, Vec3 v3, Vec3 *face);
void normalize(Vec3 *v);

#endif