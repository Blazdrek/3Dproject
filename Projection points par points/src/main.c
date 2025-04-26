#include <SDL2/SDL.h>
#include <stdio.h>

int main(){

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur SDL : %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL sous WSL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    
    SDL_Delay(3000);
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}