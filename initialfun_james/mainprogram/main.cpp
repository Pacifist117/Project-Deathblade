#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "res_path.h"

#include <cmath>
#include "graphicscontrol/cameracontrol.h"

double fx = 0;
double fy = 0;
double getx();
double gety();

int main(){

	TempSettings gamesettings;

	gamesettings.mapw = 5;
	gamesettings.maph = 3;
	gamesettings.mapx = 0;
	gamesettings.mapy = 0;
    gamesettings.mapmidx = gamesettings.mapw/2.0;
    gamesettings.mapmidy = gamesettings.maph/2.0;
	gamesettings.window_width = 1300;
	gamesettings.window_height = 800;
	CameraControl camera(&gamesettings);

	const double tilew = 0.05;
	const double tileh = 0.05;
	const int tilepw = 50;
	const int tileph = 50;
    double mousex = gamesettings.mapmidx;
    double mousey = gamesettings.mapmidy;


// initialize window, renderer, textures
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("deathblade_floating", 0, 0, gamesettings.window_width, gamesettings.window_height, SDL_WINDOW_SHOWN);
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
	bool quitnow = false;
	while(!quitnow){
		
		int zoomdirection = 0;

		while(SDL_PollEvent(&event)){
			if (event.type == SDL_KEYDOWN){
				switch(event.key.keysym.sym){
					case SDLK_ESCAPE:
						quitnow = true;
						break;
					default:
						break;
				}
			}
			else if (event.type == SDL_KEYUP){

			}
			else if (event.type == SDL_MOUSEBUTTONDOWN){
                if (event.button.button == SDL_BUTTON_LEFT)
                    camera.mousecontrol_on();
                //if (event.button.button == SDL_BUTTON_RIGHT)

			}
			else if (event.type == SDL_MOUSEBUTTONUP){
                if (event.button.button == SDL_BUTTON_LEFT)
                    camera.mousecontrol_off();
			}
			else if (event.type == SDL_MOUSEWHEEL){
                zoomdirection += event.wheel.y;
			}
			else if (event.type == SDL_MOUSEMOTION){

				if (camera.mouse_controlling()){
					camera.mousecontrol_move(event.motion.xrel, event.motion.yrel);
				}
				else{
					mousex = camera.xfrompixel(event.motion.x,CameraControl::ZPlane::Player);
					mousey = camera.yfrompixel(event.motion.y,CameraControl::ZPlane::Player);
				}
			}
			else if (event.type == SDL_QUIT){
				quitnow = true;
			}
	
		}

		SDL_RenderClear(renderer);

		camera.adjust_zoom(zoomdirection, mousex, mousey);
		
		for (double x = gamesettings.mapx; x < gamesettings.mapx+gamesettings.mapw; x += tilew){
			for (double y = gamesettings.mapy; y < gamesettings.mapy+gamesettings.maph; y += tileh){
				SDL_Rect dst = camera.calculate_display_destination(x,y,tilew,tileh,CameraControl::ZPlane::Floor);
				SDL_RenderCopy(renderer, bgtile_texture, NULL, &dst);
			}
		}

		SDL_Rect dst = camera.calculate_display_destination(getx(),gety(),0.05,0.05,CameraControl::ZPlane::Player);
		SDL_RenderCopy(renderer, character_texture, NULL, &dst);

		SDL_RenderPresent(renderer);
	}
    SDL_DestroyTexture(character_texture);
	SDL_DestroyTexture(bgtile_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
	

double getx(){
	static double x = 0.5;
	static double dx = 0.04;

	x += dx;

	if (x >= 5 || x <= 0)
        dx -= 2*dx + fx;
	
	return x;
}

double gety(){
	static double y = 0.3;
	static double dy = 0.02;

	y += dy;

	if (y >= 3 || y <= 0)
        dy -= 2*dy + fy;

	return y;
}
