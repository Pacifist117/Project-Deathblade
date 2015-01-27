#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "res_path.h"

int main(int argc, char** argv){
	
	// initialize window, renderer, textures
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Initial fun", 100, 100, 650, 500, SDL_WINDOW_SHOWN);
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
	bool trackingnow = false;
    int dx = 0;
    int dy = 0;
	int player_speed = 10;
    while(!quitnow){
        while (SDL_PollEvent(&event)){
             if (event.type == SDL_KEYDOWN){
                switch(event.key.keysym.sym){
                 case SDLK_w:
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
                case SDLK_ESCAPE:
                    quitnow = true;
                    break;
                default:
                    break;
                }
            }
			else if (event.type == SDL_KEYUP){
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
			 else if (event.type == SDL_MOUSEBUTTONDOWN){
				trackingnow = true;

			 }
			 else if (event.type == SDL_MOUSEBUTTONUP){
				 trackingnow = false;
			 }
            else if (event.type == SDL_QUIT){
                quitnow = true;
            }
        }

		if (trackingnow){
			player_position.x = event.button.x - 25;
			player_position.y = event.button.y - 25;
		}
		else{
			if (dx != 0 && dy != 0){
				dx = 0.7074*dx;
				dy = 0.7074*dy;
			}
			player_position.y += dy;
			player_position.x += dx;
		}

        SDL_RenderClear(renderer);

        for (int i = 0; i < 500; i += 50){
            for (int j = 0; j < 650; j += 50){
                SDL_Rect dst;
                dst.y = i;
                dst.x = j;
                dst.w = 50;
                dst.h = 50;
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
