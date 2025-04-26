#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>



struct point_s {double x;double y;double z;};
typedef struct point_s point;
typedef point vector;


struct display_s {int height;int width;};
typedef struct display_s display;

struct point_2d_s {double x;double y};
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


point_2d projection(point p,int width,int height,double FOV){
    double x = p.x;
    double y = p.y;
    double z = p.z;

    //Premiere étape : projection sur le plan de distance 1
    double alpha_x = y/x;
    double alpha_y = z/x;

    //Deuxieme étape : projection sur l'ecran
    double alpha_p_x = ((double) width * alpha_x / 2.0 * tan(FOV/2.0));
    double alpha_p_x = ((double) height * alpha_x / 2.0 * tan(FOV/2.0));
    
    point_2d p;
    p.x = alpha_p_x;
    p.y = alpha_p_y;
    return p;

}
