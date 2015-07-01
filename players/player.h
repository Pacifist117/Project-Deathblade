#ifndef PLAYER_H
#define PLAYER_H

#include "gameobjects/objectbase.h"
#include "ui_objects/boundclass.h"

namespace deathblade{


class Player : public ObjectBaseClass {

public:
    Player();
    ~Player();

    BoundClass bound; //!< Object used for controlled bouncing

    /*!
     * \brief Early start of what to do when player does a controlled bounce.
     */
    void rebound(ObjectBaseClass* );

    /*!
     * \brief Sets the texture for the player, and width/height in game coordinates.
     * \param player_texture Texture to be used for character.
     * \param width in game coordinates
     * \param height in game coordiantes
     */
    void setTexture(SDL_Texture* player_texture, double width, double height);

    // inherited functions
    void step_time();
    bool isMobile(){return true;}
    bool isPlayer(){return true;}
    void drawon(SDL_Renderer* renderer, CameraControl* camera);

private:

    SDL_Texture* character_texture = nullptr; //!< texture used to draw character. Set with setTexture.
    double texturewidth = 0; //!< Width of character in game coordinates.
    double textureheight = 0; //!< Height of character in game coordinates.
};



} // deathblade namespace
#endif
