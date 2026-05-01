#include "bsptree_display.h"

bool DEBUG_SPLIT_TREE = true;


bsp_tree* create_tree(){
    bsp_tree* t = malloc(sizeof(bsp_tree));
    t->back = NULL;
    t->front = NULL;
    return t;
}

void build_BSP_tree_v1(bsp_tree* t,polygon_list* list){
    if (list->size == 1) {
        t->back = NULL;
        t->front = NULL;
        t->coincidents = list;
        t->p = get(list,0).p;
    } else {
        t->coincidents = create_list();
        polygon root = pop(list);
        t->p = root.p;
        append(t->coincidents,root);

        polygon_list* front = create_list();
        polygon_list* back  = create_list();

        polygon* front_pol = malloc(sizeof(polygon));
        polygon* back_pol = malloc(sizeof(polygon));

        for (int i = 0 ; i < list->size;i++){
            
            polygon pol = get(list,i);
            
            if (belong_to_plane(t->p,pol.vertices[0]) == 0 &&  belong_to_plane(t->p,pol.vertices[1]) == 0 && belong_to_plane(t->p,pol.vertices[2]) == 0) append(t->coincidents,pol);
            else{
                split_polygon(t->p,&pol,front_pol,back_pol);

                if (DEBUG_SPLIT_TREE) { //change color
                    back_pol->col.r *=0.85;
                    back_pol->col.g *=0.85;
                    back_pol->col.b *=0.85;
                }

                if (back_pol->len >0) append(back,*back_pol);
                if (front_pol->len >0)append(front,*front_pol);
            }
        }
        if (back->size >= 1){ 
            t->back = create_tree();
            build_BSP_tree_v1(t->back,back);
        }
        if (front->size >= 1){ 
            t->front = create_tree();
            build_BSP_tree_v1(t->front,front);
        }
    }
}

void print_bsp_tree(bsp_tree* t){
    print_pol_list(t->coincidents);
    if (t->front != NULL) print_bsp_tree(t->front);
    if (t->back != NULL) print_bsp_tree(t->back);
}

void show_BSP_tree(bsp_tree* t,int width,int height,player* pl){
    if (t==NULL) return;
    if (t->back == NULL && t->front == NULL){
        show_polygon(pl,width,height,get(t->coincidents,0));
    }
    else {
        double classify_p = belong_to_plane(t->p,pl->coord);
        if (classify_p > 0){
            show_BSP_tree(t->back,width,height,pl);
            for (int i = 0;i < t->coincidents->size;i++) show_polygon(pl,width,height,get(t->coincidents,i));
            show_BSP_tree(t->front,width,height,pl);
        } else if (classify_p < 0){
            show_BSP_tree(t->front,width,height,pl);
            for (int i = 0;i < t->coincidents->size;i++) show_polygon(pl,width,height,get(t->coincidents,i));
            show_BSP_tree(t->back,width,height,pl);
        } else {
            show_BSP_tree(t->front,width,height,pl);
            show_BSP_tree(t->back,width,height,pl);
        }
    }
}