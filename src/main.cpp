#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <fstream>
#include <vector>
#include <stdio.h>
#include <iostream>
#include "LTexture.hpp"
#include "LTimer.hpp"
#include "Pokemon.hpp"

// Creating a window and rendering for it
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
TTF_Font *gFont = NULL;

LTexture newText;

const int POKEMONS_COUNT = 3;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;
Pokemon pokemons[POKEMONS_COUNT];

enum switchBetweenScreens
{
    START_GAME = 1,
    CHOOSE_PLAYER1,
    CHOOSE_PLAYER2,
    THE_BATTLE
};

void fillPokemons()
{

    // Pokemon(std::string NAME = "", int HP = 0, int MANA = 0, int ATTACK_DAMAGE = 0, std::string ATTACK_NAME = "", int WASTE_MANA = 0, SDL_Renderer *gRenderer = NULL);
    std::ifstream in("/home/stas/SDL/Pokemon/assets/pokemons.txt");

    for (int i = 0; i < POKEMONS_COUNT; i++)
    {
        std::string name, attackName;
        int hp, mana, attackDamage, wasteMana;
        Pokemon tempPokemon = Pokemon{name, hp, mana, attackDamage, attackName, wasteMana, gRenderer};
        pokemons[i] = tempPokemon;
    }
    in.close();
}

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

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == NULL)
    {
        printf("SDL failed to create a renderer. SDL_Error: %s\n", SDL_GetError());
        success = false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("SDL_image fails to initialize. IMG_Error: %s\n", IMG_GetError());
        success = false;
    }

    if (TTF_Init() < 0)
    {
        printf("SDL_ttf fails to initialize. TTF_Error: %s\n", TTF_GetError());
        success = false;
    }

    gFont = TTF_OpenFont("/home/stas/SDL/Pokemon/assets/ChunkFive-Regular.otf", 64);
    if (gFont == NULL)
    {
        printf("TTF failed to create font. TTF_Error: %s\n", TTF_GetError());
        success = false;
    }
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    fillPokemons();
    return success;
}

void close()
{
    newText.free();
    if (gFont != NULL)
        TTF_CloseFont(gFont);
    gFont = NULL;

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}

void startGame(Uint8 &alpha, int &currentState, const Uint8 *currentKeyState)
{
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 128, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(gRenderer);
    newText.render(SCREEN_WIDTH / 2 - newText.getWidth() / 2, SCREEN_HEIGHT / 2 - newText.getHeight() / 2, gRenderer);
    newText.flashing(alpha);
    if (currentKeyState[SDL_SCANCODE_RETURN])
    {
        currentState++;
    }
    SDL_RenderPresent(gRenderer);
}

void choosePlayer1(Uint8 &alpha, int &currentState, const Uint8 *currentKeyState)
{
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);
    SDL_RenderPresent(gRenderer);
}

bool loadMedia()
{
    bool success = true;

    SDL_Color tempColor = {232, 253, 96, SDL_ALPHA_OPAQUE};

    if (!newText.loadFromRenderedText("Press enter to start...", tempColor, gFont, gRenderer))
    {
        printf("Unable to print text!\n");
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

    int currentState = START_GAME;

    Uint8 alpha = 255;

    LTimer fpsTimer;

    LTimer capTimer;

    int countedFrames = 0;

    fpsTimer.start();
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        const Uint8 *currentKeyState = SDL_GetKeyboardState(NULL);
        switch (currentState)
        {
        case START_GAME:
            startGame(alpha, currentState, currentKeyState);
            break;
        case CHOOSE_PLAYER1:
            choosePlayer1(alpha, currentState, currentKeyState);
            break;
        }

        float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
        if (avgFPS > 2000000)
        {
            avgFPS = 0;
        }
        ++countedFrames;
        int frameTicks = capTimer.getTicks();
        if (frameTicks < SCREEN_TICK_PER_FRAME)
        {
            SDL_Delay(SCREEN_TICK_PER_FRAME - frameTicks);
        }
    }

    close();
    return 0;
}
