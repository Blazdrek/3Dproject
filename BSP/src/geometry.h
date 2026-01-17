#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <SDL2/SDL.h>

typedef struct point_s {
    double x;
    double y;
    double z;
} point ;
typedef point vector;


typedef struct player_s {
    point* coord;
    double angle_y;
    double angle_z;
    double FOV;
    double tanHalf_Fov;
    SDL_Renderer* renderer;
} player ;

typedef struct object_s {
    int nb_points;
    point* points;
    int** graph;
} object ;

typedef struct display_s {
    int height;
    int width;
} display ;

typedef struct point_2d_s {
    double x;
    double y;
} point_2d;

typedef struct line_s {
    point a;
    vector v;
} line;

typedef struct color_s{
    int r;
    int g;
    int b;
} color;

typedef struct plane_s { //ax + by + cz + d = 0
    double a;
    double b;
    double c;
    double d;
} plane ;

typedef struct polygon_s {
    plane p;
    point* vertices;
    int len;
} polygon;

typedef struct polygon_list_s {
    polygon* list;
    int size;
    int MAX_SIZE;
} polygon_list;

polygon_list* create_list();
polygon_list* create_from(polygon* list,int len);
void append(polygon_list* l,polygon p);
polygon pop(polygon_list* l);
polygon get(polygon_list* l,int i);

double d_abs(double x);
double dist(point a,point b);
double scalar(vector a,vector b);
double belong_to_plane(plane s,point p);
double belong_to_line_2d(line l,point p);
vector get_orthogonal(plane s);
line get_line_passing(point a,point b);
point intersect(plane p,line d);
void split_polygon(plane p,polygon* plg,polygon* front_p,polygon* back_p);

player* create_player(double FOV,SDL_Renderer* renderer);
void move_player(player* p1,double x,double y, double z);
void rotate_z(object ob,double angle);
void rotate_y(object ob,double angle);
void fill_triangle(point_2d A,point_2d B,point_2d C,SDL_Renderer* renderer);
point_2d projection(point p,int width,int height,player* pl);





#endif