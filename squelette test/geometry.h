#include "geometry.c"
#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_


//struct point_s {int x;int y;int z;};
typedef struct point_s point;

//struct plane_s {int a;int b;int c;int d;};
typedef struct plane_s plane;

//struct polygon_s {point* vertices;plane p;};
typedef struct polygon_s polygon;

//struct object_s {polygon* components;point anchor;};
typedef struct object_s object;

//struct entity_s {int angleX;point pos;};
typedef struct entity_s entity;


bool collide(player e;object o);
void move(player e1,int x,int y);




#endif