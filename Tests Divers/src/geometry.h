#pragma once

#include <math.h>
#include "matrice.h"


typedef struct {
    double x;
    double y;
} Vector2;

typedef struct {
    double x;
    double y;
    double z;
} Vector3;

typedef struct {
    Vector3 vertex0;
    Vector3 vertex1;
    Vector3 vertex2;
    Vector3 normal;
} Triangle;

typedef struct {
    Triangle *triangles;
    int triangle_count;
} Object;

int vector2_isnull(Vector2 *v);
int vector2_cmp(Vector2 *v, Vector2 *w);
int vector3_isnull(Vector3 *v);
int vector3_cmp(Vector3 *v, Vector3 *w);
void rotate_object(Object *object, Vector3 *rotation);


