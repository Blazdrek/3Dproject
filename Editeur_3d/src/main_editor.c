#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include "geometry_editor.h"

void show_obj(object ob,SDL_Window* window,SDL_Renderer* renderer,double FOV){
    point_2d* coords = malloc(sizeof(point_2d)*ob.nb_points);
    int width;
    int height;
    SDL_GetWindowSize(window,&width,&height);
    for (int i = 0; i<ob.nb_points; i++) coords[i] = projection_v2(ob.points[i],width,height,FOV);


    for (int i = 0; i<ob.nb_points; i++) {
        point p = ob.points[i];
        for (int j = 1; j<=ob.graph[i][0]; j++){
            int indice = ob.graph[i][j];
            point voisin = ob.points[indice];
            double tan_fov = tan(FOV/2);
            if ((voisin.x > 0 && voisin.y/voisin.x < tan_fov)  || (p.x > 0 && p.y/p.x < tan_fov)){
                SDL_RenderDrawLine(renderer, coords[i].x, coords[i].y,coords[indice].x, coords[indice].y);
            }
        }
    }
    free(coords);
}



int main(){

    SDL_Init(SDL_INIT_VIDEO) < 0;
    int width = 1000;
    int height = 1000;
    double FOV = 2;
    
    SDL_Window* window = SDL_CreateWindow("SDL sous WSL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);


    open_obj("assets/coucou");

    
    object cube = (make_cube(100,0,-6,12));
    bool running = true;
    SDL_Event event;
    while (running){
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                SDL_Keycode key = event.key.keysym.sym;
                if (key == SDLK_ESCAPE) running = false;
                if (key == SDLK_z) {
                    printf("FOREWARD \n");
                    fflush(stdout);
                    move(cube,-2,0,0);
                }
                if (key == SDLK_s) {
                    printf("BACKWARD \n");
                    fflush(stdout);
                    move(cube,2,0,0);
                    
                }
                if (key == SDLK_d) {
                    printf("RIGHT \n");
                    fflush(stdout);
                    move(cube,0,-2,0);
                }
                if (key == SDLK_q) {
                    printf("LEFT \n");
                    fflush(stdout);
                    move(cube,0,2,0);
                }
                if (key == SDLK_a) {
                    printf("ROTATE LEFT \n");
                    fflush(stdout);
                    rotate_z(cube,0.02);
                }
                if (key == SDLK_e) {
                    printf("ROTATE RIGHT \n");
                    fflush(stdout);
                    rotate_z(cube,-0.02);
                }
                if (key == SDLK_r) {
                    printf("ROTATE UP \n");
                    fflush(stdout);
                    rotate_y(cube,-0.02);
                }
                if (key == SDLK_f) {
                    printf("ROTATE DOWN \n");
                    fflush(stdout);
                    rotate_y(cube,0.02);
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        show_obj(cube,window,renderer,FOV);
        
        SDL_RenderPresent(renderer);


        
    }
    


    
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Fermeture du programme \n");
    fflush(stdout);

    return 0;
}