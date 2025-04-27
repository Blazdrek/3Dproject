#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_


struct point_s {double x;double y;double z;};
typedef struct point_s point;
typedef point vector;

struct point_2d_s {double x;double y;};
typedef struct point_2d_s point_2d;
typedef point_2d vector_2d;

struct display_s {int height;int width;};
typedef struct display_s display;



double dist(point a,point b);
double scalaire(vector a,vector b);
double scalaire_2d(vector_2d a, vector_2d b);
point_2d projection(point p,int width,int height,double FOV);
point_2d projection_v2(point p,int width,int height,double FOV);
point* cube(double x,double y, double z,double side_len);


#endif