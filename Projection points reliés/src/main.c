#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "geometry_points_relies.h"

void show_obj(object ob,SDL_Window* window,SDL_Renderer* renderer,double FOV){
    for (int i = 0;i<ob.nb_points;i++){
        point p = ob.points[i];
        if (p.x > 0){
            int width;
            int height;
            SDL_GetWindowSize(window,&width,&height);
            point_2d pos = projection_v2(p,width,height,FOV);
            if ((double) 0 <pos.x && pos.x < (double) width && (double) 0<pos.y && pos.y < (double) height ){
                SDL_RenderDrawPoint(renderer,(int) pos.x ,(int) pos.y);
            }
        }
    }
}

void move(object ob,double x,double y,double z){
    for (int i = 0;i < ob.nb_points;i++){
        ob.points[i].x = ob.points[i].x + x;
        ob.points[i].y = ob.points[i].y + y;
        ob.points[i].z = ob.points[i].z + z;
    }
    
}

void rotate_z(object ob,double angle){
    for (int i = 0; i < ob.nb_points; i++){
        ob.points[i].x = ob.points[i].x * cos(angle) - ob.points[i].y * sin(angle);
        ob.points[i].y = ob.points[i].x * sin(angle) + ob.points[i].y * cos(angle);
    }
}


int main(){

    SDL_Init(SDL_INIT_VIDEO) < 0;
    int width = 1280;
    int height = 720;
    
    SDL_Window* window = SDL_CreateWindow("SDL sous WSL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);


    
    object cube = make_cube(100,-6,-6,12);
    for (int i = 0;i<cube.nb_points;i++){
        point_2d pos = projection_v2(cube.points[i],width,height,1.5);
        printf("%f %f \n ",pos.x,pos.y);
    }
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
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        show_obj(cube,window,renderer,2);
        
        SDL_RenderPresent(renderer);


        
    }
    


    
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Fermeture du programme \n");
    fflush(stdout);

    return 0;
}