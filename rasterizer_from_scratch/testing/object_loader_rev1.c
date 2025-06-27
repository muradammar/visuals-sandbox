
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>

typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    Vec3 v1, v2, v3;    //vertices
    Vec3 vn1, vn2, vn3; //vertex normals
} Face;

typedef struct {

    int vertex_count;
    int face_count;
    int vn_count;

    Vec3* vertices;
    Vec3* vertex_normals;
    Face* faces;

} Object;

Object load_obj(const char* filename, int max_vertices) {

    Object obj;
    obj.vertex_count = 0;
    obj.face_count = 0;
    obj.vn_count = 0;

    FILE *fptr = fopen(filename, "r");

    if (fptr == NULL) {
        printf("error opening file");
    }

    char line[128];

    //first get the number of faces, vertices, and vertex normals
    while (fgets(line, sizeof(line), fptr)) {

        //vertex
        if(line[0] == "v" && line[1] == " ") {
            obj.vertex_count++;
        }

        //vertex normal
        else if (line[0] == "v" && line[1] == "n") {
            obj.vn_count++;
        }

        //face
        else if (line[0] == "f" && line[1] == "f") {
            obj.face_count++;
        }
    }

    //dynamically allocate memory for the variables
    obj.vertices = (Vec3*)malloc(obj.vertex_count * sizeof(Vec3));
    obj.faces = (Face*)malloc(obj.face_count * sizeof(Face));
    obj.vertex_normals = (Vec3*)malloc(obj.vn_count * sizeof(Vec3));

    //re-read the file and store the vertices, faces, etc.
    rewind(fptr);

    //keep track of how many of each element have been stored
    int v_count, f_count, vn_count;

    while (fgets(line, sizeof(line), fptr)) {

        //vertex
        if(line[0] == "v" && line[1] == " ") {
        
        }

        //vertex normal
        else if (line[0] == "v" && line[1] == "n") {
            obj.vn_count++;
        }

        //face
        else if (line[0] == "f" && line[1] == "f") {
            obj.face_count++;
        }
    }
    

}