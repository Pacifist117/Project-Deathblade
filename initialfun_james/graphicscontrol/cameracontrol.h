#include <iostream>
#include <cmath>
#include <vector>
#include "SDL.h"

class TempSettings;

/*!
 * \brief The ControlBaseClass class is the template for what every controller class will have.
 *
 * As of Feb 02, 2015 my idea is to have controller classes that control the main processes of the program.
 * For example, the CameraControlClass keeps track of the camera and controls the view of the user. Each of
 * the control classes will inherit ControlBaseClass so it will have access to any settings changed that affect
 * it.
 */
class ControlBaseClass {

protected:

    /*!
     * \brief Pointer to the object dealing with basic game settings such as resolution.
     */
    TempSettings *game_settings;

public:
    ControlBaseClass(){}
    virtual ~ControlBaseClass(){}

    /*!
     * \brief Recalculates all inner members that are dependent on other settings. This function
     * should be called for every controller class when a setting is changed.
     */
    virtual void update_settings(){}
};



/*!
 * \brief The TempSettings class is just a placeholder for the main controller class? or just the
 * game settings class.
 */
class TempSettings {
    public:

        unsigned int window_width;
        unsigned int window_height;

        double mapw;
        double maph;
        double mapx;
        double mapy;
        double mapmidx;
        double mapmidy;

        /*!
         * \brief A list of other controllers that are affected by a change within this.
         */
        std::vector<ControlBaseClass*> affectedsettings;

        void addToList(ControlBaseClass* newcontrol){
            affectedsettings.push_back(newcontrol);
        }

        void when_a_setting_is_changed(){
            // change the setting
            // eg. mapw = 1400

            for (unsigned int i = 0; i < affectedsettings.size(); ++i)
                affectedsettings[i]->update_settings();
        }

        TempSettings(){
        }

        ~TempSettings(){}

};

/*!
 * \brief The CameraControl class moves the camera according to mouse and mouse wheel inputs.
 *
 * This class functions on the concept that the camera is an object in the game's 3D space. The
 * x,y dimension are the plane of the game with (0,0) being the top left corner, and the z dimension
 * is 'above' the game.
 */
class CameraControl : public ControlBaseClass {

public:

    /*!
     * \brief Constructor for class
     * \param gamesettings The pointer to the main game controller. (sort of a parent object)
     */
    CameraControl(TempSettings *gamesettings);
    ~CameraControl();

    /*!
     * \brief The ZPlane enum labels which depth plane an object is on
     */
    enum ZPlane {
        Floor = 0,  //!< Background plane
        Player = 1, //!< Battle plane
        Gui = 2     //!< Fake plane for the GUI (could do something fun?)
    };

    /*!
     * \brief Based on input (usually from the mouse wheel), changes the z-value of the camera.
     *
     * \param input Negative values zoom out and positive values zoom in.
     * \param mouse_x Used to zoom in where mouse is pointing. In pixels.
     * \param mouse_y Used to zoom in where mouse is pointing. In pixels.
     */
    void adjust_zoom(int input, double mouse_x, double mouse_y);
    /*!
     * \brief Moves the camera based on mouse movement if mouse_control==true
     * \param relative_x X movement of the mouse in pixels
     * \param relative_y Y movement of the mouse in pixels
     */
    void mousecontrol_move(int relative_x, int relative_y);

    /*!
     * \brief Given a position in space, calculates the pixel destination.
     * \param x X position in space
     * \param y Y position in space
     * \param w Width of object in space
     * \param h Height of object in space
     * \param zplane Which plane the object is on.
     * \return Destination rectangle for rendering.
     */
    SDL_Rect calculate_display_destination ( double x,
                                             double y,
                                             double w,
                                             double h,
                                             ZPlane zplane);

    // These functions calculate space coordinates from pixel dimensions
    double xfrompixel(int pixelX, ZPlane z);
    double yfrompixel(int pixelY, ZPlane z);
    double wfrompixel(int pixelW, ZPlane z);
    double hfrompixel(int pixelH, ZPlane z);

    // These functions calculate pixel coordinates from space dimensions
    int pixelfromx(double x, ZPlane z);
    int pixelfromy(double y, ZPlane z);
    int pixelfromw(double w, ZPlane z);
    int pixelfromh(double h, ZPlane z);

    // inherited functions
    void update_settings();

    // Basic private member manipulation
    void mousecontrol_on(){ mouse_control = true;}
    void mousecontrol_off(){ mouse_control = false;}
    bool mouse_controlling(){ return mouse_control;}

protected:

    double camx; //!< X world coordinate of camera
    double camy; //!< Y world coordinate of camera
    double camz; //!< Z world coordinate of camera

    double max_x; //!< Max X world coordinate of camera
    double min_x; //!< Min X world coordinate of camera
    double max_y; //!< Max Y world coordinate of camera
    double min_y; //!< Min Y world coordinate of camera
    double max_z; //!< Max Z world coordinate of camera
    double min_z; //!< Min Z world coordinate of camera

    bool mouse_control; //!< Toggle for when mouse is in control

    // player prefernce settings
    double zoomin_speed;    //!< Sensitivity of mousewheel up: zoom in
    double zoomout_speed;   //!< Sensitivity of mousewheel down: zoom out
    double zoom_friction;   //!< How quickly the camera loses momentum
    bool momentum_on;       //!< Turn momentum off

    double fieldofviewx; //!< Field of view in the x direction (in radians)
    double fieldofviewy; //!< Field of view in the y direction (in radians)

    // Other variables used internally.
    std::vector<double> planeZs;        //!< The depths associated with each ZPlane
    std::vector<double> pixelratio;     //!< Pixels per world unit, for each ZPlane
    std::vector<double> max_pixelratio; //!< Maximum pixel ratios for each ZPlane

    double tanfovx; //convenient
    double tanfovy;

};
