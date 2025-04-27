#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "geometry.h"

void show_points(int len,point* liste_p,SDL_Window* window,SDL_Renderer* renderer,double FOV){
    for (int i = 0;i<len;i++){
        point p = liste_p[i];
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

void move(int len,point* liste_p,double x,double y,double z){
    for (int i = 0;i < len;i++){
        liste_p[i].x = liste_p[i].x + x;
        liste_p[i].y = liste_p[i].y + y;
        liste_p[i].z = liste_p[i].z + z;
    }
    
}

void rotate_z(int len,point* liste_p,double angle){
    for (int i = 0; i < len; i++){
        liste_p[i].x = liste_p[i].x * cos(angle) - liste_p[i].y * sin(angle);
        liste_p[i].y = liste_p[i].x * sin(angle) + liste_p[i].y * cos(angle);
    }
}


int main(){

    SDL_Init(SDL_INIT_VIDEO) < 0;
    int width = 1280;
    int height = 720;
    
    SDL_Window* window = SDL_CreateWindow("SDL sous WSL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);


    
    int nb_points = 8;
    point* liste_points = cube(100,-6,0.0,12);
    for (int i = 0;i<8;i++){
        point_2d pos = projection_v2(liste_points[i],width,height,1.5);;
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
                    printf(" FOREWARD \n");
                    fflush(stdout);
                    move(8,liste_points,-2,0,0);
                }
                if (key == SDLK_s) {
                    printf("BACKWARD \n");
                    fflush(stdout);
                    move(8,liste_points,2,0,0);
                    
                }
                if (key == SDLK_d) {
                    printf("RIGHT \n");
                    fflush(stdout);
                    move(8,liste_points,0,-2,0);
                }
                if (key == SDLK_q) {
                    printf("LEFT \n");
                    fflush(stdout);
                    move(8,liste_points,0,2,0);
                }
                if (key == SDLK_a) {
                    printf("ROTATE LEFT \n");
                    fflush(stdout);
                    rotate_z(8,liste_points,0.2);
                }
                if (key == SDLK_e) {
                    printf("ROTATE RIGHT \n");
                    fflush(stdout);
                    rotate_z(8,liste_points,-0.2);
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        show_points(nb_points,liste_points,window,renderer,1.5);
        
        SDL_RenderPresent(renderer);


        
    }
    


    
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Fermeture du programme \n");
    fflush(stdout);

    return 0;
}