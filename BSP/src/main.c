#include <SDL2/SDL.h>
#include <SDL_render.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include "geometry.h"
#include "bsptree_display.h"

int main(){

    SDL_Init(SDL_INIT_VIDEO) < 0;
    int width = 1000;
    int height = 1000;
    
    
    SDL_Window* window = SDL_CreateWindow("SDL sous WSL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 255 , 255, 255);

    

    plane p_horizontal = (plane) {0,0,1,0};
    plane p_vertical = (plane) {1,0,0,0};

    polygon square1;
    square1.len = 4;
    square1.p = p_horizontal;
    square1.vertices = malloc(sizeof(point)*4);
    square1.vertices[0] = (point) {-5,0,0};
    square1.vertices[1] = (point) {-5,10,0};
    square1.vertices[2] = (point) {5,10,0};
    square1.vertices[3] = (point) {5,0,0};


    polygon rect1;
    rect1.len = 4;
    rect1.p = p_vertical;
    rect1.vertices = malloc(sizeof(point)*4);
    rect1.vertices[0] = (point) {0,0,-5};
    rect1.vertices[1] = (point) {0,5,-5};
    rect1.vertices[2] = (point) {0,5,5};
    rect1.vertices[3] = (point) {0,0,5};

    bsp_tree* bspt = create_tree();

    polygon_list* plist = create_list();
    append(plist,square1);
    append(plist,rect1);
    build_BSP_tree_v1(bspt,plist);


    // polygon* front = malloc(sizeof(polygon));
    // polygon* back = malloc(sizeof(polygon));

    // polygon* s = malloc(sizeof(polygon));
    // *s = square1;

    
    
    // split_polygon(p_vertical,s,front,back);


    

    //point_2d A = {350.0,150.0};
    //point_2d B = {15.0,400.0};
    //point_2d C = {60.0,15.0};
    //fill_triangle(A,B,C,renderer);
    

   


    bool running = true;
    SDL_Event event;

    SDL_RenderPresent(renderer);

    while (running){
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                SDL_Keycode key = event.key.keysym.sym;
                if (key == SDLK_ESCAPE) running = false;   
            }
        }   
    }

    
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Fermeture du programme \n");
    fflush(stdout);

    return 0;
}