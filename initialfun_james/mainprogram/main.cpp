
#include "graphicscontrol/cameracontrol.h"
#include "gameobjects/objectcontroller.h"
#include "developer_console/developer_console.h"
#include "gameobjects/basicwall.h"
#include "gameobjects/star.h"
#include "player/player.h"
#include "res_path.h"

#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL.h"

#include <iostream>
#include <cmath>

const int SCREEN_FPS = 30;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

int main(){


    TempSettings gamesettings;

    gamesettings.mapw = 10;
    gamesettings.maph = 6;
    gamesettings.mapx = 0;
    gamesettings.mapy = 0;
    gamesettings.mapmidx = gamesettings.mapw/2.0;
    gamesettings.mapmidy = gamesettings.maph/2.0;
    gamesettings.window_width = 1300;
    gamesettings.window_height = 800;

    // initialize window, renderer, textures
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() != 0){
        std::cerr << "TTF_Init error: " << SDL_GetError() << std::endl;
        SDL_Quit();
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
    std::string starfile = resource_path + "star.png";
    SDL_Texture* character_texture = IMG_LoadTexture(renderer, charfile.c_str());
    SDL_Texture* bgtile_texture = IMG_LoadTexture(renderer, bgfile.c_str());
    SDL_Texture* star_texture = IMG_LoadTexture(renderer, starfile.c_str());
    if (character_texture == nullptr || bgtile_texture == nullptr || star_texture == nullptr){
            std::cerr << "IMG_LoadTexture error: " << SDL_GetError() << std::endl;
            SDL_DestroyTexture(character_texture);
            SDL_DestroyTexture(bgtile_texture);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
    }


	CameraControl camera(&gamesettings);
    ObjectController objects;
    DeveloperConsoleClass console(&gamesettings);
    console.add_controller(&console);
    console.add_controller(&camera);

    const double tilew = 0.5;
    const double tileh = 0.5;
    double mousex = gamesettings.mapmidx;
    double mousey = gamesettings.mapmidy;
    int mousepx = gamesettings.window_width/2;
    int mousepy = gamesettings.window_height/2;

    BasicWall bottomwall, topwall, leftwall, rightwall;
    bottomwall.create(0,gamesettings.maph,gamesettings.mapw, 0.1);
    bottomwall.set_color(120,120,120);
    objects.add_object(&bottomwall);

    topwall.create(0,0,gamesettings.mapw, -0.1);
    topwall.set_color(120,120,120);
    objects.add_object(&topwall);

    leftwall.create(0,0,-0.1,gamesettings.maph);
    leftwall.set_color(120,120,120);
    objects.add_object(&leftwall);

    rightwall.create(gamesettings.mapw,0,0.1,gamesettings.maph);
    rightwall.set_color(120,120,120);
    objects.add_object(&rightwall);

    Player human;
    human.x = 5; human.y = 5;
    human.dx = -0.025; human.dy = -0.03;
    human.setTexture(character_texture, 0.05, 0.05);
    objects.add_object(&human);

    // map x [0, 10]
    // map y [0,  6]
    // star width 0.256
    std::vector<vec2d> star_positions = {
        vec2d(6,4),
        vec2d(3,4.1),
        vec2d(9,0.2),
        vec2d(1,0.4),
        vec2d(2,2.5),
        vec2d(3,2.5),
        vec2d(9,4.9),
        vec2d(0.2,5.1),
        vec2d(4.1,4.1)
    };
    std::vector<double> star_thetas = {
        0,
        45,
        15,
        60,
        85,
        4,
        50,
        66,
        31
    };

    std::vector<Star*> star_field;
    for(int i = 0; i < star_positions.size(); ++i){
        Star* newstar = new Star();

        newstar->x = star_positions[i].x;
        newstar->y = star_positions[i].y;
        newstar->setTexture(star_texture, 0.256, 0.256);

        if(i < star_thetas.size())
            newstar->rotate(star_thetas[i]*3.14159265359/180.0);


        star_field.push_back(newstar);
        objects.add_object(star_field[i]);

    }

    SDL_Event event;	
	bool quitnow = false;
    Uint32 fps_lastframe = SDL_GetTicks();
    while(!quitnow){
		
        int zoomdirection = 0;

		while(SDL_PollEvent(&event)){

            if (console.is_active()){
                if (event.type == SDL_KEYDOWN){
                    switch(event.key.keysym.sym){
                    case SDLK_BACKQUOTE:
                        console.toggle();
                        break;
                    case SDLK_BACKSPACE:
                        console.backspace();
                        break;
                    case SDLK_RETURN:
                        console.enter();
                        break;
                    case SDLK_UP:
                        console.goback_inhistory();
                        break;
                    case SDLK_DOWN:
                        console.goforward_inhistory();
                        break;
                    default:
                        break;
                    }
                    console.render_current_command(renderer);

                }
                else if (event.type == SDL_TEXTINPUT && event.text.text[0] != '`'){
                    console.addinput(event.text.text);
                    console.render_current_command(renderer);
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN){
                    if (event.button.button == SDL_BUTTON_LEFT){
                        if(!console.mouse_grab(true, event.button.x, event.button.y))
                            camera.mousecontrol_on();
                    }
                    //if (event.button.button == SDL_BUTTON_RIGHT)

                }
                else if (event.type == SDL_MOUSEBUTTONUP){
                    if (event.button.button == SDL_BUTTON_LEFT){
                        console.mouse_grab(false, -1,-1);
                        camera.mousecontrol_off();
                    }
                }
                else if (event.type == SDL_MOUSEMOTION){
                    mousepx = event.motion.x;
                    mousepy = event.motion.y;
                    console.handle_mouse(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);

                    if (camera.mouse_controlling()){
                        camera.mousecontrol_move(event.motion.xrel, event.motion.yrel);
                    }
                    else{
                        mousex = camera.xfrompixel(event.motion.x,db::Player);
                        mousey = camera.yfrompixel(event.motion.y,db::Player);
                    }
                }
                else if (event.type == SDL_MOUSEWHEEL){
                    if(!console.scroll(event.wheel.y, mousepx, mousepy))
                        zoomdirection += event.wheel.y;
                }
                else if (event.type == SDL_QUIT){
                    quitnow = true;
                }

                continue;
            }

            // if console is not up
			if (event.type == SDL_KEYDOWN){
				switch(event.key.keysym.sym){
                case SDLK_ESCAPE:
						quitnow = true;
						break;
                case SDLK_BACKQUOTE:
                    console.toggle();
                    break;
                case SDLK_t:
                    if (!camera.is_tracking())
                        camera.track_object(&(human.x), &(human.y));
                    else
                        camera.stop_tracking();
                    break;
                case SDLK_w:
                case SDLK_UP:
                    camera.pan_updown(-1);
                    break;
                case SDLK_a:
                case SDLK_LEFT:
                    camera.pan_leftright(-1);
                    break;
                case SDLK_s:
                case SDLK_DOWN:
                    camera.pan_updown(1);
                    break;
                case SDLK_d:
                case SDLK_RIGHT:
                    camera.pan_leftright(1);
                    break;
                default:
                    break;
				}
			}
			else if (event.type == SDL_KEYUP){
                switch(event.key.keysym.sym){
                case SDLK_w:
                case SDLK_UP:
                    camera.pan_updown(0);
                    break;
                case SDLK_a:
                case SDLK_LEFT:
                    camera.pan_leftright(0);
                    break;
                case SDLK_s:
                case SDLK_DOWN:
                    camera.pan_updown(0);
                    break;
                case SDLK_d:
                case SDLK_RIGHT:
                    camera.pan_leftright(0);
                    break;
                default:
                    break;
                }
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN){
                if (event.button.button == SDL_BUTTON_LEFT){
                    camera.stop_tracking();
                    camera.mousecontrol_on();
                }
			}
			else if (event.type == SDL_MOUSEBUTTONUP){
                if (event.button.button == SDL_BUTTON_LEFT){
                    camera.mousecontrol_off();
                }
			}
			else if (event.type == SDL_MOUSEWHEEL){
                zoomdirection += event.wheel.y;
			}
			else if (event.type == SDL_MOUSEMOTION){
                mousepx = event.motion.x;
                mousepy = event.motion.y;

				if (camera.mouse_controlling()){
					camera.mousecontrol_move(event.motion.xrel, event.motion.yrel);
				}
				else{
                    mousex = camera.xfrompixel(event.motion.x,db::Player);
                    mousey = camera.yfrompixel(event.motion.y,db::Player);
                    if(mousepx <= 1) camera.pan_leftright(-1);
                    else if (mousepx >= (int)gamesettings.window_width-1) camera.pan_leftright(1);
                    else if (mousepx - event.motion.xrel <= 1) camera.pan_leftright(0);
                    else if (mousepx - event.motion.xrel >= (int)gamesettings.window_width-1) camera.pan_leftright(0);
                    if(mousepy <= 1) camera.pan_updown(-1);
                    else if (mousepy >= (int)gamesettings.window_height-1) camera.pan_updown(1);
                    else if (mousepy - event.motion.yrel <= 1) camera.pan_updown(0);
                    else if (mousepy - event.motion.yrel >= (int)gamesettings.window_height-1) camera.pan_updown(0);
				}
            }
			else if (event.type == SDL_QUIT){
				quitnow = true;
            }
	
		}


        objects.step_time();

        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
		SDL_RenderClear(renderer);

		camera.adjust_zoom(zoomdirection, mousex, mousey);
		
		for (double x = gamesettings.mapx; x < gamesettings.mapx+gamesettings.mapw; x += tilew){
			for (double y = gamesettings.mapy; y < gamesettings.mapy+gamesettings.maph; y += tileh){
                SDL_Rect dst = camera.calculate_display_destination(x,y,tilew,tileh,db::Floor);
				SDL_RenderCopy(renderer, bgtile_texture, NULL, &dst);
			}
        }

        objects.drawon(renderer, &camera);
        human.drawon(renderer,&camera);
        if(console.is_active()) console.drawon(renderer);

		SDL_RenderPresent(renderer);

        Uint32 fps_newframe = SDL_GetTicks();
        if((fps_newframe-fps_lastframe) < SCREEN_TICKS_PER_FRAME){
            SDL_Delay(SCREEN_TICKS_PER_FRAME - (fps_newframe-fps_lastframe));
        }
        fps_lastframe = fps_newframe;
	}
    SDL_DestroyTexture(character_texture);
	SDL_DestroyTexture(bgtile_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    for(unsigned int i = 0; i < star_field.size(); ++i)
        delete star_field[i];

    return 0;
}
