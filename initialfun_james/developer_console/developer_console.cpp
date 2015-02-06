#include "developer_console.h"
#include <sstream>
#include <iterator>

DeveloperConsoleClass::DeveloperConsoleClass(TempSettings *gamesettings){

    if (gamesettings == NULL){
        std::cerr << "A null pointer was passed into camera control\n";
        return;
    }
    game_settings = gamesettings;

    name = "console";
    history_maxlength = 100;



    // Settings
    fontsize = 18;
    setfont("sample.ttf",fontsize);
    font_color.r = 255;
    font_color.g = 255;
    font_color.b = 255;
    font_color.a = 255;
    background_color.r = 100;
    background_color.g = 100;
    background_color.b = 100;
    background_color.a = 50;

    position.x = 0;
    position.y = 0;
    position.w = game_settings->window_width/2;
    position.h = game_settings->window_height/2;
    textfield.w = position.w-4;
    textfield.h = fontsize+3;
    textfield.x = position.x + 2;
    textfield.y = position.y + position.h - textfield.h - 2;

    // other internal
    current_command = "";
    history_texture = NULL;
    input_texture = NULL;
    active = false;
}

DeveloperConsoleClass::~DeveloperConsoleClass(){
    SDL_DestroyTexture(history_texture);
    SDL_DestroyTexture(input_texture);
}

void DeveloperConsoleClass::update_settings(){
    position.w = game_settings->window_width;
    position.h = game_settings->window_height;
    textfield.w = position.w;
    textfield.x = position.x + 2;
    textfield.y = position.y + position.h - textfield.h;

}

void DeveloperConsoleClass::addtohistory(std::string prompt){
	if (history.size() == history_maxlength){
		history.pop_back();
	}
	history.insert(history.begin(),prompt);
}

void DeveloperConsoleClass::set_historymaxlength(unsigned int newlength){
	for (unsigned int i = newlength; i < history.size(); ++i)
		history.pop_back();
	history_maxlength = newlength;
}


bool DeveloperConsoleClass::execute_command(std::string command_and_arguments){

	addtohistory(command_and_arguments);
    std::vector<std::string> arguments;
    std::stringstream ss(command_and_arguments);
    std::string item;
    while (std::getline(ss,item,' '))
        arguments.push_back(item);

    for(unsigned int i = 0; i < controllers.size(); ++i){
        if (controllers[i]->parse_arguments(arguments))
            return true;
    }

    *outstream << "Invalid controller name in command.\n";
    *outstream << "Type '<controller> help' for more information.\n";
    *outstream << "Current controllers include: \n";
    for (unsigned int i = 0; i < controllers.size(); ++i)
        *outstream << "   " << controllers[i]->name << std::endl;
    return false;
}

void DeveloperConsoleClass::drawon(SDL_Renderer *renderer)
{

    // draw the box
    SDL_SetRenderDrawColor(renderer,background_color.r,background_color.g,background_color.b,background_color.a);
    SDL_RenderFillRect(renderer, &position);
    SDL_SetRenderDrawColor(renderer,240,240,240,240);
    SDL_RenderDrawRect(renderer,&textfield);

    // write the text
    if(input_texture != NULL){
        SDL_Rect inputrect;
        SDL_QueryTexture(input_texture, NULL, NULL, &inputrect.w, &inputrect.h);
        inputrect.x = textfield.x+2;
        inputrect.y = textfield.y+4;
        SDL_RenderCopy(renderer, input_texture, NULL, &inputrect);
    }

}

void DeveloperConsoleClass::render_text(SDL_Renderer* renderer){

    if (font == NULL) return;

    if(input_texture != NULL){
        SDL_DestroyTexture(input_texture);
    }

    if(current_command.length() == 0){
        input_texture = NULL;
        return;
    }

    SDL_Surface *input_surface = TTF_RenderText_Blended(font, current_command.c_str(), font_color);
    if (input_surface == NULL){
        std::cerr << "TTF_RenderText error: " << SDL_GetError() << std::endl;
        return;
    }
    input_texture = SDL_CreateTextureFromSurface(renderer, input_surface);
    if (input_texture == NULL){
        std::cerr << "SDL_CreateTextureFromSurface error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(input_surface);
        return;
    }
    SDL_FreeSurface(input_surface);
}

void DeveloperConsoleClass::addinput(std::string input)
{
    current_command += input;
}

void DeveloperConsoleClass::backspace(){
    if (current_command.length() > 0)
        current_command = current_command.substr(0,current_command.length()-1);
}

void DeveloperConsoleClass::enter(){
    execute_command(current_command);
    current_command = "";
    addinput("");
}

void DeveloperConsoleClass::setfont(std::string filename, unsigned int font_size)
{
#ifdef _WIN32
    const char PATH_SEP = '\\';
#else
    const char PATH_SEP = '/';
#endif
    static std::string baseRes;
    if (baseRes.empty()){
        char *basePath = SDL_GetBasePath();
        if (basePath){
            baseRes = basePath;
            SDL_free(basePath);
        }
        else {
            std::cerr << "Error getting resource path: " << SDL_GetError() << std::endl;
            return;
        }
        //We replace the last bin/ with res/ to get the the resource path
        size_t pos = baseRes.rfind("bin");
        baseRes = baseRes.substr(0, pos) + "res" + PATH_SEP;
    }

    fontsize = font_size;
    textfield.h = fontsize+3;
    textfield.y = position.y + position.h - textfield.h - 2;

    std::string full_filename = baseRes + filename;
    font = TTF_OpenFont(full_filename.c_str(), fontsize);
    if (font == NULL){
        std::cerr << "TTF_OpenFont error: " << SDL_GetError() << std::endl;
    }
}

void DeveloperConsoleClass::add_controller(ControlBaseClass *newclass)
{
    for (int i = 0; i < controllers.size(); ++i){
        if (newclass == controllers[i])
            return;
    }
    controllers.push_back(newclass);
}

bool DeveloperConsoleClass::parse_arguments(std::vector<std::string> args){
    if (args[0].compare(name) != 0) return false;

    if (args[1].compare("help") == 0){
        *outstream << "Possilble console setting commands are:\n";
        *outstream << "   console help\n";
        *outstream << "   console <command> help\n\n";
    }
    else if (args[1].compare("font") == 0){
        if (args[2].compare("help") == 0){
            *outstream << "   console font <filename>\n";
            *outstream << "      Sets the new font.\n\n";
        }
        else{
            // update font
            *outstream << "font set to " << args[2] << std::endl;
        }
    }



//    // Settings
//    TTF_Font *font;
//    unsigned int fontsize;
//    SDL_Color font_color;
//    SDL_Color background_color;
//    SDL_Rect position;
//    SDL_Rect textfield;

    return true;

}
