#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int SCREEN_FPS = 60; //desired frames per second
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS; //milliseconds per frame, rounds low

const int SPRITE_SIZE = 63; //square sprite size  (ie 63 = 64x64 pixel sprite)
const int SPRITE_FRAMES = 2; //number of sprites in animation
const int SPRITE_SWITCH_TIME = 1000; //time in ms between switching frames
const int SPRITE_WAIT = SPRITE_SWITCH_TIME / SCREEN_TICKS_PER_FRAME; //frames to wait for animation switch

const std::string SPRITE_PATH = "animation.png"; //where to find animation

class LTexture
{
    public:
        //Initializes variables
        LTexture();

        //Deallocated memory
        ~LTexture();

        //Loads image at a specified path
        bool loadFromFile( std::string path );

        //Deallocates texture
        void free();

        //Set color modulation
        void setColor( Uint8 red, Uint8 green, Uint8 blue );

        //Set blending
        void setBlendMode( SDL_BlendMode blending );

        //Set alpha modulation
        void setAlpha( Uint8 alpha );

        //Renders terxture at a given point
        void render( int x, int y, SDL_Rect* clip = NULL );

        //Gets image dimensions
        int getWidth();
        int getHeight();

    private:
        //The actual hardware texture
        SDL_Texture* mTexture;

        //Image dimensions
        int mWidth;
        int mHeight;
};

//Starts up SDL and creates window
bool init();

//loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we are rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Animation
SDL_Rect gSpriteClips[ SPRITE_FRAMES ];
LTexture gSpriteSheetTexture;

LTexture::LTexture()
{
    //Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
    //Deallocate
    free();
}

bool LTexture::loadFromFile( std::string path )
{
    //Get rid of preexisting texture
    free();

    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );

    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n",
                path.c_str(), IMG_GetError() );
    }
    else
    {
        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE,
                SDL_MapRGB( loadedSurface -> format, 0, 0xFF, 0xFF ) );

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n",
                    path.c_str(), SDL_GetError() );
        }
        else
        {
            //get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        //get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
        //Return success
        mTexture = newTexture;
        return mTexture != NULL;
    }

void LTexture::free()
{
    //Free texture if it exists
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    //modulate texture rgb
    SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
    //Set blending function
    SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
    //Modulate texture alpha
    SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip )
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    //Set clip rendering dimensions
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //render to screen
    SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad );
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}

bool init()
{
    //Init flag
    bool success = true;

    //initialize sdl
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }

        //Create window
        gWindow = SDL_CreateWindow( "Animation Test",
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //create vsynced renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1,
                    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );


                //Initiaize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_Image Error: %s\n",
                            IMG_GetError() );
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia()
{
    //loading success flag
    bool success = true;

    //Load sprite texture sheet
    if( !gSpriteSheetTexture.loadFromFile( SPRITE_PATH ) )
    {
        printf( "Failed to load texture %s\n", SPRITE_PATH.c_str() );
        success = false;
    }
    else
    {
       for( int i=0; i < SPRITE_FRAMES; i++ )
       {
           //size is in pixels, (0,0) is in top left corner of image
           gSpriteClips[ i ].x = 0 + SPRITE_SIZE*i;
           gSpriteClips[ i ].y = 0;
           gSpriteClips[ i ].w = SPRITE_SIZE;
           gSpriteClips[ i ].h = SPRITE_SIZE;
       }
    }
       return success;
}

void close()
{
    //Free loaded images
    gSpriteSheetTexture.free();

    //Destroy Window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int main( int argc, char* args[] )
{
    //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;

            //current animation frame
            int frame = 0;

            //last recorded frame when running at desired framerate
            Uint32 fps_lastframe = SDL_GetTicks();

            //while application is running
            while( !quit )
            {
                //handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //if window is closed out, quit program
                    if( e.type == SDL_QUIT )
                    {
                     quit = true;
                    }

                    //general keypress event case structure
                    if( e.type == SDL_KEYDOWN )
                    {
                        switch( e.key.keysym.sym)
                        {
                            //if escape key is hit, quit program
                            case SDLK_ESCAPE:
                                quit = true;
                                break;
                            default:
                                break;
                        }
                    }
                }

                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );

                //time to wait between frame switches. Since frame is an int,
                //any math that results in a decimal will be rounded down, so:
                // 0/1 = 0, 1/1 = 1, 1/3 = 1,
                // SWITCH_TIME / TICKS_PER_FRAME determines total frames to wait
                SDL_Rect* currentClip = &gSpriteClips[ frame / SPRITE_WAIT ];

                //Renders sprite in center of screen
                gSpriteSheetTexture.render( ( SCREEN_WIDTH - currentClip->w ) / 2,
                        ( SCREEN_HEIGHT - currentClip->h ) /2, currentClip );

                //Update screen
                SDL_RenderPresent( gRenderer );

                //wait remaining time if running too fast
                if(( SDL_GetTicks() - fps_lastframe ) < SCREEN_TICKS_PER_FRAME )
                {
                    SDL_Delay( SCREEN_TICKS_PER_FRAME - ( SDL_GetTicks() - fps_lastframe ));
                }

                fps_lastframe = SDL_GetTicks();

                //Go to next frame
                ++frame;

                //Cycle animation
                if( frame /  SPRITE_WAIT  >= SPRITE_FRAMES )
                {
                    frame = 0;
                }
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}
