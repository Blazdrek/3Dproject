#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "queue.h"

///////////////////////////////////////
//Structures///////////////////////////
///////////////////////////////////////

typedef struct point_s {
    double x;
    double y;
    double z;
} point ;
typedef point vector;

typedef struct point_2d_s {
    double x;
    double y;
} point_2d;

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

typedef struct color_s{
    int r;
    int g;
    int b;
} color;

typedef struct line_s {
    point a;
    vector v;
} line;

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

///////////////////////////////////////
//Functions ///////////////////////////
///////////////////////////////////////

    /////////////Polygons Dyn Array////////

polygon_list* create_list(){
    polygon_list* p_l= malloc(sizeof(polygon_list));
    p_l->MAX_SIZE = 10;
    p_l->size = 0;
    p_l->list = malloc(sizeof(polygon)*p_l->MAX_SIZE);
}

polygon_list* create_from(polygon* list,int len){
    polygon_list* p_l= malloc(sizeof(polygon_list));
    p_l->MAX_SIZE = len*2;
    assert(p_l->MAX_SIZE > len);
    p_l->size = len;
    p_l->list = realloc(list,sizeof(polygon)*p_l->MAX_SIZE);
    return p_l;
}

void append(polygon_list* l,polygon p){
    l->list[l->size] = p;
    l->size++;
    if (l->size == l->MAX_SIZE){
        assert(l->MAX_SIZE < l->MAX_SIZE*2);
        l->MAX_SIZE = l->MAX_SIZE * 2;
        l->list = realloc(l->list,sizeof(polygon)*l->MAX_SIZE);
        assert(l->list != NULL);  
    }
}

polygon pop(polygon_list* l){
    assert(l->size > 0);
    polygon p = l->list[l->size - 1];
    l->size--;
    if (l->size <= l->MAX_SIZE/4){
        l->MAX_SIZE = l->MAX_SIZE/2;
        l->list = realloc(l->list,sizeof(polygon)*l->MAX_SIZE);
        assert(l->list != NULL);
    }
    return p;
}

polygon get(polygon_list* l,int i){ 
    assert(i>=0 && i<l->size);
    return l->list[i] ;
}



    /////////////Maths/////////////////////

        ////Utilities :

// |x|
double d_abs(double x){
    if (x < 0) return -x;
    return x;
}

// ||a - b||
double dist(point a,point b){
    double x = (a.x - b.x);
    double y = (a.y - b.y);
    double z = (a.z - b.z);
    return sqrt(x*x + y*y + z*z);
}

//  <a|b>
double scalar(vector a,vector b){
    return (a.x * b.x ) + (a.y * b.y) + (a.z * b.z);
}

        ///Planes :

// ax + by + cz + d
double belong_to_plane(plane s,point p){
    return s.a * p.x + s.b * p.y + s.c * p.z + s.d;
}

// (a,b,c)
vector get_orthogonal(plane s){
    vector v;
    v.x = s.a;
    v.y = s.b;
    v.z = s.c;
    return v;
}

// (AB)
line get_line_passing(point a,point b){
    vector v;
    line l;
    l.a = b;
    v.x = (a.x - b.x);
    v.y = (a.y - b.y);
    v.z = (a.z - b.z);
    l.v = v;

    return l;
}

// Intersction plane  / line (! does not works if d ⊂ P )
point intersect(plane p,line d){
    ///
    assert(scalar(get_orthogonal(p),d.v) != 0);
    double k = -1 * belong_to_plane(p,d.a) / scalar(get_orthogonal(p),d.v);//Voir shéma 1
    point pt;
    pt.x = d.a.x + k * d.v.x;
    pt.y = d.a.y + k * d.v.y;
    pt.z = d.a.z + k * d.v.z;
    return pt;  
}



void split_polygon(plane p,polygon* plg,polygon* front,polygon* back){

    
    point prec = plg->vertices[0];
    point* start = malloc(sizeof(point)); *start = prec;
    double c_prec = belong_to_plane(p,prec);

    queue* q_front = create_queue();
    queue* q_back = create_queue();

    if (c_prec == 0) {
        point* start2 = malloc(sizeof(point)); *start2 = *start;

        enqueue(q_front,start);
        enqueue(q_back,start2);
    } 
    else if (c_prec > 0) enqueue(q_front,start);
    else enqueue(q_back,start);

    for (int i = 1;i < plg->len;i++){
        point curr = plg->vertices[i];
        double c = belong_to_plane(p,curr);

        point* curr_add = malloc(sizeof(point)); *curr_add = curr;

        if (c * c_prec < 0){ // Les deux points A et B sont de part et d'autre du plan [(+) * (-) < 0]
            line d = get_line_passing(prec, curr);
            point inter = intersect(p,d); // calcul du point d'intersection du plan p et de la droite (prec curr)
            point *inter_add = malloc(sizeof(point)); *inter_add = inter;
            point *inter_add2 = malloc(sizeof(point)); *inter_add2 = inter;

            if (c > 0) { //Cas ou A->B Traverse le plan , A (-) et B (+)
                enqueue(q_back,inter_add); // Je rajoute le point d'intersection a la suite
                enqueue(q_front,inter_add2);
                enqueue(q_front,curr_add);
            } else { //Cas ou A->B Traverse le plan , A(+) et B (-)
                enqueue(q_front,inter_add); // Je rajoute le point d'intersection a la suite
                enqueue(q_back,inter_add2);
                enqueue(q_back,curr_add);
            }
        } else if (c == 0 ){// Cas ou A->B traverse le plan car B appartient au plan
            point* curr_add2 = malloc(sizeof(point)); *curr_add2 = *curr_add;
            enqueue(q_front,curr_add);// on doit ajouter le point aux deux polygones
            enqueue(q_back,curr_add2);
        } else if (c > 0 ) { // Cas ou A->B ne traverse pas le plan, B ( + )
            enqueue(q_front,curr_add);
        } else if (c < 0) { // Cas ou A->B ne traverse pas le plan , B(-)
            enqueue(q_back,curr_add);
        }
        prec = curr;
        c_prec = c;
    }

    point curr = *start;
    double c = belong_to_plane(p,curr);
    c_prec = belong_to_plane(p,prec);

    //On compare finalement le dernier point avec le premier pour clore le polygon , au cas ou la derniere droite coupe le plan
    if (c * c_prec < 0){ 
        line d = get_line_passing(prec, curr);
        point inter = intersect(p,d); 
        point *inter_add = malloc(sizeof(point)); *inter_add = inter;
        point *inter_add2 = malloc(sizeof(point)); *inter_add2 = inter;
        enqueue(q_back,inter_add); 
        enqueue(q_front,inter_add2);
    }

    front->len = len(q_front);
    if (front->len > 0) front->vertices = malloc(sizeof(point)*front->len);
    for (int i = 0;i < front->len;i++){
        point* curr = dequeue(q_front);
        printf("front pol : %f %f %f\n",curr->x,curr->y,curr->z);
        front->vertices[i] = *curr;
        free(curr);
    }
    free_queue(q_front);

    back->len = len(q_back);
    if (back->len > 0) back->vertices = malloc(sizeof(point)*back->len);
    
    for (int i = 0;i < back->len;i++){
        point* curr = dequeue(q_back);
        printf("back pol : %f %f %f\n",curr->x,curr->y,curr->z);
        back->vertices[i] = *curr;
        free(curr);
    }
    free_queue(q_back);
    return;
}

/*void split_polygons(plane p,polygon_list plgs,polygon_list* front_p,polygon_list* back_p,polygon_list* intersect_p){
    
}*/



    ////////////////Game//////////////

player* create_player(double FOV,SDL_Renderer* renderer){
    player* p = malloc(sizeof(player));
    p->FOV = FOV;
    p->tanHalf_Fov = tan(FOV/2);
    p->renderer = renderer;
    return p;
}

void move_player(player* p1,double x, double y, double z){
    p1->coord->x = p1->coord->x + x;
    p1->coord->y = p1->coord->y + y;
    p1->coord->z = p1->coord->z + z;

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



point_2d projection(point p,int width,int height,player* pl){
    double x = p.x - pl->coord->x;
    double y = p.y - pl->coord->y;
    double z = p.z - pl->coord->z;

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

void fill_triangle(point_2d A,point_2d B,point_2d C,SDL_Renderer* renderer){
    point_2d ps[] = {A,B,C};
    point_2d min = A;
    point_2d max = A;
    for (int i = 0;i < 3;i++){
        point_2d curr = ps[i];
        if (curr.x < min.x) min.x = curr.x;
        if (curr.y < min.y) min.y = curr.y;
        if (curr.x > max.x) max.x = curr.x;
        if (curr.y > max.y) max.y = curr.y;
    }
    
    for (double x = min.x ; x< max.x ; x++){
        for (double y = min.y ; y < max.y ; y++){
            bool showing = true;
            for (int i = 0;i < 3;i++){
                point_2d edge1 = ps[i];
                point_2d edge2 = ps[(i+1)%3];
                point_2d third = ps[(i+2)%3];
                double a;
                if (edge1.x != edge2.x) a = (edge2.y - edge1.y) / (edge2.x - edge1.x);
                else a = 0.0;
                double b = edge1.y - edge1.x * a;
                double eq_car_point = a * x - y + b;
                double eq_car_temoin = a * third.x - third.y + b;
                showing = showing && (eq_car_point * eq_car_temoin >= 0);
            }
            if (showing) {
                //printf("(%f %f) ",x,y);
                SDL_RenderDrawPoint(renderer,(int) x,(int)y);
            }
        }
        //printf("\n");
    }
}

void show_polygon(player* pl , polygon pol){
    assert(pol.len > 2);
    point init = pol.vertices[0];
    point last = pol.vertices[1];
    for (int i = 2;i<pol.len;i++){
        point curr = pol.vertices[i];
    }
}