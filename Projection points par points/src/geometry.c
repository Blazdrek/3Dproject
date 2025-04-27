#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>



struct point_s {double x;double y;double z;};
typedef struct point_s point;
typedef point vector;


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


point_2d projection(point p,int width,int height,double FOV){
    double x = p.x;
    double y = p.y;
    double z = p.z;

    //Premiere étape : projection sur le plan de distance 1
    double alpha_x = y/x;
    double alpha_y = z/x;

    //Deuxieme étape : projection sur l'ecran
    double alpha_p_x = ((double) width * alpha_x / (2.0 * tan(FOV/2.0)));
    double alpha_p_y = ((double) height * alpha_x / (2.0 * tan(FOV/2.0)));
    
    point_2d n_p;
    n_p.x = alpha_p_x;
    n_p.y = alpha_p_y;
    return n_p;

}
point_2d projection_v2(point p,int width,int height,double FOV){
    double x = p.x;
    double y = p.y;
    double z = p.z;

    double alpha = 1 / (2 * sin(FOV/2));
    double x_prime = y*alpha/x;
    double y_prime = z*alpha/x;

    double x_seconde = width*x_prime;
    double y_seconde = height*y_prime;

    point_2d n_p;
    n_p.x = x_seconde + width/2;
    n_p.y = y_seconde + height/2;
    return n_p;

}

point* cube(double x,double y, double z,double side_len){
    point* liste_points = malloc(sizeof(point)*8);
    int indice = 0;
    for (int i = 0;i<2;i++){
        for (int j = 0;j<2;j++){
            for (int k = 0;k<2;k++){
                liste_points[indice].x = x + (i * side_len) ;
                liste_points[indice].y = y + (j * side_len) ;
                liste_points[indice].z = z + (k * side_len );
                indice++;
            }
        }
    }
    return liste_points;
}