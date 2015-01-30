#include <iostream>
#include <cmath>
#include <vector>
#include "SDL.h"

class TempSettings {
	public:

		unsigned int window_width;
		unsigned int window_height;

		double mapw;
		double maph;
		double mapx;
		double mapy;

		std::vector<ControlBaseClass*> affectedsettings;

		void addToList(ControlBaseClass* newcontrol){
			affectedsettings.push_back(newcontrol);
		}

		void when_a_setting_is_changed(){
			// change the setting
			// eg. mapw = 1400

			for (unsigned int i = 0; i < affectedsettings.size(); ++i)
				affectedsettings[i].update_settings();
		}

		TempSettings(){
			window_width = 1300;
			window_height = 800;
		}

		~TempSettings(){}

};

class ControlBaseClass {
	
	public:
		ControlBaseClass(){};
		virtual ~ControlBaseClass(){};

		virtual void update_settings();
}


class CameraControl : public ControlBaseClass {

	private:
		double fieldofviewx; // radians
		double fieldofviewy; // radians
		double tanfovx; //convenient
		double tanfovy;

		std::vector<double> max_pixelratio;
		std::vector<double> pixelratio;
		std::vector<double> planeZs;
	
	protected:

		double camx;
		double camy;
		double camz;

		double max_x;
		double min_x;
		double max_y;
		double min_y;
		double max_z;
		double min_z;

		bool mouse_control;

		// player settings
		double zoomin_speed;
		double zoomout_speed;
		double zoom_friction;
		bool momentum_on;	

		TempSettings *game_settings;

	public:

		enum ZPlane {
			Floor = 0,
			Player = 1,
			Gui = 2
		};

		ZoomControl(TempSettings *gamesettings);
		~ZoomControl();

		void adjust_zoom(int input);
		void mousecontrol_on(){ mouse_control = true;}
		void mousecontrol_off(){ mouse_control = false;}
		void mousecontrol_move(int relative_x, int relative_y);

		SDL_Rect calculate_display_destination ( double x,
												 double y,
												 double w,
												 double h,
												 ZPlane zplane);


		double xfrompixel(int pixelX, ZPlane z);
		double yfrompixel(int pixelY, ZPlane z);
		double wfrompixel(int pixelW, ZPlane z);
		double hfrompixel(int pixelH, ZPlane z);
		int pixelfromx(double x, ZPlane z);
		int pixelfromy(double y, ZPlane z);
		int pixelfromw(double w, ZPlane z);
		int pixelfromh(double h, ZPlane z);
};
