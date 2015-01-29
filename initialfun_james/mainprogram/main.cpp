#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "res_path.h"

#define WINDOW_WIDTH 1300
#define WINDOW_HEIGHT 800

int main(int argc, char** argv){
	
	// initialize window, renderer, textures
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Initial fun", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr){
        std::cerr << "SDL_CreateWindow error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr){
        std::cerr << "SDL_CreateRenderer error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    std::string resource_path = getResourcePath("");
    std::string charfile = resource_path + "initialcharacter.png";
    std::string bgfile = resource_path + "initialbackgroundtile.png";
    SDL_Texture* character_texture = IMG_LoadTexture(renderer, charfile.c_str());
    SDL_Texture* bgtile_texture = IMG_LoadTexture(renderer, bgfile.c_str());
    if (character_texture == nullptr || bgtile_texture == nullptr){
        std::cerr << "IMG_LoadTexture error: " << SDL_GetError() << std::endl;
        SDL_DestroyTexture(character_texture);
        SDL_DestroyTexture(bgtile_texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Event event;
    SDL_Rect player_position;
    player_position.x = 300;
    player_position.y = 300;
    player_position.w = 50;
    player_position.h = 50;
    bool quitnow = false;
    int dx = 6;
    int dy = 14;
	int clickx, clicky;
	double forcex = 0, forcey = 0;
	double zoomfactor = 0.5;
    while(!quitnow){
        while (SDL_PollEvent(&event)){
             if (event.type == SDL_KEYDOWN){
                switch(event.key.keysym.sym){
/*                 case SDLK_w:
                    dy = -player_speed;
                    break;
                 case SDLK_s:
                    dy = player_speed;
                    break;
                 case SDLK_a:
                    dx = -player_speed;
                    break;
                 case SDLK_d:
                    dx = player_speed;
                    break;
*/                case SDLK_ESCAPE:
                    quitnow = true;
                    break;
                default:
                    break;
                }
            }
/*			else if (event.type == SDL_KEYUP){
                switch(event.key.keysym.sym){
                 case SDLK_w:
                    dy = 0;
                    break;
                 case SDLK_s:
                    dy = 0;
                    break;
                 case SDLK_a:
                    dx = 0;
                    break;
                 case SDLK_d:
                    dx = 0;
                    break;
                default:
                    break;
                }
            }
*/			 else if (event.type == SDL_MOUSEBUTTONDOWN){
				clickx = event.button.x;
				clicky = event.button.y;
			 }
			 else if (event.type == SDL_MOUSEBUTTONUP){
				forcex = 0.1*(clickx - event.button.x);
				forcey = 0.1*(clicky - event.button.y);
			 }
			 else if (event.type == SDL_MOUSEWHEEL){
				//event.wheel.direction,x,y
				 zoomfactor += event.wheel.y*0.02;
			 }
            else if (event.type == SDL_QUIT){
                quitnow = true;
            }
        }
		player_position.y += dy;
		player_position.x += dx;
	
		int mapwidth = WINDOW_WIDTH*zoomfactor; 
		int mapheight = WINDOW_HEIGHT*zoomfactor; 
		int buff = 100*zoomfactor;
		int tilewidth = 50*zoomfactor;
		player_position.w = 50*zoomfactor;
		player_position.h = 50*zoomfactor;
		int player_width = 50*zoomfactor;


		if (player_position.x < buff){
			dx = -dx + forcex;
			dy =  dy + forcey;
			player_position.x += 2*dx;
			forcex = 0;
			forcey = 0;
		}
		else if (player_position.x+player_width > mapwidth-buff){
			dx = -dx + forcex;
			dy =  dy + forcey;
			player_position.x += 2*dx;
			forcex = 0;
			forcey = 0;
		}
		else if (player_position.y < buff){
			dy = -dy + forcey;
			dx =  dx + forcex;
			player_position.y += 2*dy;
			forcex = 0;
			forcey = 0;
		}
		else if (player_position.y+player_width > mapheight-buff){
			dy = -dy + forcey;
			dx =  dx + forcex;
			player_position.y += 2*dy;
			forcex = 0;
			forcey = 0;
		}


        SDL_RenderClear(renderer);
	
        for (int i = buff; i < mapheight-buff; i += tilewidth){
            for (int j = buff; j < mapwidth-buff; j += tilewidth){
                SDL_Rect dst;
                dst.y = i;
                dst.x = j;
                dst.w = tilewidth;
                dst.h = tilewidth;
                SDL_RenderCopy(renderer, bgtile_texture, NULL, &dst);
            }
        }
        SDL_RenderCopy(renderer, character_texture, NULL, &player_position);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(character_texture);
    SDL_DestroyTexture(bgtile_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
