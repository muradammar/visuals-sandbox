#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "rasterizer1.h"

/*
this file contatins the functionality for applying transformations to a 3D
point in all 6 DOF. 
*/

//dot product between a 2D 3x3 matrix and 1D 3x1 column vector
void dot(float matrix[3][3], Vec3 *v) {

    Vec3 result = {0, 0, 0};
    float result_array[3] = {result.x, result.y, result.z};
    float cur_position[3] = {v->x, v->y, v->z};

    for (int i=0 ; i<3 ; i++) {
        float elem_result = 0;

        for (int j=0 ; j<3 ; j++) {
            elem_result += cur_position[j] * matrix[i][j];
        }

        result_array[i] = elem_result;
    }

    //copy result onto input vector
    v->x = result_array[0];
    v->y = result_array[1];
    v->z = result_array[2];
}

//rotate the given 3D coordinate about the x-axis by the given angle in degrees
void rotate_x(Vec3 *v, int angle) {

    //deg to rad
    float angle_rad = (float)(angle * 0.0174532925);

    float rotation_matrix [3][3] = {{1,               0,                0},
                                    {0, cosf(angle_rad), -sinf(angle_rad)},
                                    {0, sinf(angle_rad), cosf(angle_rad)}};

    dot(rotation_matrix, v);
}

// //adjust the face normal after rotation
// void adjust_face(Vec3 v1, Vec3 v2, Vec3 v3, Vec3 *face) {

//     //compute cross product to find normal
//     Vec3 a = {v2.x - v1.x}
// }