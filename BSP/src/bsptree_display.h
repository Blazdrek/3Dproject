#ifndef _BSPTREE_DISPLAY_H_
#define _BSPTREE_DISPLAY_H_


typedef struct bsp_tree_s {
    struct bsp_tree_s* front;
    struct bsp_tree_s* back;
    plane p;
    polygon_list* coincidents;
    int len;
} bsp_tree;

bsp_tree* create_tree();
void build_BSP_tree_v1(bsp_tree* t,polygon_list* list);


#endif