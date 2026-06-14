#include "main.h"

int main(){
    
    SDL_Init(SDL_INIT_VIDEO);
    int width = 1000;
    int height = 1000;
    color black = (color) {0,0,0};
    
    //Initialisation de la fenetre / de l'arbre bsp / parsing polygone list
    SDL_Window* window = SDL_CreateWindow("SDL sous WSL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 255 , 255, 1);

    player* pl = create_player(120, renderer);
    display *main_display = display_create(width,height);
    display *diff_display = display_create(width,height);
    display_fill(main_display,black);
    display_fill(diff_display,black);


    bsp_tree* bspt = create_tree();
    polygon_list* plist = parse_file("polygons2.txt");
    build_BSP_tree_v1(bspt,plist);

    bool running = true;
    bool has_moved = true;
    SDL_Event event;

    time_t nb_frame = 0;
    time_t time_running= time(NULL);
    double pspeed = 1.0;

    SDL_RenderPresent(renderer);

    

    while (running){
        nb_frame++;
        if (time(NULL)-time_running == 1){
            printf("%ld FPS\n",nb_frame);
            nb_frame = 0;
            time_running = time(NULL);
        }
        nb_frame++;
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
                pl->coord.x += pspeed * cos(pl->angle_z);
                pl->coord.y -= pspeed * sin(pl->angle_z);
                break;
            case SDLK_s:
                has_moved = true;
                pl->coord.x -= pspeed * cos(pl->angle_z);
                pl->coord.y += pspeed * sin(pl->angle_z);
                break;

            case SDLK_d:
                has_moved = true;
                pl->coord.x += pspeed * sin(pl->angle_z);
                pl->coord.y += pspeed * cos(pl->angle_z);
                break;
            case SDLK_q:
                has_moved = true;
                pl->coord.x -= pspeed * sin(pl->angle_z);
                pl->coord.y -= pspeed * cos(pl->angle_z);
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
                break;

            case SDLK_c:
                has_moved = true;
                pl->coord.z -= 0.2;
                break;

            default:
                break;
            }            
        }
        if (true){
            SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); 
            show_BSP_tree(bspt,main_display,pl);
            display_compare_and_show(pl->renderer,main_display,diff_display);
            display_copy(main_display,diff_display);
            display_fill(main,black);
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