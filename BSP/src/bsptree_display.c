#include <SDL2/SDL.h>
#include "geometry.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>


typedef struct bsp_tree_s {
    struct bsp_tree_s* front;
    struct bsp_tree_s* back;
    plane p;
    polygon_list* coincidents;
    int len;
} bsp_tree;

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
    } else {
        t->coincidents = create();
        polygon root = pop(list);
        t->p = root.p;
        append(t->coincidents,root);

        polygon_list* front = create();
        polygon_list* back  = create();
        for (int i = 0 ; i < list->size;i++){
            polygon pol = get(list,i);
            if (scalar(get_orthogonal(t->p),get_orthogonal(pol.p)) == 0 && belong_to_plane(t->p,pol.vertices[0])==0) append(t->coincidents,pol);
            else split_polygon(t->p,pol,front,back);
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

//void show_BSP_tree(bsp_tree* t,)