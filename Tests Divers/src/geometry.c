#include "geometry.h"

int vector2_isnull(Vector2 *v) {
    return v->x == 0 && v->y == 0;
}
int vector2cmp(Vector2 *v, Vector2 *w) {
    return v->x != w->x || v->y != w->y;
}

int vector3_isnull(Vector3 *v) {
    return v->x == 0 && v->y == 0 && v->z == 0;
}
int vector3cmp(Vector3 *v, Vector3 *w) {
    return v->x != w->x || v->y != w->z || v->z != w->z;
}

void rotate_object(Object *object, Vector3 *rotation) {
    if (vector3_isnull(rotation)) return;
    Mat *rot_x = mat_create(3, 3,
            1, 0, 0,
            0, cos(rotation->x), -sin(rotation->x),
            0, sin(rotation->x), cos(rotation->x)
        );

    Mat *rot_y = mat_create(3, 3, 
            cos(rotation->y), 0, sin(rotation->y),
            0, 1, 0,
            -sin(rotation->y), 0, cos(rotation->y)
        );

    Mat *rot_z = mat_create(3, 3, 
            cos(rotation->z), -sin(rotation->z), 0,
            sin(rotation->z), cos(rotation->z), 0,
            0, 0, 1
        );
}