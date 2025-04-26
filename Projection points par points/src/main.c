#include <SDL2/SDL.h>
#include <stdio.h>



int main(){

    SDL_Init(SDL_INIT_VIDEO) < 0;
    int width = 1000;
    int height = 800;
    SDL_Window* window = SDL_CreateWindow("SDL sous WSL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    
    SDL_Delay(3000);
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}