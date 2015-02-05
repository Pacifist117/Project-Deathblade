#include "graphicscontrol/cameracontrol.h"

CameraControl::CameraControl(TempSettings *gamesettings){

	if (gamesettings == NULL){
		std::cerr << "A null pointer was passed into camera control\n";
		return;
	}

	planeZs.push_back(0);
    planeZs.push_back(0.2);
	planeZs.push_back(1);

    zoomin_speed = 0.05;
    zoomout_speed = 0.075;
	zoom_friction = 0.3;
    momentum_on = true;
    x_sidebuffer = 0.25;
    y_sidebuffer = 0.25;

    mouse_control = false;

	fieldofviewx = 75*3.14156/180.0;
	tanfovx = tan(fieldofviewx/2.0);

	max_pixelratio.push_back( 1000);
    max_pixelratio.push_back( max_pixelratio[db::Floor] + planeZs[db::Player]*2*tanfovx);
	max_pixelratio.push_back( 1 );

	game_settings = gamesettings;
	gamesettings->addToList(this);
	update_settings();

    pixelratio.push_back(max_pixelratio[db::Floor]);
    pixelratio.push_back(max_pixelratio[db::Player]);
    pixelratio.push_back(max_pixelratio[db::Gui]);

}

CameraControl::~CameraControl(){
}

void CameraControl::update_settings(){

    tanfovx = tan(fieldofviewx/2.0);
	fieldofviewy = 2*atan(game_settings->window_height*tanfovx/game_settings->window_width);
	tanfovy = tan(fieldofviewy/2.0);

	camx = game_settings->mapx + game_settings->mapw/2.0;
	camy = game_settings->mapy + game_settings->maph/2.0;

    max_x = game_settings->mapx + game_settings->mapw + x_sidebuffer;
    min_x = game_settings->mapx - x_sidebuffer;
    max_y = game_settings->mapy + game_settings->maph + y_sidebuffer;
    min_y = game_settings->mapy - y_sidebuffer;

    min_z = game_settings->window_width/(2.0*max_pixelratio[db::Player]*tanfovx);
    max_z = game_settings->mapw/(2.0*tanfovx) + planeZs[db::Player];
	camz = max_z;
	
}

void CameraControl::adjust_zoom(int input, double mouse_x, double mouse_y){

    static double dx = 0;
    static double dy = 0;
    static double dz = 0;

    double oldpr = pixelratio[db::Player];

    if (input < 0){
        dz += zoomout_speed*(max_z - camz - input);
    }
    else if (input > 0){
        dz += zoomin_speed*(min_z - camz - input);
    }
    camz += dz;


    if (momentum_on){
        dz -= dz*zoom_friction;
        if (dz*dz < 0.01) dz = 0;
    }
    else {
        dz = 0;
    }

    if (camz > max_z) camz = max_z;
	if (camz < min_z) camz = min_z;

    pixelratio[db::Floor] = game_settings->window_width/(2.0*camz - planeZs[db::Floor]*tanfovx);
    pixelratio[db::Player] = game_settings->window_width/(2.0*camz - planeZs[db::Player]*tanfovx);

    if(input >= 0 && pixelratio[db::Player] != oldpr){
        camx = mouse_x - (oldpr/pixelratio[db::Player])*(mouse_x - camx);
        camy = mouse_y - (oldpr/pixelratio[db::Player])*(mouse_y - camy);
    }

    checkcamxy();

	return;
}

void CameraControl::mousecontrol_move(int relative_x, int relative_y){
    camx = camx - wfrompixel(relative_x, db::Player);
    camy = camy - hfrompixel(relative_y, db::Player);
    checkcamxy();
}

SDL_Rect CameraControl::calculate_display_destination(  double x,
                                                        double y,
                                                        double w,
                                                        double h,
                                                        db::ZPlane zplane){

    SDL_Rect dst;
    dst.x = pixelfromx(x,zplane);
    dst.y = pixelfromy(y,zplane);
    dst.w = pixelfromw(w,zplane);
    dst.h = pixelfromh(h,zplane);
    return dst;
}

SDL_Rect CameraControl::calculate_display_destination( ObjectBaseClass* object){

    SDL_Rect dst;
    dst.x = pixelfromx(object->x,object->zplane);
    dst.y = pixelfromy(object->y,object->zplane);
    dst.w = pixelfromw(object->w,object->zplane);
    dst.h = pixelfromh(object->h,object->zplane);
    return dst;
}

double CameraControl::xfrompixel(int pixelX, db::ZPlane z){
	return (pixelX - game_settings->window_width/2.0)/pixelratio[z] + camx;
}

double CameraControl::yfrompixel(int pixelY, db::ZPlane z){
	return (pixelY - game_settings->window_height/2.0)/pixelratio[z] + camy;
}

double CameraControl::wfrompixel(int pixelW, db::ZPlane z){
	return pixelW/pixelratio[z];
}

double CameraControl::hfrompixel(int pixelH, db::ZPlane z){
	return pixelH/pixelratio[z];
}

int CameraControl::pixelfromx(double x, db::ZPlane z){
	return game_settings->window_width/2.0 + pixelratio[z]*(x - camx);
}

int CameraControl::pixelfromy(double y, db::ZPlane z){
	return game_settings->window_height/2.0 + pixelratio[z]*(y- camy);
}

int CameraControl::pixelfromw(double w, db::ZPlane z){
	return w*pixelratio[z];
}

int CameraControl::pixelfromh(double h, db::ZPlane z){
	return h*pixelratio[z];
}

void CameraControl::checkcamxy(){
    if (camx + (camz+planeZs[db::Player])*tanfovx > max_x)
        camx = 0.5*(camx + max_x - (camz+planeZs[db::Player])*tanfovx);
    else if (camx - (camz+planeZs[db::Player])*tanfovx < min_x)
        camx = 0.5*(camx + min_x + (camz+planeZs[db::Player])*tanfovx);
    if (camy + (camz+planeZs[db::Player])*tanfovy > max_y)
        camy = 0.5*(camy + max_y - (camz+planeZs[db::Player])*tanfovy);
    else if (camy - (camz+planeZs[db::Player])*tanfovy < min_y)
        camy = 0.5*(camy + min_y + (camz+planeZs[db::Player])*tanfovy);
}

bool CameraControl::parse_arguments(std::vector<std::string> args){

    if (args[0].compare("camera") != 0) return false;

    if (args[1].compare("help") == 0){
        *outstream << "Possilble camera commands are:\n";
        *outstream << "   camera help\n";
        *outstream << "   camera camx <double>\n";
        *outstream << "   camera camy <double>\n";
        *outstream << "   camera camz <double>\n";
        *outstream << "   camera zoomin_speed <double>\n";
        *outstream << "   camera zoomout_speed <double>\n";
        *outstream << "   camera zoom_friction <double>\n";
        *outstream << "   camera momentum_on <true/false>\n";
        *outstream << "   camera x_sidebuffer <double>\n";
        *outstream << "   camera y_sidebuffer <double>\n";
        *outstream << "   camera fieldofvew <double>\n";
        *outstream << "   camera planeZs <Floor/Player> <double>\n";
        *outstream << "   camera <command> help\n\n";
    }
    else if (args[1].compare("camx") == 0){
        if (args[2].compare("help") == 0){
            *outstream << "   camera camx <double>\n";
            *outstream << "      Sets the new camera x position. It is bounded by map width and field of view.\n\n";
        }
        else{
            camx = atof(args[2].c_str());
            checkcamxy();
            *outstream << "camx set to " << camx << std::endl;
        }
    }
    else if (args[1].compare("camy") == 0){
        if (args[2].compare("help") == 0){
            *outstream << "   camera camy <double>\n";
            *outstream << "      Sets the new camera y position. It is bounded by map height and field of view.\n\n";
        }
        else{
            camy = atof(args[2].c_str());
            checkcamxy();
            *outstream << "camy set to " << camy << std::endl;
        }
    }
    else if (args[1].compare("camz") == 0){
        if (args[2].compare("help") == 0){
            *outstream << "   camera camz <double>\n";
            *outstream << "      Sets the new camera z position. It is bounded by map size and field of view (a pyramid structure above the field.\n\n";
        }
        else{
            camz = atof(args[2].c_str());
            *outstream << "camz set to " << camz << std::endl;
        }
    }
    else if (args[1].compare("zoomin_speed") == 0){
        if (args[2].compare("help") == 0){
            *outstream << "   camera zoomin_speed <double>\n";
            *outstream << "      Sets the multiplier for the speed of zooming in. Default is 0.05.\n";
        }
        else{
            zoomin_speed = atof(args[2].c_str());
            *outstream << "zoomin_speed set to " << zoomin_speed << std::endl;
        }
    }
    else if (args[1].compare("zoomout_speed") == 0){
        if (args[2].compare("help") == 0){
            *outstream << "   camera zoomout_speed <double>\n";
            *outstream << "      Sets the multiplier for the speed of zooming out. Default is 0.075.\n";
        }
        else{
            zoomout_speed = atof(args[2].c_str());
            *outstream << "zoomout_speed set to " << zoomout_speed << std::endl;
        }
    }
    else if (args[1].compare("zoom_friction") == 0){
        if (args[2].compare("help") == 0){
            *outstream << "   camera zoom_friction <double>\n";
            *outstream << "      Sets the multiplier for the friction in momentum zooming. Default is 0.3.\n";
        }
        else{
            zoom_friction = atof(args[2].c_str());
            *outstream << "zoom_friction set to " << zoom_friction << std::endl;
        }
    }
    else if (args[1].compare("momentum_on") == 0){
        if (args[2].compare("help") == 0){
            *outstream << "   camera momentum_on <true/false\n";
            *outstream << "      Turns momentum effect of zooming on/off.\n";
        }
        else{
            if (args[2].compare("true") == 0){
                momentum_on = true;
                *outstream << "momentum_on set to true\n";
            }
            else if (args[2].compare("false") == 0){
                momentum_on = false;
                *outstream << "momentum_on set to false\n";
            }
        }
    }
    else if (args[1].compare("x_sidebuffer") == 0){
        if (args[2].compare("help") == 0){
            *outstream << "   camera x_sidebuffer <double>\n";
            *outstream << "      Sets the amount of in-game space the camera can see to the left/right of the map. Default is 0.25.\n";
        }
        else{
            x_sidebuffer = atof(args[2].c_str());
            *outstream << "x_sidebuffer set to " << x_sidebuffer << std::endl;
        }
    }
    else if (args[1].compare("y_sidebuffer") == 0){
        if (args[2].compare("help") == 0){
            *outstream << "   camera y_sidebuffer <double>\n";
            *outstream << "      Sets the amount of in-game space the camera can see above/below the map. Default is 0.25.\n";
        }
        else{
            y_sidebuffer = atof(args[2].c_str());
            *outstream << "y_sidebuffer set to " << y_sidebuffer << std::endl;
        }
    }
    else if (args[1].compare("fieldofview") == 0){
        if (args[2].compare("help") == 0){
            *outstream << "   camera fieldofvew <double>\n";
            *outstream << "      Sets the horizontal field of view of the camera (in degrees). Default is 75 degrees.\n";
        }
        else{
            fieldofviewx = atof(args[2].c_str());
            *outstream << "fieldofview set to " << fieldofviewx << std::endl;
            fieldofviewx = fieldofviewx*3.14156/180.0;
            update_settings();
        }
    }
    else if (args[1].compare("planeZs") == 0){
        if (args[2].compare("help") == 0){
            *outstream << "   camera planeZs <Floor/Player> <double>\n";
            *outstream << "      Sets the z value for the entire plane of Foor/Player. This is used in calculations for zoom. Defaults are 0/0.2.\n";
        }
        else{
            if (args.size() < 4) return true;
            if (args[2].compare("Floor") == 0){
                planeZs[db::Floor] = atof(args[2].c_str());
                update_settings();
                *outstream << "planeZs[Floor] set to " << planeZs[db::Floor] << std::endl;
            }
            else if (args[2].compare("Player") == 0){
                planeZs[db::Player] = atof(args[2].c_str());
                update_settings();
                *outstream << "planeZs[Player] set to " << planeZs[db::Player] << std::endl;
            }
        }
    }

    return true;
}

