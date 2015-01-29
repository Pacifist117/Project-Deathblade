#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "res_path.h"

#include <cmath>
/*
	Learned Functions

	=== Map X coordinate to Pixel location ===
	Pixel_Location = Window_Width/2 + (X - CameraX)*(currentPixelsPerX)

	where currentPixelsPerX is xtop*zoomfactor below

	=== Reverse of above ===
	X = (Pixel_Location - Window_Width/2)/(currentPixelsPerX) + CameraX

	=== How zoom affects camerax ===
	New_camerax = (CameraX - MouseX)(zoomfactor_new/zoomfactor_old) + MouseX

*/
#define WINDOW_WIDTH 1300
#define WINDOW_HEIGHT 800

d
double calculatezoom(int mousedirection, double currentzoom){

	double camerath = 2*atan(750.0/currentzoom);
	camerath -= 0.25*mousedirection;
	if (camerath < minth) camerath = minth;
	if (camerath > maxth) camerath = maxth;

	return 750.0/tan(camerath/2);

	
	// momemtum

	/*static double velocity = 0;

	if (mousedirection > 0){
		velocity += zoomin_multiplier*(maxz - currentzoom);
	}
	else if (mousedirection < 0){
		velocity -= zoomout_multiplier*(currentzoom - minz);
	}

	currentzoom += velocity;
	velocity -= velocity*friction;
	if (velocity*velocity < 0.01){
		velocity = 0;
	}
*/
	// return

}ouble calculatezoom(int mousedirection, double currentzoom);
class ZoomHandler{

	private:
		unsigned int window_width;
		unsigned int window_height;

		double max_zoom = 2;
		double min_zoom = 0.866;

		double min_fov = 1.047;
		double max_fov = 2.1;
		double momentum_friction = 0.3;
		double zoomin_multiplier = 0.25;
		double zoomout_multiplier = 0.25;


	protected:

	public:
		ZoomHandler(){}
		~ZoomHandler(){}

		calculateZoom(){
			//	double cameraz = 866.0/currentzoom;
			//simple
			//currentzoom += 25*mousedirection;
			//	cameraz -= 0.35*mousedirection;
			//	if (cameraz < minz) cameraz = minz;
			//	if (cameraz > maxz) cameraz = maxz;

			//	return 866.0/cameraz;
		}

}

int main(){
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
	
	const double mapw = 1.1;
	const double maph = 0.6;
	const double mapx = 0.1;
	const double mapy = 0.1;
	const double tilew = 0.05;
	const double tileh = 0.05;
	const int tilepw = 50;
	const int tileph = 50;

	double zoomfactor = 433;
	double mousex = 0;
	double mousey = 0;

	double camerax = mapx + mapw/2;
	double cameray = mapy + maph/2;
	bool mouse_cameracontrol = false;
	
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
		double oldzoom = zoomfactor;

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
				mouse_cameracontrol = true;
			}
			else if (event.type == SDL_MOUSEBUTTONUP){
				mouse_cameracontrol = false;
			}
			else if (event.type == SDL_MOUSEWHEEL){
				zoomdirection = event.wheel.y;
			}
			else if (event.type == SDL_MOUSEMOTION){
	
				if (mouse_cameracontrol){
					camerax = camerax - (event.motion.xrel)/(zoomfactor);
					cameray = cameray - (event.motion.yrel)/(zoomfactor);
				}
				else{
					mousex = (event.motion.x - WINDOW_WIDTH/2.0)/(zoomfactor) + camerax;
					mousey = (event.motion.y - WINDOW_HEIGHT/2.0)/(zoomfactor) + cameray;
				}
			}
			else if (event.type == SDL_QUIT){
				quitnow = true;
			}
	
		}

		SDL_RenderClear(renderer);


		zoomfactor = calculatezoom(zoomdirection, zoomfactor);
		
		if (zoomfactor != oldzoom){
			camerax = (camerax - mousex)*(oldzoom/zoomfactor) + mousex;
			cameray = (cameray - mousey)*(oldzoom/zoomfactor) + mousey;
		}

		
		for (double x = mapx; x < mapx+mapw; x += tilew){
			for (double y = mapy; y < mapy+maph; y += tileh){
				SDL_Rect dst;
				dst.x = WINDOW_WIDTH/2.0 + zoomfactor*(x-camerax);
				dst.y = WINDOW_HEIGHT/2.0 + zoomfactor*(y-cameray);
				dst.w = zoomfactor*tilew;
				dst.h = zoomfactor*tileh;
				SDL_RenderCopy(renderer, bgtile_texture, NULL, &dst);
			}
		} 
		SDL_RenderPresent(renderer);
	}
    SDL_DestroyTexture(character_texture);
	SDL_DestroyTexture(bgtile_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
	
