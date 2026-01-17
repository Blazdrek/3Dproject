#include <stdlib.h>
#include <stdbool.h>
#include <math.h>



struct point_s {double x;double y;double z;};
typedef struct point_s point;
typedef point vector;
struct plane_s {int a;int b;int c;int d;};
typedef struct plane_s plane;
struct polygon_s {point* vertices;plane p;};
typedef struct polygon_s polygon;
struct hitbox_s {int height;int width;int length;};
typedef struct hitbox_s hitbox;
struct object_s {polygon* components;point anchor;hitbox h;};
typedef struct object_s object;

struct player_s {double angleX;double FOV;point pos;hitbox h;};
typedef struct player_s player;

double dist(point a,point b){
    double x = (a.x - b.x);
    double y = (a.y - b.y);
    double z = (a.z - b.z);
    return sqrt(x*x + y*y + z*z);
}

double scalaire(vector a,vector b){
    return (a.x * b.x ) + (a.y * b.y) + (a.z * b.z);
}

int projection(player p,point a){
    
}


