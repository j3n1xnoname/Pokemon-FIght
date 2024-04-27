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

enum TextIndexes
{
    ENTER_GAME,
    CHOOSE_PLAYER1_TEXT,
    CHOOSE_PLAYER2_TEXT,
    PLAYER_ONE,
    PLAYER_TWO
};

enum IconIndexes
{
    ATTACK,
    DEFEND,
    MANA_BLUE,
    MANA_RED
};

enum PokemonNumber
{
    PIKACHU,
    CHARIZARD,
    BULBASAUR
};

// Creating a window and rendering for it
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
TTF_Font *gFont64 = NULL;
TTF_Font *gFont48 = NULL;
TTF_Font *gFont32 = NULL;

const int POKEMONS_COUNT = 3;
const int TEXT_COUNT = 5;
const int ICON_COUNT = 4;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;
Pokemon pokemons[POKEMONS_COUNT];
LTexture texts[TEXT_COUNT];
LTexture icons[ICON_COUNT];

enum switchBetweenScreens
{
    START_GAME = 1,
    CHOOSE_PLAYER1,
    CHOOSE_PLAYER2,
    THE_BATTLE
};

void fillPngPokemons(Pokemon &pokemon, std::string name)
{
    if (name == "Pikachu")
    {
        pokemon.pokemonTexture.loadFromFile("/home/stas/SDL/Pokemon/assets/pikachu.png", gRenderer);
    }
    else if (name == "Charizard")
    {
        pokemon.pokemonTexture.loadFromFile("/home/stas/SDL/Pokemon/assets/charizard.png", gRenderer);
    }
    else if (name == "Bulbasaur")
    {
        pokemon.pokemonTexture.loadFromFile("/home/stas/SDL/Pokemon/assets/bulbasaur.png", gRenderer);
    }
    SDL_Color tempColor = {0, 0, 0, SDL_ALPHA_OPAQUE};

    pokemon.pokemonName.loadFromRenderedText(name, tempColor, gFont48, gRenderer);
}
void fillTextPokemons(Pokemon &pokemon, int hp, int mana, int attackDamage, int wasteMana)
{
    SDL_Color tempColor = {255, 0, 0, SDL_ALPHA_OPAQUE};
    pokemon.attackDamageText.loadFromRenderedText(std::to_string(attackDamage), tempColor, gFont48, gRenderer);
    pokemon.wasteManaText.loadFromRenderedText(std::to_string(wasteMana), tempColor, gFont48, gRenderer);
    tempColor = {0, 255, 123, SDL_ALPHA_OPAQUE};
    pokemon.hpText.loadFromRenderedText(std::to_string(hp), tempColor, gFont48, gRenderer);
    tempColor = {40, 111, 254, SDL_ALPHA_OPAQUE};
    pokemon.manaPointsText.loadFromRenderedText(std::to_string(mana), tempColor, gFont48, gRenderer);
}

void fillPokemons()
{
    // Pokemon(std::string NAME = "", int HP = 0, int MANA = 0, int ATTACK_DAMAGE = 0, std::string ATTACK_NAME = "", int WASTE_MANA = 0, SDL_Renderer *gRenderer = NULL);
    std::ifstream in("/home/stas/SDL/Pokemon/assets/pokemons.txt");

    for (int i = 0; i < POKEMONS_COUNT; i++)
    {
        std::string name, attackName;
        int hp, mana, attackDamage, wasteMana;
        in >> name >> hp >> mana >> attackDamage >> attackName >> wasteMana;
        Pokemon tempPokemon = Pokemon{name, hp, mana, attackDamage, attackName, wasteMana, gRenderer};
        pokemons[i] = tempPokemon;
        fillPngPokemons(pokemons[i], name);
        fillTextPokemons(pokemons[i], hp, mana, attackDamage, wasteMana);
    }

    in.close();
}

bool fillTexts()
{
    bool success = true;
    SDL_Color tempColor = {232, 253, 96, SDL_ALPHA_OPAQUE};
    if (!texts[ENTER_GAME].loadFromRenderedText("Press enter to start...", tempColor, gFont64, gRenderer))
    {
        success = false;
    }
    tempColor = {64, 96, 255, SDL_ALPHA_OPAQUE};
    if (!texts[CHOOSE_PLAYER1_TEXT].loadFromRenderedText("Choose pokemon", tempColor, gFont48, gRenderer))
    {
        success = false;
    }
    if (!texts[PLAYER_ONE].loadFromRenderedText("Player 1", tempColor, gFont48, gRenderer))
    {
        success = false;
    }
    tempColor = {255, 21, 21, SDL_ALPHA_OPAQUE};
    if (!texts[CHOOSE_PLAYER2_TEXT].loadFromRenderedText("Choose pokemon", tempColor, gFont48, gRenderer))
    {
        success = false;
    }
    if (!texts[PLAYER_TWO].loadFromRenderedText("Player 2", tempColor, gFont48, gRenderer))
    {
        success = false;
    }
    if (!success)
    {
        printf("Failed to load array!\n");
    }

    return success;
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

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    return success;
}

void close()
{
    for (int i = 0; i < TEXT_COUNT; i++)
    {
        texts[i].free();
    }
    for (int i = 0; i < ICON_COUNT; i++)
    {
        icons[i].free();
    }
    if (gFont64 != NULL)
        TTF_CloseFont(gFont64);
    gFont64 = NULL;

    if (gFont48 != NULL)
        TTF_CloseFont(gFont48);
    gFont48 = NULL;
    if (gFont32 != NULL)
    {
        TTF_CloseFont(gFont32);
    }
    gFont32 = NULL;

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}

void startGame(int &currentState, const Uint8 *currentKeyState, LTexture &newText)
{
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 128, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(gRenderer);
    newText.render(SCREEN_WIDTH / 2 - newText.getWidth() / 2, SCREEN_HEIGHT / 2 - newText.getHeight() / 2, gRenderer);
    newText.flashing();
    if (currentKeyState[SDL_SCANCODE_RETURN])
    {
        currentState++;
    }
    SDL_RenderPresent(gRenderer);
}

void choosePlayer(int &currentState, LTexture &choosePlayerText, LTexture &playerText, int &curState)
{
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);
    choosePlayerText.render(SCREEN_WIDTH / 2 - choosePlayerText.getWidth() / 2, 0, gRenderer);
    playerText.render(0, 0, gRenderer);

    pokemons[curState].pokemonTexture.flashing();
    pokemons[curState].pokemonName.flashing();

    pokemons[curState].attackDamageText.render(SCREEN_WIDTH / 2 + icons[ATTACK].getWidth(), SCREEN_HEIGHT / 4 - icons[ATTACK].getHeight(), gRenderer);
    pokemons[curState].hpText.render(SCREEN_WIDTH / 2 + icons[DEFEND].getWidth(), SCREEN_HEIGHT * 2 / 4 - icons[DEFEND].getHeight(), gRenderer);
    pokemons[curState].manaPointsText.render(SCREEN_WIDTH / 2 + icons[MANA_BLUE].getWidth(), SCREEN_HEIGHT * 3 / 4 - icons[MANA_BLUE].getHeight(), gRenderer);
    pokemons[curState].wasteManaText.render(SCREEN_WIDTH / 2 + icons[MANA_RED].getWidth(), SCREEN_HEIGHT - icons[MANA_RED].getHeight(), gRenderer);

    pokemons[0].pokemonTexture.render(0, 0, gRenderer);
    pokemons[0].pokemonName.render(300, 180, gRenderer);
    pokemons[1].pokemonTexture.render(25, 250, gRenderer);
    pokemons[1].pokemonName.render(300, 400, gRenderer);
    pokemons[2].pokemonTexture.render(0, 400, gRenderer);
    pokemons[2].pokemonName.render(300, 590, gRenderer);

    icons[ATTACK].render(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4 - icons[ATTACK].getHeight(), gRenderer);
    icons[DEFEND].render(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 2 / 4 - icons[DEFEND].getHeight(), gRenderer);
    icons[MANA_BLUE].render(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 3 / 4 - icons[MANA_BLUE].getHeight(), gRenderer);
    icons[MANA_RED].render(SCREEN_WIDTH / 2, SCREEN_HEIGHT - icons[MANA_RED].getHeight(), gRenderer);

    SDL_RenderPresent(gRenderer);
}

void choosePlayerEvents(int &curEventState, SDL_Event &e, int &curState)
{
    if (e.type == SDL_KEYDOWN)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_UP:
            if (curEventState != 0)
            {
                pokemons[curEventState].pokemonTexture.setNewAlpha(255);
                pokemons[curEventState].pokemonName.setNewAlpha(255);
            }
            curEventState--;
            break;
        case SDLK_DOWN:
            if (curEventState != 2)
            {
                pokemons[curEventState].pokemonTexture.setNewAlpha(255);
                pokemons[curEventState].pokemonName.setNewAlpha(255);
            }
            curEventState++;
            break;
        case SDLK_RETURN:
            for (int i = 0; i < POKEMONS_COUNT; i++)
            {
                pokemons[i].pokemonTexture.setNewAlpha(255);
                pokemons[i].pokemonName.setNewAlpha(255);
            }
            curState++;
            break;
        }
    }

    if (curEventState < 0)
        curEventState = 0;
    if (curEventState > 2)
        curEventState = 2;
}

bool loadMedia()
{
    bool success = true;

    gFont64 = TTF_OpenFont("/home/stas/SDL/Pokemon/assets/ChunkFive-Regular.otf", 64);
    if (gFont64 == NULL)
    {
        printf("TTF failed to create font. TTF_Error: %s\n", TTF_GetError());
        success = false;
    }

    gFont48 = TTF_OpenFont("/home/stas/SDL/Pokemon/assets/ChunkFive-Regular.otf", 48);
    if (gFont48 == NULL)
    {
        printf("TTF failed to create font. TTF_Error: %s\n", TTF_GetError());
        success = false;
    }

    gFont32 = TTF_OpenFont("/home/stas/SDL/Pokemon/assets/ChunkFive-Regular.otf", 32);
    if (gFont32 == NULL)
    {
        printf("TTF failed to create font. TTF_Error: %s\n", TTF_GetError());
        success = false;
    }

    for (int i = 0; i < ICON_COUNT; i++)
    {
        bool flag = false;
        if (i == ATTACK)
        {
            if (!icons[i].loadFromFile("/home/stas/SDL/Pokemon/assets/attack.png", gRenderer))
            {
                flag = true;
            }
        }
        else if (i == DEFEND)
        {
            if (!icons[i].loadFromFile("/home/stas/SDL/Pokemon/assets/defend.png", gRenderer))
                flag = true;
        }
        else if (i == MANA_BLUE)
        {
            if (!icons[i].loadFromFile("/home/stas/SDL/Pokemon/assets/manaBlue.png", gRenderer))
                flag = true;
        }
        else if (i == MANA_RED)
        {
            if (!icons[i].loadFromFile("/home/stas/SDL/Pokemon/assets/manaRed.png", gRenderer))
                flag = true;
        }

        if (flag)
        {
            printf("IMG failed to create icon. IMG_Error: %s\n", IMG_GetError());
            success = false;
        }
    }

    fillPokemons();

    if (!fillTexts())
        success = false;

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
    int tempState1 = 0;
    int tempState2 = 0;

    LTimer fpsTimer;

    LTimer capTimer;

    int countedFrames = 0;
    fpsTimer.start();
    const Uint8 *currentKeyState = SDL_GetKeyboardState(NULL);

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (currentState == CHOOSE_PLAYER1)
            {
                choosePlayerEvents(tempState1, e, currentState);
            }
            else if (currentState == CHOOSE_PLAYER2)
            {
                choosePlayerEvents(tempState2, e, currentState);
            }
        }

        switch (currentState)
        {
        case START_GAME:
            startGame(currentState, currentKeyState, texts[ENTER_GAME]);
            break;
        case CHOOSE_PLAYER1:
            choosePlayer(currentState, texts[CHOOSE_PLAYER1_TEXT], texts[PLAYER_ONE], tempState1);
            break;
        case CHOOSE_PLAYER2:
            choosePlayer(currentState, texts[CHOOSE_PLAYER2_TEXT], texts[PLAYER_TWO], tempState2);
            break;
        case THE_BATTLE:
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
