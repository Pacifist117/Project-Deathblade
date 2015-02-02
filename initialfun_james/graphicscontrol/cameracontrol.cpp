#include "cameracontrol.h"

CameraControl::CameraControl(TempSettings *gamesettings){

	if (gamesettings == NULL){
		std::cerr << "A null pointer was passed into camera control\n";
		return;
	}

	planeZs.push_back(0);
	planeZs.push_back(0.2);
	planeZs.push_back(1);

	zoomin_speed = 0.3;
	zoomout_speed = 0.3;
	zoom_friction = 0.3;

	mouse_control = false;
	momentum_on = true;

	fieldofviewx = 75*3.14156/180.0;
	tanfovx = tan(fieldofviewx/2.0);

	max_pixelratio.push_back( 1000);
	max_pixelratio.push_back( max_pixelratio[Floor] + planeZs[Player]*2*tanfovx);
	max_pixelratio.push_back( 1 );

	game_settings = gamesettings;
	gamesettings->addToList(this);
	update_settings();

	pixelratio.push_back(max_pixelratio[Floor]);
	pixelratio.push_back(max_pixelratio[Player]);
	pixelratio.push_back(max_pixelratio[Gui]);

}

CameraControl::~CameraControl(){
}

void CameraControl::update_settings(){

	fieldofviewy = 2*atan(game_settings->window_height*tanfovx/game_settings->window_width);
	tanfovy = tan(fieldofviewy/2.0);

	camx = game_settings->mapx + game_settings->mapw/2.0;
	camy = game_settings->mapy + game_settings->maph/2.0;
	max_x = game_settings->mapx + game_settings->mapw;
	min_x = game_settings->mapx;
	max_y = game_settings->mapy + game_settings->maph;
	min_y = game_settings->mapy;

	min_z = game_settings->window_width/(2.0*max_pixelratio[Player]*tanfovx);
	max_z = game_settings->mapw/(2.0*tanfovx) + planeZs[Player];
	camz = max_z;

	
}

void CameraControl::adjust_zoom(int input, double mouse_x, double mouse_y){
	
	static double velocity = 0;
	double oldpr = pixelratio[Player];
	
	if (input < 0){
		velocity += zoomin_speed*(max_z - camz);
	}
	else if (input > 0){
		velocity += zoomout_speed*(min_z - camz);
	}

	camz += velocity;
	velocity -= velocity*zoom_friction;
	if (velocity*velocity < 0.01){
		velocity = 0;
	}

	if (camz > max_z) camz = max_z;
	if (camz < min_z) camz = min_z;

	pixelratio[Floor] = game_settings->window_width/(2.0*camz - planeZs[Floor]*tanfovx);
	pixelratio[Player] = game_settings->window_width/(2.0*camz - planeZs[Player]*tanfovx);

	if (pixelratio[Player] != oldpr){
		camx = mouse_x - (oldpr/pixelratio[Player])*(mouse_x - camx);
		camy = mouse_y - (oldpr/pixelratio[Player])*(mouse_y - camy);
	}

	return;
}

void CameraControl::mousecontrol_move(int relative_x, int relative_y){
	camx = camx - wfrompixel(relative_x, Player);
	camy = camy - hfrompixel(relative_y, Player);
	if (camx > max_x) camx = max_x;
	else if (camx < min_x) camx = min_x;
	if (camy > max_y) camy = max_y;
	else if (camy < min_y) camy = min_y;
}

SDL_Rect CameraControl::calculate_display_destination(  double x, 
														double y, 
														double w, 
														double h, 
														ZPlane zplane){

	SDL_Rect dst;
	dst.x = pixelfromx(x,zplane);
	dst.y = pixelfromy(y,zplane);
	dst.w = pixelfromw(w,zplane);
	dst.h = pixelfromh(h,zplane);
	return dst;
}

double CameraControl::xfrompixel(int pixelX, ZPlane z){
	return (pixelX - game_settings->window_width/2.0)/pixelratio[z] + camx;
}

double CameraControl::yfrompixel(int pixelY, ZPlane z){
	return (pixelY - game_settings->window_height/2.0)/pixelratio[z] + camy;
}

double CameraControl::wfrompixel(int pixelW, ZPlane z){
	return pixelW/pixelratio[z];
}

double CameraControl::hfrompixel(int pixelH, ZPlane z){
	return pixelH/pixelratio[z];
}

int CameraControl::pixelfromx(double x, ZPlane z){
	return game_settings->window_width/2.0 + pixelratio[z]*(x - camx);
}

int CameraControl::pixelfromy(double y, ZPlane z){
	return game_settings->window_height/2.0 + pixelratio[z]*(y- camy);
}

int CameraControl::pixelfromw(double w, ZPlane z){
	return w*pixelratio[z];
}

int CameraControl::pixelfromh(double h, ZPlane z){
	return h*pixelratio[z];
}

