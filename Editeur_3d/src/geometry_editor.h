#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_


struct point_s {double x;double y;double z;};
typedef struct point_s point;
typedef point vector;

struct point_2d_s {double x;double y;};
typedef struct point_2d_s point_2d;
typedef point_2d vector_2d;

struct object_s {int nb_points ; point* points;int** graph;};
typedef struct object_s object;

struct display_s {int height;int width;};
typedef struct display_s display;



double dist(point a,point b);
double scalaire(vector a,vector b);
double scalaire_2d(vector_2d a, vector_2d b);
void move(object ob,double x,double y,double z);
void rotate_z(object ob,double angle);
void rotate_y(object ob,double angle);
point_2d projection(point p,int width,int height,double FOV);
point_2d projection_v2(point p,int width,int height,double FOV);
object make_square(double x,double y,double z,double side_len);
object make_cube(double x,double y, double z,double side_len);
object open_obj(char* filename);
object save_obj(object obj,char* filename);


#endif