// void load_obj(SDL_Surface* surface, const char* filename, int max_vertices, Uint32 color) {
//     FILE* fptr = fopen(filename, "r");

//     //error handling
//     if (fptr == NULL) {
//         printf("Error opening file\n");
//         return;
//     }

//     //printf("file opened");

//     static Vec3 vertices[1024];
//     static int vertices_count = 0;
//     static bool initialized = false;

//     if (!initialized) {
//         FILE* fptr = fopen(filename, "r");
//         initialized = true;
//     }

//     //read obj file line-by-line
//     char line[128];
//     while (fgets(line, sizeof(line), fptr)) {

//         //read in all points
//         if (line[0] == 'v' && line[1] == ' ') {
//             Vec3 v;
//             sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z);

//             vertices[vertices_count] = v;
//             vertices_count++;

//             //printf("point: %f %f %f\n", v.x, v.y, v.z);
//         }

        

//         /*
//         faces are formatted as such: x/y/z a/b/c d/e/f

//         where the first entry in every entity is the vertex of a triangle. 
//         the middle entry is the texture if any. and the final entry is the normal
//         vector. The final entry should be the same across all entities as they have the same
//         normal. 

//         ex: f 3//1 7//1 8//1 is triangle 3->7->1 with a normal in the direction
//         of normal vector #1
//         */

//         //read in faces
//         else if (line[0] == 'f') {
//             int a, b, c;

//             sscanf(line, "f %d//%*d %d//%*d %d//%*d", &a, &b, &c);

//             //project the 3d points a, b, c onto 2D space and draw the corresp. triangle
//             Vec2 p1 = orthographic_proj(vertices[a - 1]);
//             Vec2 p2 = orthographic_proj(vertices[b - 1]);
//             Vec2 p3 = orthographic_proj(vertices[c - 1]);

//             drawTriangle(surface, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color);

//             //printf("%f %f %f %f %f %f\n",  p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
//         }
//     }

    //rotate
//     for (int i=0 ; i<vertices_count ; i++) {
//         rotate_x(&vertices[i], 1);
//     }

//     fclose(fptr);
//     return;
// }

// void load_obj(SDL_Surface* surface, const char* filename, int max_vertices, Uint32 color) {
//     static Vec3 vertices[1024];
//     static int vertices_count = 0;
//     static bool initialized = false;

//     static int face_indices[1024][3];  // store faces
//     static int face_count = 0;

//     if (!initialized) {
//         FILE* fptr = fopen(filename, "r");

//         if (fptr == NULL) {
//             printf("Error opening file\n");
//             return;
//         }

//         char line[128];
//         while (fgets(line, sizeof(line), fptr)) {

//             if (line[0] == 'v' && line[1] == ' ') {
//                 Vec3 v;
//                 sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z);
//                 vertices[vertices_count++] = v;
//             }

//             else if (line[0] == 'f') {
//                 int a, b, c;
//                 sscanf(line, "f %d//%*d %d//%*d %d//%*d", &a, &b, &c);
//                 face_indices[face_count][0] = a - 1;
//                 face_indices[face_count][1] = b - 1;
//                 face_indices[face_count][2] = c - 1;
//                 face_count++;
//             }
//         }

//         fclose(fptr);
//         initialized = true;
//     }

//     // Draw all stored faces
//     for (int i = 0; i < face_count; i++) {
//         int a = face_indices[i][0];
//         int b = face_indices[i][1];
//         int c = face_indices[i][2];

//         Vec2 p1 = orthographic_proj(vertices[a]);
//         Vec2 p2 = orthographic_proj(vertices[b]);
//         Vec2 p3 = orthographic_proj(vertices[c]);

//         // drawTriangle(surface, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color);
//     drawTriangle(surface, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color);
//     }

//     // Rotate cube
//     for (int i = 0; i < vertices_count; i++) {
//         rotate_x(&vertices[i], 1);
//     }
// }