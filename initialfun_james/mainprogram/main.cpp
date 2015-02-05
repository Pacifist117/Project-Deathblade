
#include "graphicscontrol/cameracontrol.h"
#include "gameobjects/basicwall.h"
#include "res_path.h"

#include "SDL_image.h"
#include "SDL2_gfxPrimitives.h"
#include "SDL.h"

#include <iostream>
#include <cmath>


double fx = 0;
double fy = 0;
double getx();
double gety();

void drawpolygon(SDL_Renderer* renderer, CameraControl* cam, double x, double y, std::vector<std::vector<double> > &relpoints);

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

    const double tilew = 0.5;
    const double tileh = 0.5;
	const int tilepw = 50;
	const int tileph = 50;
    double mousex = gamesettings.mapmidx;
    double mousey = gamesettings.mapmidy;

    BasicWall bottomwall, topwall, leftwall, rightwall;
    bottomwall.create(0,gamesettings.maph,gamesettings.mapw, 0.1);
    bottomwall.set_color(120,120,120);

    topwall.create(0,0,gamesettings.mapw, -0.1);
    topwall.set_color(120,120,120);

    leftwall.create(0,0,-0.1,gamesettings.maph);
    leftwall.set_color(120,120,120);

    rightwall.create(gamesettings.mapw,0,0.1,gamesettings.maph);
    rightwall.set_color(120,120,120);


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
                    mousex = camera.xfrompixel(event.motion.x,db::Player);
                    mousey = camera.yfrompixel(event.motion.y,db::Player);
				}
			}
			else if (event.type == SDL_QUIT){
				quitnow = true;
			}
	
		}

        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
		SDL_RenderClear(renderer);

		camera.adjust_zoom(zoomdirection, mousex, mousey);
		
		for (double x = gamesettings.mapx; x < gamesettings.mapx+gamesettings.mapw; x += tilew){
			for (double y = gamesettings.mapy; y < gamesettings.mapy+gamesettings.maph; y += tileh){
                SDL_Rect dst = camera.calculate_display_destination(x,y,tilew,tileh,db::Floor);
				SDL_RenderCopy(renderer, bgtile_texture, NULL, &dst);
			}
		}



        SDL_Rect dst = camera.calculate_display_destination(getx(),gety(),0.05,0.05,db::Player);
        SDL_RenderCopy(renderer, character_texture, NULL, &dst);

        drawpolygon(renderer,&camera,bottomwall.x,bottomwall.y,bottomwall.bounding_points);
        drawpolygon(renderer,&camera,topwall.x,topwall.y,topwall.bounding_points);
        drawpolygon(renderer,&camera,leftwall.x,leftwall.y,leftwall.bounding_points);
        drawpolygon(renderer,&camera,rightwall.x,rightwall.y,rightwall.bounding_points);

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


void drawpolygon(SDL_Renderer* renderer, CameraControl* cam, double x, double y, std::vector<std::vector<double> >& relpoints){
    std::vector<Sint16> vx; vx.reserve(relpoints.size());
    std::vector<Sint16> vy; vy.reserve(relpoints.size());
    for(unsigned int i = 0; i < relpoints.size(); ++i){
        vx[i] = cam->pixelfromx(x+relpoints[i][0],db::Player);
        vy[i] = cam->pixelfromy(y+relpoints[i][1],db::Player);
    }
    filledPolygonRGBA(renderer,vx.data(),vy.data(),relpoints.size(),100,100,100,255);
}
