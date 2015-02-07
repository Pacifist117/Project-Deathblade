#ifndef DEVELOPER_CONSOLE_H
#define DEVELOPER_CONSOLE_H

#include "graphicscontrol/tempsettings.h"
#include "graphicscontrol/controlbaseclass.h"

#include "SDL.h"
#include "SDL_ttf.h"

#include <iostream>
#include <vector>
#include <string>

class DeveloperConsoleClass : public ControlBaseClass {

public:
    DeveloperConsoleClass(TempSettings *gamesettings);
    ~DeveloperConsoleClass();

    void set_historymaxlength(unsigned int newlength);
    bool execute_command(std::string command_and_arguments);

    void drawon(SDL_Renderer* renderer);
    void render_current_command(SDL_Renderer *renderer);
    void render_history(SDL_Renderer *renderer);

    // basic private member manipulation
    void addinput(std::string input);
    void backspace();
    void enter();
    void goback_inhistory();
    void goforward_inhistory();
    bool mouse_grab(bool mousedown, int mousex, int mousey);
    void handle_mouse(int mousex, int mousey, int relmousex, int relmousey);
    bool scroll(int input, int mousepx, int mousepy);
    void setfont(std::string filename, unsigned int font_size);
    void add_controller(ControlBaseClass* newclass);
    void bringup()  { active = true; }
    void close()    { active = false; }
    void toggle()   { active = !active; }
    bool is_active(){ return active; }

    // virtual functions
    virtual void update_settings();
    virtual std::string parse_arguments(std::vector<std::string> args);
	
private:
    std::vector<std::string> history;
    std::vector<std::string> rethistory;
    unsigned int history_maxlength;
    unsigned int history_place;
    void addtohistory(std::string prompt, std::string result);



    // Settings
    TTF_Font *font;
    unsigned int fontsize;
    SDL_Color font_color;
    SDL_Color background_color;
    SDL_Rect position;


    // other internal
    TempSettings* game_settings;
    std::string current_command;
    SDL_Texture *history_texture;
    SDL_Texture *input_texture;
    bool active;
    bool move_grabbed;
    bool resize_left,resize_right,resize_top,resize_bottom;
    int scroll_offset;

    std::vector<ControlBaseClass*> controllers;

};

#endif
