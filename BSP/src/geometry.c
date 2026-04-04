#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "queue.h"

bool DEBUG_SHOW_POLYGON=true;

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
    point coord;
    double angle_x;
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

typedef struct color_s{// R, G, B
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
    color col;
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
    return p_l;
}

polygon_list* create_from(polygon* list,int len){//polygon* in arg , polygon_list created
    polygon_list* p_l= malloc(sizeof(polygon_list));
    p_l->MAX_SIZE = len*2;
    assert(p_l->MAX_SIZE > len);
    p_l->size = len;
    p_l->list = realloc(list,sizeof(polygon)*p_l->MAX_SIZE);
    return p_l;
}

void append(polygon_list* l,polygon p){//Dynamic array
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

void print_pol_list(polygon_list* plist){
    for (int i = 0;i<plist->size;i++){
        printf("%d\n",i);
        for (int j = 0;j<get(plist,i).len;j++){
            printf("%f,%f,%f\n",(get(plist,i).vertices[j].x),(get(plist,i).vertices[j].y),(get(plist,i).vertices[j].z));
    
        }
    }
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

double norme(vector a){
    return a.x*a.x + a.y*a.y + a.z*a.z;
}

vector vectorial_product(vector a,vector b){
    vector c;
    c.x = a.y * b.z - a.z * b.y;
    c.y = a.z * b.x - a.x * b.z;
    c.z = a.x * b.y - a.y * b.x;
    return c;
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
plane get_plane_passing_by(point a,point b,point c){
    vector ab = (vector) {a.x - b.x,a.y - b.y,a.z - b.z};
    vector ac = (vector) {a.x - c.x,a.y - c.y,a.z - c.z};
    vector vect = vectorial_product(ab,ac);
    double n = norme(vect);
    vect.x /= n;
    vect.y /= n;
    vect.z /= n;

    double d = - (vect.x * a.x + vect.y * a.y + vect.z * a.z);
    plane p = (plane) {vect.x,vect.y,vect.z,d};
    return p;
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
    if (front->len > 2) {
        front->vertices = malloc(sizeof(point)*front->len);
        front->p = plg->p;
    }
    else {
        (front->vertices = NULL);
        front->len = 0;
    }

    for (int i = 0;i < front->len;i++){
        point* curr = dequeue(q_front);
        //printf("front pol : %f %f %f\n",curr->x,curr->y,curr->z);
        front->vertices[i] = *curr;
        front->col = plg->col;
        free(curr);
    }
    free_queue(q_front);

    back->len = len(q_back);
    if (back->len > 2) {
        back->vertices = malloc(sizeof(point)*back->len); 
        back->p = plg->p;
    }
    else {
        back->vertices = NULL;
        back->len = 0;
    }
    
    for (int i = 0;i < back->len;i++){
        point* curr = dequeue(q_back);
        //printf("back pol : %f %f %f\n",curr->x,curr->y,curr->z);
        back->vertices[i] = *curr;
        back->col = plg->col;
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
    p->angle_x = 0;
    p->angle_y = 0;
    p->angle_z = 0;
    p->FOV = FOV;
    p->tanHalf_Fov = tan(FOV/2);
    p->renderer = renderer;
    p->coord = (point) {0,0,0};
    return p;
}

void move_player(player* p1,double x, double y, double z){
    p1->coord.x = p1->coord.x + x;
    p1->coord.y = p1->coord.y + y;
    p1->coord.z = p1->coord.z + z;

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

point relative_pos(point p,player* pl){
    point new_pos;
    new_pos.x = (p.x - pl->coord.x);
    new_pos.y = (p.y - pl->coord.y);
    new_pos.z = (p.z - pl->coord.z);

    double tmpx = new_pos.x;
    double tmpy = new_pos.y;
    double tmpz = new_pos.z;

        //Rotation autour de Z
    new_pos.x = tmpx * cos(pl->angle_z) - tmpy * sin(pl->angle_z);
    new_pos.y = tmpx * sin(pl->angle_z) + tmpy * cos(pl->angle_z);

    tmpx = new_pos.x;
    tmpy = new_pos.y;
    tmpz = new_pos.z;

        //Rotation autour de Y
    new_pos.z = tmpz * cos(pl->angle_y) - tmpx * sin(pl->angle_y);
    new_pos.x = tmpz * sin(pl->angle_y) + tmpx * cos(pl->angle_y);

    tmpx = new_pos.x;
    tmpy = new_pos.y;
    tmpz = new_pos.z;

        //Rotation autour de X
    new_pos.y = tmpy * cos(pl->angle_x) - tmpz * sin(pl->angle_x);
    new_pos.z = tmpy * sin(pl->angle_x) + tmpz * cos(pl->angle_x);    
    
    return new_pos;
}



point_2d projection(point p, int width,int height,player* pl){

    point np = relative_pos(p,pl);
    double x = np.x;
    double y = np.y;
    double z = np.z;

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

void _fill_triangle_outdated(point_2d A,point_2d B,point_2d C, int w,int h,SDL_Renderer* renderer){
    double width = (double) w;
    double height = (double) h;

    point_2d ps[] = {A,B,C};
    double coefs[6];
    double eq_car_temoins[3];
    point_2d min = A;
    point_2d max = A;
    for (int i = 0;i < 3;i++){
        point_2d curr = ps[i];
        point_2d edge2 = ps[(i+1)%3];
        point_2d third = ps[(i+2)%3];
        if (curr.x < min.x) min.x = curr.x;
        if (curr.y < min.y) min.y = curr.y;
        if (curr.x > max.x) max.x = curr.x;
        if (curr.y > max.y) max.y = curr.y;
        if (curr.x != edge2.x) coefs[2*i] = (edge2.y - curr.y) / (edge2.x - curr.x);
        else coefs[2*i] = 0.0;
        coefs[2*i + 1] = curr.y - curr.x * coefs[2*i];
        eq_car_temoins[i] = coefs[2*i] * third.x - third.y + coefs[2*i + 1];;

    }
   if (min.x < 0) min.x = 0;
   if (min.y < 0) min.y = 0;
   if (max.x >= width) max.x = width;
   if (max.y >= height) max.y = height;

    double facteur_correctif = 50;

    for (double x = min.x ; x< max.x ; x++){
        for (double y = min.y ; y < max.y ; y++){
            bool showing = true;
            for (int i = 0;i < 3;i++){
                double eq_car_point = coefs[2*i] * x - y + coefs[2*i + 1];;
                showing = showing && (eq_car_point * eq_car_temoins[i] >= -facteur_correctif);
            }
            if (showing) {
                //printf("(%f %f) ",x,y);
                SDL_RenderDrawPoint(renderer,(int) x,(int)y);
            }
        }
        //printf("\n");
    }
}

double min(double a,double b){
    if (a > b) return b;
    return a;
}

double max(double a,double b){
    if (a > b) return a;
    return b;
}

void fill_triangle(point_2d A,point_2d B,point_2d C, int max_w, int max_h, SDL_Renderer* renderer){
    double max_height = (double) max_h;
    double max_width = (double) max_w;
    point_2d points[3] = {A,B,C};

    // Sort point
    for (int i = 1;i >= 0;i--){
        for (int j = i;j<2;j++){
            if (points[j].x > points[j+1].x){
                point_2d tmp = points[j];
                points[j] = points[j+1];
                points[j+1] = tmp;
            }
        }
    }
    
    // Si 0-1 Existe
    if (points[0].x != points[1].x){
        double pente_01 = ( points[0].y - points[1].y ) / (points[0].x - points[1].x); //Pente de 0 a 1
        double pente_02 = ( points[0].y - points[2].y ) / (points[0].x - points[2].x); //Pente de 0 a 2

        double k_01 = points[0].y - pente_01*points[0].x; //y = pente_01x + k_01
        double k_02 = points[0].y - pente_02*points[0].x; //y = pente_02x + k_02
        
        //Si 1 est en dessous de 0-2 (le coin est vers le bas)
        if (pente_02*points[1].x + k_02 < points[1].y){

            double min_x = max(points[0].x, 0.0);
            double max_x = min(points[1].x, max_width-1); // Min entre x et le bord de la fenetre (= width si x > width)
            for (double x = min_x; x<=max_x ; x++){ // 0 <= X <= Width
                
                
                double min_y = max(pente_02*x + k_02,0.0);// Y est en dessous de 0-2 donc on veut qu'il soit entre 0-2 et le min entre le bas ( h ) et mr bas du triangle
                double max_y = min(pente_01*x + k_01,max_height-1);
                for (double y = min_y; y <= max_y; y++ ){ // 0 <= Y <= max_height
                    
                    if (x>= 0 && y>=0 && x<max_width && y<max_height) {SDL_RenderDrawPoint(renderer,(int) x,(int)y);}
                    else printf("(%f, %f)", x, y);
                }
            }
        } else { // Y est au dessus de 0-2 ( tete en haut )

            double min_x = max(points[0].x, 0.0);
            double max_x = min(points[1].x,max_width-1);
            for (double x = min_x;x<=max_x ;x++){
                
                double max_y = min(pente_02*x + k_02,max_height-1);/// y est au min sur 0-1 et au max sur 0-2 
                double min_y = max(pente_01*x + k_01,0.0);
                for (double y = min_y;y <= max_y;y++ ){// 0 <= Y <= max_height

                    if (x>= 0 && y>=0 && x<max_width && y<max_height) {SDL_RenderDrawPoint(renderer,(int) x,(int)y);}
                    else printf("[%f, %f]", x, y);
                }
            }
        }
    } 
    // Si 1-2 existe
    if (points[1].x != points[2].x){
        double pente_12 = ( points[2].y - points[1].y ) / (points[2].x - points[1].x);
        double pente_02 = ( points[0].y - points[2].y ) / (points[0].x - points[2].x);
        double k_12 = points[2].y - pente_12 *points[2].x; //y = pente_01x + k_01
        double k_02 = points[0].y - pente_02 *points[0].x; //y = pente_02x + k_02

        if (pente_02*points[1].x + k_02 < points[1].y){//toujours y tete en bas
            
            double max_x = min(points[2].x,max_width-1);
            double min_x = max(points[1].x,0.0);
            for (double x = min_x ;x<=max_x ;x++){
                
                double max_y = min(max_height-1,pente_12*x + k_12);
                double min_y = max(0.0,pente_02*x + k_02);
                for (double y = min_y;y <= max_y;y++ ){
                    
                    if (x>= 0 && y>=0 && x<max_width && y<max_height) {SDL_RenderDrawPoint(renderer,(int) x,(int)y);}
                    else printf("~%f, %f~", x, y);
                }
            }
        } else {

            double max_x = min(points[2].x,max_width-1);
            double min_x = max(points[1].x,0.0);
            for (double x = min_x;x<=max_x ;x++){
                
                double max_y = min(max_height-1,pente_02*x + k_02);
                double min_y = max(0.0,pente_12*x + k_12);
                for (double y =min_y;y <= max_y;y++ ){
                    
                    if (x>= 0 && y>=0 && x<max_width && y<max_height) {SDL_RenderDrawPoint(renderer,(int) x,(int)y);}
                    else printf("{%f, %f}", x, y);
                }
            }
        }
    }
}



void show_polygon(player* pl , int width,int height, polygon pol){

    point_2d* p_list = malloc(sizeof(point_2d) * pol.len);
    for (int i = 0; i< pol.len; i++){
        //printf("%d \n",i);
        p_list[i] = projection(pol.vertices[i],width,height,pl);
        //printf("passé1\n");
        if (i >= 2) {
            if (DEBUG_SHOW_POLYGON) { //change color
                    pol.col.r *=0.85;
                    pol.col.g *=0.85;
                    pol.col.b *=0.85;
                }
            SDL_SetRenderDrawColor(pl->renderer,pol.col.r,pol.col.g,pol.col.b,1);
            fill_triangle(p_list[0],p_list[i-1],p_list[i],width,height,pl->renderer);
        }
    }
    free(p_list);
}