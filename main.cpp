/*
current state: comments pls, structuring pls, documentation pls, ui pls.


*/

//Using SDL, SDL_image, standard IO, math, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>

#include <iostream>
#include <vector>

#include "Cell.h"
#include "smartTable.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;


SDL_Texture* gTexture = NULL;


const int roundTime = 100;

enum gameState
{
    planning,
    gameplay,
    pause
};

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
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
        gWindow = SDL_CreateWindow( "Conway's game of life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
            }
        }
    }

    return success;
}


//textures aren't a thing yet here
bool loadMedia()
{

    //Loading success flag
    bool success = true;

//    //Load PNG texture
//    gTexture = loadTexture( "texture.png" );
//    if( gTexture == NULL )
//    {
//        printf( "Failed to load texture image!\n" );
//        success = false;
//    }

    return success;

}

void close()
{
    //Free loaded image
    SDL_DestroyTexture( gTexture );
    gTexture = NULL;

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}




int main( int argc, char* args[] )
{
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            bool quit = false;
            SDL_Event e;
            gameState myGameState=planning;

            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);

            SDL_Color backgroundColor = {255, 255, 255, 255};
            SDL_Color lineColor = {255, 255, 255, 255};

            int horribleHack=1;

            int activeCellx=4;
            int activeCelly=4;

            Cell sampleCell(lineColor, backgroundColor, 10,10);

            smartTable mySmartTable (verticalTableSize, horizontalTableSize, &sampleCell);

            mySmartTable.highlightCell(activeCellx,activeCelly);
//            mySmartTable.highlightCell(activeCellx,activeCelly);

            while( !quit )
            {
                if (myGameState==gameplay)
                {
                    mySmartTable.deHighLight();
                    mySmartTable.executeRound();
                    SDL_Delay(roundTime);

                }

                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {

                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                    else if( e.type == SDL_KEYDOWN )
                    {
                        switch( e.key.keysym.sym )
                        {
                        case SDLK_UP:
                            switch (myGameState)
                            {
                            case gameplay:

                                break;
                            case planning:
                                mySmartTable.moveHighlightedCell(up);
                                break;
                            case pause:

                                break;

                            }
                            break;

                        case SDLK_DOWN:
                            switch (myGameState)
                            {
                            case gameplay:

                                break;
                            case planning:
                                mySmartTable.moveHighlightedCell(down);

                                break;
                            case pause:

                                break;
                            }
                            break;
                        case SDLK_LEFT:
                            switch (myGameState)
                            {
                            case gameplay:

                                break;
                            case planning:
                                mySmartTable.moveHighlightedCell(left);

                                break;
                            case pause:

                                break;
                            }
                            break;
                        case SDLK_RIGHT:
                            switch (myGameState)
                            {
                            case gameplay:

                                break;
                            case planning:
                                mySmartTable.moveHighlightedCell(right);

                                break;
                            case pause:

                                break;
                            }
                            break;
                        case SDLK_RETURN:
                            switch (myGameState)
                            {
                            case gameplay:

                                break;
                            case planning:
                                if (mySmartTable.getCellColor(mySmartTable.getHighlightedCell()) == defaultLiveColor)
                                    mySmartTable.colorCell(defaultLineColor, defaultBackgroundColor, mySmartTable.getHighlightedCell().y, mySmartTable.getHighlightedCell().x);
                                else mySmartTable.colorCell(defaultLineColor, defaultLiveColor, mySmartTable.getHighlightedCell().y, mySmartTable.getHighlightedCell().x);
                                break;
                            case pause:

                                break;
                            }
                            break;
                        case SDLK_SPACE:
                            switch (myGameState)
                            {
                            case gameplay:
                                myGameState=pause;
                                break;
                            case planning:
                                myGameState=gameplay;
                                break;
                            case pause:
                                myGameState=gameplay;
                                break;
                            }
                            break;
                        }
                    }


                }

                mySmartTable.draw(gRenderer);
                SDL_RenderPresent( gRenderer );
            }
            close();
        }
    }

}
