#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "geometry_points_relies.h"

int main(){

    SDL_Init(SDL_INIT_VIDEO) < 0;
    int width = 1000;
    int height = 1000;
    
    
    SDL_Window* window = SDL_CreateWindow("SDL sous WSL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    player* p1 = create_player(1.8,renderer);
    
    
    object cube = (make_cube(100,0,-6,12));
    bool running = true;
    bool moves_log = false;
    SDL_Event event;
    while (running){
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                SDL_Keycode key = event.key.keysym.sym;
                if (key == SDLK_ESCAPE) running = false;
                if (key == SDLK_l) moves_log = ! moves_log;

///Movements
                if (key == SDLK_z) {
                    if (moves_log) printf("FOREWARD \n");
                    fflush(stdout);
                    move(cube,-2,0,0);
                }
                if (key == SDLK_s) {
                    if (moves_log) printf("BACKWARD \n");
                    fflush(stdout);
                    move(cube,2,0,0);
                    
                }
                if (key == SDLK_d) {
                    if (moves_log) printf("RIGHT \n");
                    fflush(stdout);
                    move(cube,0,-2,0);
                }
                if (key == SDLK_q) {
                    if (moves_log) printf("LEFT \n");
                    fflush(stdout);
                    move(cube,0,2,0);
                }
                if (key == SDLK_a) {
                    if (moves_log) printf("ROTATE LEFT \n");
                    fflush(stdout);
                    rotate_z(cube,0.02);
                }
                if (key == SDLK_e) {
                    if (moves_log) printf("ROTATE RIGHT \n");
                    fflush(stdout);
                    rotate_z(cube,-0.02);
                }
                if (key == SDLK_r) {
                    if (moves_log) printf("ROTATE UP \n");
                    fflush(stdout);
                    rotate_y(cube,-0.02);
                }
                if (key == SDLK_f) {
                    if (moves_log) printf("ROTATE DOWN \n");
                    fflush(stdout);
                    rotate_y(cube,0.02);
                }

///Prints cube coords
                if (key == SDLK_p){
                    print_coords(cube,width,height,p1);
                }
            }
        }
        
        SDL_SetRenderDrawColor(p1->renderer, 0, 0, 0, 255);
        SDL_RenderClear(p1->renderer);
        SDL_SetRenderDrawColor(p1->renderer, 255, 255, 255, 255);
        show_obj(cube,window,p1);
        
        SDL_RenderPresent(renderer);


        
    }
    

    
    
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Fermeture du programme \n");
    fflush(stdout);

    return 0;
}