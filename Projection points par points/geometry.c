#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

struct point_s {double x;double y;double z;};
typedef struct point_s point;
typedef point vector;

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

