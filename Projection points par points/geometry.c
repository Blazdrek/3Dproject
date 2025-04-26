#include <stdlib.h>
#include <stdbool.h>
#include <math.h>



struct point_s {double x;double y;double z;};
typedef struct point_s point;
typedef point vector;

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


point_2d projection(point p){
    double x = p.x;
    double y = p.y;
    double z = p.z;

    //Premiere étape : projection sur le plan de distance 1
    double x1 = y/x;
    double y1 = z/x;

    //Deuxieme étape : projection sur l'ecran
    


}
