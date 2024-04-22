#include <SDL.h>
#include "stdio.h"
#include "LTexture.hpp"

// Creating a window and rendering for it
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

LTexture texture;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

// Initialization of all SDL subsystems and windows
bool init()
{
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL fails to initialize. SDL_Error: %s\n", SDL_GetError());
        success = false;
    }

    gWindow = SDL_CreateWindow("Pokemon Fight", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL)
    {
        printf("SDL failed to create window. SDL_Error: %s\n", SDL_GetError());
        success = false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == NULL)
    {
        printf("SDL failed to create a renderer. SDL_Error: %s\n", SDL_GetError());
        success = false;
    }

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    return success;
}

void close()
{
    SDL_Quit();

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
}

bool loadMedia()
{
    bool success = true;
    if (!texture.loadFromFile("/home/stas/SDL/Pokemon/assets/pikachu.png", gRenderer))
    {
        printf("Unable to load image!\n");
        success = false;
    }

    return success;
}

int main()
{
    if (!init())
    {
        printf("Failed to load SDL!\n");
    }
    if (!loadMedia())
    {
        printf("Failed to load media!\n");
    }
    bool quit = false;

    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        SDL_RenderClear(gRenderer);
        texture.render(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, gRenderer);
        SDL_RenderPresent(gRenderer);
    }

    close();
    return 0;
}
