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

//rotate the given 3d coordinate. Rotation is specified by yaw (z, gamma), pitch(y, beta), roll(x, alpha)
void rotate(Vec3 *v, int gamma, int beta, int alpha) {
    float gamma_rad = gamma * 0.0174532925f;
    float beta_rad  = beta  * 0.0174532925f;
    float alpha_rad = alpha * 0.0174532925f;

    float rotation_matrix[3][3] = {
        { cosf(beta_rad) * cosf(gamma_rad),  cosf(beta_rad) * sinf(gamma_rad),  -sinf(beta_rad) },
        { sinf(alpha_rad) * sinf(beta_rad) * cosf(gamma_rad) - cosf(alpha_rad) * sinf(gamma_rad),
          sinf(alpha_rad) * sinf(beta_rad) * sinf(gamma_rad) + cosf(alpha_rad) * cosf(gamma_rad),
          sinf(alpha_rad) * cosf(beta_rad) },
        { cosf(alpha_rad) * sinf(beta_rad) * cosf(gamma_rad) + sinf(alpha_rad) * sinf(gamma_rad),
          cosf(alpha_rad) * sinf(beta_rad) * sinf(gamma_rad) - sinf(alpha_rad) * cosf(gamma_rad),
          cosf(alpha_rad) * cosf(beta_rad) }
    };

    dot(rotation_matrix, v);
}


//adjust the face normal after rotation
void adjust_face(Vec3 v1, Vec3 v2, Vec3 v3, Vec3 *face) {

    //compute cross product to find normal
    Vec3 a = {v3.x - v1.x, v3.y - v1.y, v3.z - v1.z};
    Vec3 b = {v2.x - v1.x, v2.y - v1.y, v2.z - v1.z};

    // a x b
    face->x = (a.y * b.z) - (a.z * b.y);
    face->y = (a.z * b.x) - (a.x * b.z);
    face->z = (a.x * b.y) - (a.y * b.x);
}

//normalize given vector
void normalize(Vec3 *v) {

    //get vector magnitude
    float mag = (v->x * v->x) + (v->y * v->y) + (v->z * v->z);

    mag = sqrt(mag);

    v->x = v->x / mag;
    v->y = v->y / mag;
    v->z = v->z / mag;
}