#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>

struct point_s {
    double x;
    double y;
    double z;
};
typedef struct point_s point;
typedef point vector;

struct player_s {
    point* coord;
    double angle_y;
    double angle_z;
    double FOV;
    double tanHalf_Fov;
    SDL_Renderer* renderer;
};
typedef struct player_s player;

struct object_s {
    int nb_points;
    point* points;
    int** graph;
};
typedef struct object_s object;

struct display_s {
    int height;
    int width;
};
typedef struct display_s display;

struct point_2d_s {
    double x;
    double y;
};
typedef struct point_2d_s point_2d;
typedef point_2d vector_2d;

struct plane_s {
    double a;
    double b;
    double c;
    double d;
};
typedef struct plane_s plane;





///* FUNCTIONS *///




player* create_player(double FOV,SDL_Renderer* renderer){
    player* p = malloc(sizeof(player));
    p->FOV = FOV;
    p->tanHalf_Fov = tan(FOV/2);
    p->renderer = renderer;
    return p;
}

//Absolute Value for double var
double d_abs(double x){
    if (x < 0) return -x;
    return x;
}

//Distance between 2points
double dist(point a,point b){
    double x = (a.x - b.x);
    double y = (a.y - b.y);
    double z = (a.z - b.z);
    return sqrt(x*x + y*y + z*z);
}

//Scalar product
double scalar(vector a,vector b){
    return (a.x * b.x ) + (a.y * b.y) + (a.z * b.z);
}

//Scalar with 2d_vecgtor
double scalar_2d(vector_2d a, vector_2d b){
    return (a.x * b.x ) + (a.y * b.y);
}

//Pick orthogonal vector from a plane
vector get_orthogonal(plane p){
    vector v;
    v.x = p.a;
    v.y = p.b;
    v.z = p.c;
    return v;

}







///Object Moves

//Just adds x y z to currents object coords
void move(object ob,double x,double y,double z){
    for (int i = 0;i < ob.nb_points;i++){
        ob.points[i].x = ob.points[i].x + x;
        ob.points[i].y = ob.points[i].y + y;
        ob.points[i].z = ob.points[i].z + z;
    }
    
}

//Rotation z_axe centered , true rotation ( means that it's same axe for every single object)
void rotate_z(object ob,double angle){
    for (int i = 0; i < ob.nb_points; i++){
        ob.points[i].x = ob.points[i].x * cos(angle) - ob.points[i].y * sin(angle);
        ob.points[i].y = ob.points[i].x * sin(angle) + ob.points[i].y * cos(angle);
    }
}

//Rotation y_axe centered , true rotation ( same as b4 )
void rotate_y(object ob,double angle){
    for (int i = 0; i < ob.nb_points; i++){
        ob.points[i].z = ob.points[i].z * cos(angle) - ob.points[i].x * sin(angle);
        ob.points[i].x = ob.points[i].z * sin(angle) + ob.points[i].x * cos(angle);
    }
}

//Screen projecting , converts 3d coords into 2d coords , simulating perspective
//(0,0) Screen center , arbitrary choice . 
point_2d projection(point p,int width,int height,player* pl){
    double x = p.x;
    double y = p.y;
    double z = p.z;

    double alpha = 1 / (2 * pl->tanHalf_Fov);
    
    double x_prime = y;
    double y_prime = z;
    if (x > 0){
        x_prime = y*alpha/x;
        y_prime = z*alpha/x;
    }

    double x_seconde = width*x_prime;
    double y_seconde = height*y_prime;

    point_2d n_p;
    n_p.x = x_seconde + width/2;
    n_p.y = height - (y_seconde + height/2);
    return n_p;

}

//Draw the object on a given renderer , with a given FOV ( SDL_Window only used to pick up height and width )
void show_obj(object ob,SDL_Window* window,player* p1){
    point_2d* coords = malloc(sizeof(point_2d)*ob.nb_points);
    int width;
    int height;
    SDL_GetWindowSize(window,&width,&height);
    for (int i = 0; i<ob.nb_points; i++) coords[i] = projection(ob.points[i],width,height,p1);


    for (int i = 0; i<ob.nb_points; i++) {
        point p = ob.points[i];
        for (int j = 1; j<=ob.graph[i][0]; j++){
            int indice = ob.graph[i][j];
            point voisin = ob.points[indice];

///Condition pour afficher
            if ( (voisin.x > 0 && voisin.y/voisin.x < p1->tanHalf_Fov)     ||         (p.x > 0 && p.y/p.x < p1->tanHalf_Fov) ){
                SDL_RenderDrawLine(p1->renderer, coords[i].x, coords[i].y,coords[indice].x, coords[indice].y);
            }
        }
    }
    free(coords);
}


///Utilities

//returns a 2d_square, bottom left centered

object make_square(double x,double y,double z,double side_len){
    point* liste_points = malloc(sizeof(point)*4);
    int** graph = malloc(sizeof(int*)*3);
    for (int i = 0;i<4;i++) {
        graph[i] = malloc(sizeof(int)*3);
        graph[i][0] = 2;
    }
    for (int i = 0;i<=1;i++){
        for (int j = 0;j<=1;j++){
            liste_points[j + 2*i].x = x;
            liste_points[j + 2*i].y = y+ (double)i*side_len;
            liste_points[j + 2*i].z = z + (double)j*side_len;
            graph[j + 2*i][1] = (i+j+1)%2;
            graph[j + 2*i][2] = ((i+j)%2)*3 + ((i+j+1)%2)*2;
        }
    }
    object c;
    c.nb_points = 4;
    c.points = liste_points;
    c.graph = graph;

    return c;
}

//Cube , bottom front left centered
object make_cube(double x,double y, double z,double side_len){
    point* liste_points = malloc(sizeof(point)*8);
    int** graph = malloc(sizeof(point*)*8);
    for (int i = 0;i<8;i++){
        graph[i] = malloc(sizeof(int)*4);
        graph[i][0] = 3;
    }

    object c1 = make_square(x,y,z,side_len);
    object c2 = make_square(x+side_len ,y,z,side_len);

    for (int i = 0;i<c1.nb_points;i++){
        liste_points[i] = c1.points[i];
        for (int j = 1 ; j <= c1.graph[i][0];j++) graph[i][j] = c1.graph[i][j];
        graph[i][3] = i+4;
    }
    for (int i = 0;i<c2.nb_points;i++){
        liste_points[i+4] = c2.points[i];
        for (int j = 1;j <=c2.graph[i][0];j++){
            graph[i+4][j] = c2.graph[i][j] + 4;
            printf("%d %d \n",i+4,c2.graph[i][j]+4);
        }
        graph[i+4][3] = i;
    }

    object c;
    c.nb_points = 8;
    c.points = liste_points;
    c.graph = graph;

    printf("C'est envoyé");
    fflush(stdout);

    return c;
}

//Prints coordinates of an object
void print_coords(object ob,int width,int height,player* p1){
     for (int i = 0; i<ob.nb_points; i++) {
        point p = ob.points[i];
        point_2d p2d= projection(p,width,height,p1);
        printf("point n%d : (%f,%f,%f) -> (%d,%d)\n",i,p.x,p.y,p.z,(int)p2d.x,(int)p2d.y);
    }
}