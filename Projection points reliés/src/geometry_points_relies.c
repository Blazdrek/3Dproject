#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>



struct point_s {double x;double y;double z;};
typedef struct point_s point;
typedef point vector;

struct object_s {int nb_points ; point* points; point** graph};
typedef struct object_s object;

struct display_s {int height;int width;};
typedef struct display_s display;

struct point_2d_s {double x;double y;};
typedef struct point_2d_s point_2d;
typedef point_2d vector_2d;

// Distance entre deux points
double dist(point a,point b){
    double x = (a.x - b.x);
    double y = (a.y - b.y);
    double z = (a.z - b.z);
    return sqrt(x*x + y*y + z*z);
}

double scalaire(vector a,vector b){
    return (a.x * b.x ) + (a.y * b.y) + (a.z * b.z);
}

double scalaire_2d(vector_2d a, vector_2d b){
    return (a.x * b.x ) + (a.y * b.y);
}

point_2d projection_v2(point p,int width,int height,double FOV){
    double x = p.x;
    double y = p.y;
    double z = p.z;

    double alpha = 1 / (2 * tan(FOV/2));
    double x_prime = y*alpha/x;
    double y_prime = z*alpha/x;

    double x_seconde = width*x_prime;
    double y_seconde = height*y_prime;

    point_2d n_p;
    n_p.x = x_seconde + width/2;
    n_p.y = y_seconde + height/2;
    return n_p;

}

object make_cube(double x,double y, double z,double side_len){
    point* liste_points = malloc(sizeof(point)*8);
    point** graph = malloc(sizeof(point*)*8);
    for (int i = 0; i<8;i++) graph[i] = malloc(sizeof(point)*3);
    int indice = 0;
    for (int i = 0;i<8;i++){
         int nz = i %2;
        int ny = (i/2) % 2;
        int nx = (i/4) %2 ;
        liste_points[i].x = nx;
        liste_points[i].y = ny;
        liste_points[i].z = nz;
        


    }

    object c;
    c.nb_points = 8;
    c.points = liste_points;
    return c;
}