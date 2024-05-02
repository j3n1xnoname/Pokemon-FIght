#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include "LTexture.hpp"
#include "LTimer.hpp"
#include "Pokemon.hpp"
#include "Player.hpp"

enum AttackIndexes
{
    FIRST_ATTACK,
    SECOND_ATTACK,
    THIRD_ATTACK,
    SKIP_ATTACK
};

enum TextIndexes
{
    ENTER_GAME,
    CHOOSE_PLAYER1_TEXT,
    CHOOSE_PLAYER2_TEXT,
    PLAYER_ONE,
    PLAYER_TWO,
    SKIP_MOVE,
    P1_GAME,
    P2_GAME
};

enum IconIndexes
{
    ATTACK,
    DEFEND,
    MANA_BLUE,
    MANA_RED,
    ARROW,
    SWORD,
    BOTTLE,
    HEART
};

enum PokemonNumber
{
    PIKACHU,
    CHARIZARD,
    BULBASAUR
};

enum MapIndexes
{
    FIRST_MAP,
    SECOND_MAP,
    THIRD_MAP,
    FOURTH_MAP
};
// Creating a window and rendering for it
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
TTF_Font *gFont64 = NULL;
TTF_Font *gFont48 = NULL;
TTF_Font *gFont32 = NULL;

TTF_Font *gFontGame = NULL;

const int POKEMONS_COUNT = 3;
const int TEXT_COUNT = 8;
const int ICON_COUNT = 8;
const int MAP_COUNT = 4;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;
Pokemon pokemons[POKEMONS_COUNT];

LTexture texts[TEXT_COUNT];
LTexture icons[ICON_COUNT];
LTexture maps[MAP_COUNT];

Player player1;
Player player2;

enum switchBetweenScreens
{
    START_GAME = 1,
    CHOOSE_PLAYER1,
    CHOOSE_PLAYER2,
    THE_BATTLE
};

void fillPngPokemon(Pokemon &pokemon, std::string name)
{
    if (name == "Pikachu")
    {
        pokemon.pokemonTexture.loadFromFile("/home/stas/SDL/Pokemon/assets/pikachu.png", gRenderer);
        pokemon.pokemonTextureReversed.loadFromFile("/home/stas/SDL/Pokemon/assets/pikachuReversed.png", gRenderer);
    }
    else if (name == "Charizard")
    {
        pokemon.pokemonTexture.loadFromFile("/home/stas/SDL/Pokemon/assets/charizard.png", gRenderer);
        pokemon.pokemonTextureReversed.loadFromFile("/home/stas/SDL/Pokemon/assets/charizardReversed.png", gRenderer);
    }
    else if (name == "Bulbasaur")
    {
        pokemon.pokemonTexture.loadFromFile("/home/stas/SDL/Pokemon/assets/bulbasaur.png", gRenderer);
        pokemon.pokemonTextureReversed.loadFromFile("/home/stas/SDL/Pokemon/assets/bulbasaurReversed.png", gRenderer);
    }
    SDL_Color tempColor = {0, 0, 0, SDL_ALPHA_OPAQUE};

    pokemon.healthbar.loadFromFile("/home/stas/SDL/Pokemon/assets/healthbar.png", gRenderer);
    pokemon.pokemonName.loadFromRenderedText(name, tempColor, gFont48, gRenderer);
}

void fillAttackPokemon(Attack &attack, std::string attackName, std::string attackDescription, int attackDamage, int attackWasteMana)
{
    SDL_Color color = {0, 0, 0, SDL_ALPHA_OPAQUE};
    attack.attackDamageText.loadFromRenderedText(std::to_string(attackDamage), color, gFontGame, gRenderer);
    attack.attackNameText.loadFromRenderedText(attackName, color, gFontGame, gRenderer);
    attack.wasteManaText.loadFromRenderedText(std::to_string(attackWasteMana), color, gFontGame, gRenderer);
    attack.attackDescriptionText.loadFromRenderedText(attackDescription, color, gFontGame, gRenderer);
}

void fillTextPokemon(Pokemon &pokemon, int hp, int mana, int attackDamage, int wasteMana)
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

    std::string s;

    for (int i = 0; i < POKEMONS_COUNT; i++)
    {
        std::string name, attackName, attackDescription;
        int hp, mana, attackDamage, attackWasteMana;
        std::getline(in, s);
        name = s;
        std::getline(in, s);
        hp = std::stoi(s);
        std::getline(in, s);
        mana = std::stoi(s);
        pokemons[i] = Pokemon{name, hp, mana};
        for (int j = 0; j < 3; j++)
        {
            std::getline(in, s);
            attackName = s;
            std::getline(in, s);
            attackDescription = s;
            std::getline(in, s);
            attackDamage = std::stoi(s);
            std::getline(in, s);
            attackWasteMana = std::stoi(s);
            pokemons[i].pokemonAttacks[j] = Attack{attackName, attackDescription, attackDamage, attackWasteMana};
            fillAttackPokemon(pokemons[i].pokemonAttacks[j], attackName, attackDescription, attackDamage, attackWasteMana);
        }
        fillTextPokemon(pokemons[i], hp, mana, attackDamage, attackWasteMana);
        fillPngPokemon(pokemons[i], name);

        pokemons[i].pokemonAttacks[3] = Attack{"Skip move", "Just skip a move", 0, 0};
        fillAttackPokemon(pokemons[i].pokemonAttacks[3], "Skip move", "Just skip a move", 0, 0);
        std::getline(in, s);
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
    if (!texts[P1_GAME].loadFromRenderedText("P1", tempColor, gFontGame, gRenderer))
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
    if (!texts[P2_GAME].loadFromRenderedText("P2", tempColor, gFontGame, gRenderer))
    {
        success = false;
    }

    tempColor = {0, 0, 0, 0};
    if (!texts[SKIP_MOVE].loadFromRenderedText("Skip move", tempColor, gFontGame, gRenderer))
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
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
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

    if (gFontGame != NULL)
        TTF_CloseFont(gFontGame);
    gFontGame = NULL;

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
    newText.render(SCREEN_WIDTH / 2 - newText.getWidth() / 2, SCREEN_HEIGHT / 2 - newText.getHeight() / 2, gRenderer);
    newText.flashing();
    if (currentKeyState[SDL_SCANCODE_RETURN])
    {
        currentState++;
    }
}

void choosePlayer(LTexture &choosePlayerText, LTexture &playerText, int &curState)
{
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

    gFontGame = TTF_OpenFont("/home/stas/SDL/Pokemon/assets/ComicoroRu_0.ttf", 64);

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
        else if (i == ARROW)
        {
            if (!icons[i].loadFromFile("/home/stas/SDL/Pokemon/assets/arrow.png", gRenderer))
                flag = true;
        }
        else if (i == HEART)
        {
            if (!icons[i].loadFromFile("/home/stas/SDL/Pokemon/assets/heart2.png", gRenderer))
                flag = true;
        }
        else if (i == SWORD)
        {
            if (!icons[i].loadFromFile("/home/stas/SDL/Pokemon/assets/sword2.png", gRenderer))
                flag = true;
        }
        else if (i == BOTTLE)
        {
            if (!icons[i].loadFromFile("/home/stas/SDL/Pokemon/assets/bottle.png", gRenderer))
                flag = true;
        }

        if (flag)
        {
            printf("IMG failed to create icon. IMG_Error: %s\n", IMG_GetError());
            success = false;
        }
    }

    for (int i = 0; i < MAP_COUNT; i++)
    {
        bool flag = false;
        if (i == FIRST_MAP)
        {
            if (!maps[i].loadFromFile("/home/stas/SDL/Pokemon/assets/backgrounds/game_background_1.png", gRenderer))
            {
                flag = true;
            }
        }
        else if (i == SECOND_MAP)
        {
            if (!maps[i].loadFromFile("/home/stas/SDL/Pokemon/assets/backgrounds/game_background_2.png", gRenderer))
            {
                flag = true;
            }
        }
        else if (i == THIRD_MAP)
        {
            if (!maps[i].loadFromFile("/home/stas/SDL/Pokemon/assets/backgrounds/game_background_3.png", gRenderer))
            {
                flag = true;
            }
        }
        else if (i == FOURTH_MAP)
        {
            if (!maps[i].loadFromFile("/home/stas/SDL/Pokemon/assets/backgrounds/game_background_4.png", gRenderer))
            {
                flag = true;
            }
        }

        if (flag)
        {
            printf("IMG failed to create map. IMG_Error: %s\n", IMG_GetError());
            success = false;
        }
    }

    fillPokemons();

    if (!fillTexts())
        success = false;

    return success;
}

void outputIconsInBattle(int curState, Player &player)
{
    int tempWidth = 50 + player.pokemon.pokemonAttacks[curState].attackNameText.getWidth() + icons[SWORD].getWidth();
    switch (curState)
    {
    case 0:
        icons[SWORD].render(50 + player.pokemon.pokemonAttacks[curState].attackNameText.getWidth(), SCREEN_HEIGHT / 2, gRenderer);
        icons[BOTTLE].render(tempWidth + player.pokemon.pokemonAttacks[curState].attackDamageText.getWidth(), SCREEN_HEIGHT / 2, gRenderer);
        player.pokemon.pokemonAttacks[curState].wasteManaText.render(tempWidth + player.pokemon.pokemonAttacks[curState].attackDamageText.getWidth() +
                                                                         icons[BOTTLE].getWidth(),
                                                                     SCREEN_HEIGHT / 2, gRenderer);
        player.pokemon.pokemonAttacks[curState].attackDamageText.render(tempWidth, SCREEN_HEIGHT / 2, gRenderer);
        break;
    case 1:
        icons[SWORD].render(50 + player.pokemon.pokemonAttacks[curState].attackNameText.getWidth(), SCREEN_HEIGHT / 2 + 50, gRenderer);
        icons[BOTTLE].render(tempWidth + player.pokemon.pokemonAttacks[curState].attackDamageText.getWidth(), SCREEN_HEIGHT / 2 + 50, gRenderer);
        player.pokemon.pokemonAttacks[curState].wasteManaText.render(tempWidth + player.pokemon.pokemonAttacks[curState].attackDamageText.getWidth() +
                                                                         icons[BOTTLE].getWidth(),
                                                                     SCREEN_HEIGHT / 2 + 50, gRenderer);
        player.pokemon.pokemonAttacks[curState].attackDamageText.render(tempWidth, SCREEN_HEIGHT / 2 + 50, gRenderer);
        break;
    case 2:
        icons[SWORD].render(50 + player.pokemon.pokemonAttacks[curState].attackNameText.getWidth(), SCREEN_HEIGHT / 2 + 100, gRenderer);
        icons[BOTTLE].render(tempWidth + player.pokemon.pokemonAttacks[curState].attackDamageText.getWidth(), SCREEN_HEIGHT / 2 + 100, gRenderer);
        player.pokemon.pokemonAttacks[curState].wasteManaText.render(tempWidth + player.pokemon.pokemonAttacks[curState].attackDamageText.getWidth() +
                                                                         icons[BOTTLE].getWidth(),
                                                                     SCREEN_HEIGHT / 2 + 100, gRenderer);
        player.pokemon.pokemonAttacks[curState].attackDamageText.render(tempWidth, SCREEN_HEIGHT / 2 + 100, gRenderer);

        break;
    case 3:
        icons[SWORD].render(50 + player.pokemon.pokemonAttacks[curState].attackNameText.getWidth(), SCREEN_HEIGHT / 2 + 150, gRenderer);
        icons[BOTTLE].render(tempWidth + player.pokemon.pokemonAttacks[curState].attackDamageText.getWidth(), SCREEN_HEIGHT / 2 + 150, gRenderer);
        player.pokemon.pokemonAttacks[curState].wasteManaText.render(tempWidth + player.pokemon.pokemonAttacks[curState].attackDamageText.getWidth() +
                                                                         icons[BOTTLE].getWidth(),
                                                                     SCREEN_HEIGHT / 2 + 150, gRenderer);
        player.pokemon.pokemonAttacks[curState].attackDamageText.render(tempWidth, SCREEN_HEIGHT / 2 + 150, gRenderer);
        break;
    }
}

void battleLogic(Player &player1, Player &player2, int curState, int &curGlobalState)
{
    player1.pokemon.setMana(player1.pokemon.getManaPoints() - player1.pokemon.pokemonAttacks[curState].getWastedMana());
}

void battle(Player &player1, Player &player2, Player &currentPlayer, int &curState, int &curGlobalState, int curPlayer)
{
    SDL_Rect tempRectangle1 = {player1.pokemon.pokemonTexture.getWidth() * 3 / 2 - 20,
                               player1.pokemon.pokemonTexture.getHeight() + player1.pokemon.healthbar.getHeight() + 10,
                               std::max((int)(1.0 * player1.pokemon.healthbar.getWidth() / player1.pokemon.getFullHp() * player1.pokemon.getHp()), 0), player1.pokemon.healthbar.getHeight()};

    SDL_Rect tempRectangle2 = {player2.pokemon.pokemonTexture.getWidth() + player2.pokemon.healthbar.getWidth() / 2 + 360,
                               player2.pokemon.pokemonTexture.getHeight() + player2.pokemon.healthbar.getHeight() + 10,
                               std::max((int)(1.0 * player1.pokemon.healthbar.getWidth() / player1.pokemon.getFullHp() * player1.pokemon.getHp()), 0), player2.pokemon.healthbar.getHeight()};

    player1.pokemon.pokemonTextureReversed.render(SCREEN_WIDTH / 2 - player1.pokemon.pokemonTexture.getWidth() - 50,
                                                  SCREEN_HEIGHT / 4 - player1.pokemon.pokemonTexture.getHeight() + 100, gRenderer);
    player2.pokemon.pokemonTexture.render(SCREEN_WIDTH / 2 + 50,
                                          SCREEN_HEIGHT / 4 - player2.pokemon.pokemonTexture.getHeight() + 100, gRenderer);

    player1.pokemon.healthbar.render(player1.pokemon.pokemonTexture.getWidth() * 3 / 2 - 20,
                                     player1.pokemon.pokemonTexture.getHeight() + player1.pokemon.healthbar.getHeight() + 10, gRenderer);

    icons[HEART].render(player1.pokemon.pokemonTexture.getWidth() * 3 / 2 - 20 - icons[HEART].getWidth(),
                        player1.pokemon.pokemonTexture.getHeight() + player1.pokemon.healthbar.getHeight() - icons[HEART].getWidth() / 4, gRenderer);

    player2.pokemon.healthbar.render(player2.pokemon.pokemonTexture.getWidth() + player2.pokemon.healthbar.getWidth() / 2 + 360,
                                     player2.pokemon.pokemonTexture.getHeight() + player2.pokemon.healthbar.getHeight() + 10, gRenderer);

    icons[HEART].render(player2.pokemon.pokemonTexture.getWidth() + player2.pokemon.healthbar.getWidth() / 2 + 360 - icons[HEART].getWidth(),
                        player2.pokemon.pokemonTexture.getHeight() + player2.pokemon.healthbar.getHeight() - icons[HEART].getWidth() / 4, gRenderer);

    texts[P1_GAME].render(10, 0, gRenderer);
    texts[P2_GAME].render(SCREEN_WIDTH - texts[P2_GAME].getWidth() - 10, 0, gRenderer);

    currentPlayer.pokemon.pokemonAttacks[FIRST_ATTACK].attackNameText.render(50, SCREEN_HEIGHT / 2, gRenderer);
    currentPlayer.pokemon.pokemonAttacks[SECOND_ATTACK].attackNameText.render(50, SCREEN_HEIGHT / 2 + 50, gRenderer);
    currentPlayer.pokemon.pokemonAttacks[THIRD_ATTACK].attackNameText.render(50, SCREEN_HEIGHT / 2 + 100, gRenderer);
    currentPlayer.pokemon.pokemonAttacks[SKIP_ATTACK].attackNameText.render(50, SCREEN_HEIGHT / 2 + 150, gRenderer);
    icons[ARROW].render(0, SCREEN_HEIGHT / 2 + curState * 50 + 10, gRenderer);

    outputIconsInBattle(curState, currentPlayer);

    currentPlayer.pokemon.pokemonAttacks[curState].attackDescriptionText.render(10, SCREEN_HEIGHT / 2 + 220, gRenderer);

    if (curPlayer == 1)
    {
        battleLogic(player1, player2, curGlobalState);
    }
    else
    {
        battleLogic(player2, player1, curGlobalState);
    }

    SDL_SetRenderDrawColor(gRenderer, 201, 15, 15, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(gRenderer, &tempRectangle1);
    SDL_RenderFillRect(gRenderer, &tempRectangle2);
}

void battleEvent(SDL_Event &e, int &currentPlayer, Player &curPlayer, Player &anotherPlayer, int &currentState, int &curGlobalState)
{
    if (e.type == SDL_KEYDOWN)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_DOWN:
            currentState++;
            break;
        case SDLK_UP:
            currentState--;
            break;
        case SDLK_RETURN:
            currentState = 0;
            if (currentPlayer == 1)
                currentPlayer = 2;
            else
                currentPlayer = 1;
            break;
        }

        if (currentState > SKIP_ATTACK)
            currentState = SKIP_ATTACK;
        if (currentState < FIRST_ATTACK)
            currentState = FIRST_ATTACK;
    }
}

int main()
{
    std::srand(std::time(0));
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

    int tempStateBattle1 = 0;
    int tempStateBattle2 = 0;

    int currentPlayer = 1;

    LTimer fpsTimer;

    LTimer capTimer;

    int countedFrames = 0;
    fpsTimer.start();
    const Uint8 *currentKeyState = SDL_GetKeyboardState(NULL);

    int framePlayer1 = 0;

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
            else if (currentState == THE_BATTLE)
            {
                if (player2.pokemon.pokemonTexture.isEmpty())
                {
                    player2.pokemon = pokemons[tempState2];
                }
                if (player1.pokemon.pokemonTexture.isEmpty())
                {
                    player1.pokemon = pokemons[tempState1];
                }

                if (currentPlayer == 1)
                {
                    battleEvent(e, currentPlayer, player1, player2, tempStateBattle1, currentState);
                }
                else
                {
                    battleEvent(e, currentPlayer, player2, player1, tempStateBattle2, currentState);
                }
            }
        }

        switch (currentState)
        {
        case START_GAME:
            SDL_SetRenderDrawColor(gRenderer, 0, 0, 128, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(gRenderer);
            startGame(currentState, currentKeyState, texts[ENTER_GAME]);
            break;
        case CHOOSE_PLAYER1:
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);
            choosePlayer(texts[CHOOSE_PLAYER1_TEXT], texts[PLAYER_ONE], tempState1);
            break;
        case CHOOSE_PLAYER2:
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);
            choosePlayer(texts[CHOOSE_PLAYER2_TEXT], texts[PLAYER_TWO], tempState2);
            break;
        case THE_BATTLE:
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);
            if (currentPlayer == 1)
            {
                battle(player1, player2, player1, tempStateBattle1, currentState, currentPlayer);
            }
            else
            {
                battle(player1, player2, player2, tempStateBattle2, currentState, currentPlayer);
            }
            break;
        }

        SDL_RenderPresent(gRenderer);

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
