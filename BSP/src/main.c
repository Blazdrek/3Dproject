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
    SDL_SetRenderDrawColor(renderer, 0, 255 , 255, 1);

    player* pl = create_player(120, renderer);

    plane p_horizontal = (plane) {0,0,1,0};
    plane p_vertical = (plane) {1,0,0,0};
    plane p_vert2 = (plane) {1,0,0,-50};

    polygon square1;
    square1.len = 4;
    square1.p = p_horizontal;
    square1.vertices = malloc(sizeof(point)*4);
    square1.vertices[0] = (point) {45,-5,0};
    square1.vertices[1] = (point) {55,-5,0};
    square1.vertices[2] = (point) {55,5,0};
    square1.vertices[3] = (point) {45,5,0};
    square1.col = (color) {255,0,0};

    polygon sq2;
    sq2.len = 4;
    sq2.p = p_vert2;
    sq2.vertices = malloc(sizeof(point)*4);
    sq2.vertices[0] = (point) {50,-5,-5};
    sq2.vertices[1] = (point) {50,-5,5};
    sq2.vertices[2] = (point) {50,5,5};
    sq2.vertices[3] = (point) {50,5,-5};
    sq2.col = (color) {0,255,0};

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
    append(plist,sq2);
    build_BSP_tree_v1(bspt,plist);

    bool running = true;
    bool has_moved = true;
    SDL_Event event;

    double pspeed = 1.0;

    SDL_RenderPresent(renderer);



    while (running){
        while (SDL_PollEvent(&event)) {
            SDL_Keycode key = event.key.keysym.sym;
            if (event.type == SDL_KEYDOWN) {
                SDL_Keycode key = event.key.keysym.sym;
                if (key == SDLK_ESCAPE) running = false;   
            }

            switch (key)
            {
            
            case SDLK_z:
                has_moved = true;
                printf("%f %f %f\n",pl->coord.x,pl->coord.y,pl->angle_z);
                pl->coord.x += pspeed * cos(pl->angle_z);
                pl->coord.y -= pspeed * sin(pl->angle_z);
                printf("%f %f %f\n",pl->coord.x,pl->coord.y,pl->angle_z);
                break;
            case SDLK_s:
                has_moved = true;
                pl->coord.x -= pspeed * cos(pl->angle_z);
                pl->coord.y += pspeed * sin(pl->angle_z);
                break;

            case SDLK_d:
                has_moved = true;
                pl->coord.y += pspeed;
                break;
            case SDLK_q:
                has_moved = true;
                pl->coord.y -= pspeed;
                break;

            case SDLK_a:
                has_moved = true;
                pl->angle_z += 0.02;
                break;

            case SDLK_e:
                has_moved = true;
                pl->angle_z -= 0.02;
                break;

            case SDLK_r:
                has_moved = true;
                pl->angle_y += 0.02;
                break;

            case SDLK_f:
                has_moved = true;
                pl->angle_y -= 0.02;
                break;

            case SDLK_SPACE:
                has_moved = true;
                pl->coord.z += 0.2;

            // case SDLK_LCTRL:
            //     has_moved = true;
            //     printf("DOWN\n");
            //     pl->coord.z -= 0.2;

            default:
                break;
            }            
        }
        if (has_moved){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); 
            show_BSP_tree(bspt,width,height,pl);
            SDL_RenderPresent(renderer);
            // for (int i = 0;i<4;i++){
            //     point r = relative_pos(sq2.vertices[i],pl);
            //     printf("%f %f %f\n",r.x,r.y,r.z);
            //     point_2d p2d = projection(r,width,height,pl);
            //     printf("%f %f\n",p2d.x,p2d.y);
            // }
        }
        has_moved = false;
    }


    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Fermeture du programme \n");
    fflush(stdout);

    return 0;
}