#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

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
    P2_GAME,
    P1_ATTACKS,
    P2_ATTACKS,
    HP,
    MANA,
    PLAYER_ONE_WIN,
    PLAYER_TWO_WIN,

    POKEMON_FIGHT,
    GAME_WAS_MADE,
    AUTHOR_NICKNAME,

    ENTER_TO_CONTINUE

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
    HEART,
    MANA_BOTTLE,
    CROWN
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

TTF_Font *gFontEnd1 = NULL;
TTF_Font *gFontEnd2 = NULL;

TTF_Font *gFontGame = NULL;

Mix_Chunk *enter = NULL;
Mix_Chunk *pressArrow = NULL;
Mix_Chunk *pressEnterMenu = NULL;
Mix_Chunk *hit = NULL;

Mix_Music *battleMusic = NULL;
Mix_Music *winnerMusic = NULL;
Mix_Music *endMusic = NULL;

const int POKEMONS_COUNT = 3;
const int TEXT_COUNT = 18;
const int ICON_COUNT = 10;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;
Pokemon pokemons[POKEMONS_COUNT];

LTexture texts[TEXT_COUNT];
LTexture icons[ICON_COUNT];

Player PLAYER1;
Player PLAYER2;

enum switchBetweenScreens
{
    START_GAME = 1,
    CHOOSE_PLAYER1,
    CHOOSE_PLAYER2,
    THE_BATTLE,
    WINNER,
    END_GAME
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
    pokemon.manaPointsbar.loadFromFile("/home/stas/SDL/Pokemon/assets/healthbar.png", gRenderer);
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

    if (!texts[P1_ATTACKS].loadFromRenderedText("Player 1 Attacks", SDL_Color{64, 96, 255, SDL_ALPHA_OPAQUE}, gFontGame, gRenderer))
    {
        success = false;
    }
    if (!texts[PLAYER_ONE_WIN].loadFromRenderedText("Player 1 WIN", SDL_Color{64, 96, 255, SDL_ALPHA_OPAQUE}, gFontGame, gRenderer))
    {
        success = false;
    }

    if (!texts[PLAYER_TWO_WIN].loadFromRenderedText("Player 2 WIN", SDL_Color{255, 21, 21, SDL_ALPHA_OPAQUE}, gFontGame, gRenderer))
    {
        success = false;
    }
    if (!texts[P2_ATTACKS].loadFromRenderedText("Player 2 Attacks", SDL_Color{255, 21, 21, SDL_ALPHA_OPAQUE}, gFontGame, gRenderer))
    {
        success = false;
    }
    if (!texts[HP].loadFromRenderedText("HP:", SDL_Color{201, 15, 15, SDL_ALPHA_OPAQUE}, gFontGame, gRenderer))
    {
        success = false;
    }
    if (!texts[MANA].loadFromRenderedText("MANA:", SDL_Color{64, 96, 255, SDL_ALPHA_OPAQUE}, gFontGame, gRenderer))
    {
        success = false;
    }
    if (!texts[POKEMON_FIGHT].loadFromRenderedText("Pokemon Fight", SDL_Color{0, 0, 0, SDL_ALPHA_OPAQUE}, gFontEnd1, gRenderer))
    {
        success = false;
    }
    if (!texts[GAME_WAS_MADE].loadFromRenderedText("The game was made by", SDL_Color{0, 0, 0, SDL_ALPHA_OPAQUE}, gFontEnd1, gRenderer))
    {
        success = false;
    }
    if (!texts[AUTHOR_NICKNAME].loadFromRenderedText("J3N1X", SDL_Color{0, 0, 0, SDL_ALPHA_OPAQUE}, gFontEnd2, gRenderer))
    {
        success = false;
    }
    if (!texts[ENTER_TO_CONTINUE].loadFromRenderedText("Press c to continue...", SDL_Color{0, 0, 0, SDL_ALPHA_OPAQUE}, gFontGame, gRenderer))
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
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0)
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

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
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

    if (gFontEnd1 != NULL)
        TTF_CloseFont(gFontEnd1);
    gFontEnd1 = NULL;

    if (gFontEnd2 != NULL)
        TTF_CloseFont(gFontEnd2);
    gFontEnd2 = NULL;

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    Mix_FreeChunk(enter);
    enter = NULL;

    Mix_FreeChunk(pressEnterMenu);
    pressEnterMenu = NULL;

    Mix_FreeChunk(pressArrow);
    pressArrow = NULL;

    Mix_FreeChunk(hit);
    hit = NULL;

    Mix_FreeMusic(battleMusic);
    battleMusic = NULL;

    Mix_FreeMusic(winnerMusic);
    winnerMusic = NULL;

    Mix_FreeMusic(endMusic);
    endMusic = NULL;

    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
}

void startGame(LTexture &newText)
{
    newText.render(SCREEN_WIDTH / 2 - newText.getWidth() / 2, SCREEN_HEIGHT / 2 - newText.getHeight() / 2, gRenderer);
    newText.flashing();
}

void startGameEvent(SDL_Event &e, int &currentState)
{
    if (e.type == SDL_KEYDOWN)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_RETURN:
            currentState++;
            Mix_PlayChannel(-1, enter, 0);
            break;
        }
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
                Mix_PlayChannel(-1, pressArrow, 0);
            }
            curEventState--;
            break;
        case SDLK_DOWN:
            if (curEventState != 2)
            {
                pokemons[curEventState].pokemonTexture.setNewAlpha(255);
                pokemons[curEventState].pokemonName.setNewAlpha(255);
                Mix_PlayChannel(-1, pressArrow, 0);
            }
            curEventState++;
            break;
        case SDLK_RETURN:
            for (int i = 0; i < POKEMONS_COUNT; i++)
            {
                pokemons[i].pokemonTexture.setNewAlpha(255);
                pokemons[i].pokemonName.setNewAlpha(255);
                Mix_PlayChannel(-1, pressEnterMenu, 0);
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

    gFontEnd1 = TTF_OpenFont("/home/stas/SDL/Pokemon/assets/SEASRN.ttf", 64);
    if (gFontEnd1 == NULL)
    {
        printf("TTF failed to create font. TTF_Error: %s\n", TTF_GetError());
        success = false;
    }

    gFontEnd2 = TTF_OpenFont("/home/stas/SDL/Pokemon/assets/FFF_Tusj.ttf", 64);
    if (gFontEnd2 == NULL)
    {
        printf("TTF failed to create font. TTF_Error: %s\n", TTF_GetError());
        success = false;
    }

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
        else if (i == MANA_BOTTLE)
        {
            if (!icons[i].loadFromFile("/home/stas/SDL/Pokemon/assets/manaBottle.png", gRenderer))
                flag = true;
        }
        else if (i == CROWN)
        {
            if (!icons[i].loadFromFile("/home/stas/SDL/Pokemon/assets/crown.png", gRenderer))
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

    // LOAD SOUND

    enter = Mix_LoadWAV("/home/stas/SDL/Pokemon/assets/enterButton.mp3");
    if (enter == NULL)
    {
        printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    pressArrow = Mix_LoadWAV("/home/stas/SDL/Pokemon/assets/pressArrow.mp3");
    if (pressArrow == NULL)
    {
        printf("Failed to load sound effect(pressArrow)! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    pressEnterMenu = Mix_LoadWAV("/home/stas/SDL/Pokemon/assets/pressEnterMenu.mp3");
    if (pressEnterMenu == NULL)
    {
        printf("Failed to load sound effect(pressEnterMenu)! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    hit = Mix_LoadWAV("/home/stas/SDL/Pokemon/assets/hit.wav");
    if (hit == NULL)
    {
        printf("Failed to load sound effect(hit)! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    battleMusic = Mix_LoadMUS("/home/stas/SDL/Pokemon/assets/battleMusic.mp3");
    if (battleMusic == NULL)
    {
        printf("Failed to load music(battleMusic)! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    winnerMusic = Mix_LoadMUS("/home/stas/SDL/Pokemon/assets/winnerMusic.mp3");
    if (winnerMusic == NULL)
    {
        printf("Failed to load music(winnerMusic)! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    endMusic = Mix_LoadMUS("/home/stas/SDL/Pokemon/assets/endMusic.wav");
    if (endMusic == NULL)
    {
        printf("Failed to load music(endMusic)! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
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

bool battleLogic(Player &player1, Player &player2, int curState, int &curGlobalState)
{
    int minusManaP1 = player1.pokemon.getManaPoints() - player1.pokemon.pokemonAttacks[curState].getWastedMana();

    if (minusManaP1 < 0)
    {
        return false;
    }
    player1.pokemon.setMana(minusManaP1);

    int minusHpP2 = player2.pokemon.getHp() - player1.pokemon.pokemonAttacks[curState].getAttackDamage();

    if (minusHpP2 <= 0)
    {
        curGlobalState++;
        minusHpP2 = 0;
        Mix_HaltMusic();
    }
    player2.pokemon.setHp(minusHpP2);

    player2.pokemon.setMana(std::min(player2.pokemon.getManaPoints() + 10, player2.pokemon.getFullMana()));

    return true;
}

int isManaMoreZero(Player &player1, Player &player2, int curState)
{
    return (player1.pokemon.getManaPoints() - player1.pokemon.pokemonAttacks[curState].getWastedMana()) >= 0;
}

void setNewAlphaInBattle(Player &player1, Player &player2)
{
    for (int i = 0; i <= SKIP_ATTACK; i++)
    {
        if (!isManaMoreZero(player1, player2, i))
        {
            player1.pokemon.pokemonAttacks[i].attackNameText.setNewAlpha(111);
        }
    }
}

void recoverTextureAlpha(Player &player1, Player &player2)
{
    for (int i = 0; i <= SKIP_ATTACK; i++)
    {
        player1.pokemon.pokemonAttacks[i].attackNameText.setNewAlpha(255);
        player2.pokemon.pokemonAttacks[i].attackNameText.setNewAlpha(255);
    }
}

void battle(Player &player1, Player &player2, Player &currentPlayer, int &curState, int &curGlobalState, int curPlayer, int anotherPlayer)
{

    SDL_Rect tempRectangle1 = {player1.pokemon.pokemonTexture.getWidth() * 3 / 2 - 20,
                               player1.pokemon.pokemonTexture.getHeight() + player1.pokemon.healthbar.getHeight() + 10,
                               std::max((int)(1.0 * player1.pokemon.healthbar.getWidth() / player1.pokemon.getFullHp() * player1.pokemon.getHp()), 0), player1.pokemon.healthbar.getHeight()};

    SDL_Rect tempRectangle2 = {player2.pokemon.pokemonTexture.getWidth() + player2.pokemon.healthbar.getWidth() / 2 + 360,
                               player2.pokemon.pokemonTexture.getHeight() + player2.pokemon.healthbar.getHeight() + 10,
                               std::max((int)(1.0 * player2.pokemon.healthbar.getWidth() / player2.pokemon.getFullHp() * player2.pokemon.getHp()), 0), player2.pokemon.healthbar.getHeight()};

    SDL_Rect tempRectangle3 = {player1.pokemon.pokemonTexture.getWidth() * 3 / 2 - 20,
                               player1.pokemon.pokemonTexture.getHeight() + 3 * player1.pokemon.manaPointsbar.getHeight(),
                               std::max((int)(1.0 * player1.pokemon.manaPointsbar.getWidth() / player1.pokemon.getFullMana() * player1.pokemon.getManaPoints()), 0), player1.pokemon.manaPointsbar.getHeight()};

    SDL_Rect tempRectangle4 = {player2.pokemon.pokemonTexture.getWidth() + player2.pokemon.healthbar.getWidth() / 2 + 360,
                               player2.pokemon.pokemonTexture.getHeight() + 3 * player2.pokemon.healthbar.getHeight(),
                               std::max((int)(1.0 * player2.pokemon.manaPointsbar.getWidth() / player2.pokemon.getFullMana() * player2.pokemon.getManaPoints()), 0), player2.pokemon.manaPointsbar.getHeight()};

    player1.pokemon.pokemonTextureReversed.render(SCREEN_WIDTH / 2 - player1.pokemon.pokemonTexture.getWidth() - 50,
                                                  SCREEN_HEIGHT / 4 - player1.pokemon.pokemonTexture.getHeight() + 100, gRenderer);
    player2.pokemon.pokemonTexture.render(SCREEN_WIDTH / 2 + 50,
                                          SCREEN_HEIGHT / 4 - player2.pokemon.pokemonTexture.getHeight() + 100, gRenderer);

    player1.pokemon.healthbar.render(player1.pokemon.pokemonTexture.getWidth() * 3 / 2 - 20,
                                     player1.pokemon.pokemonTexture.getHeight() + player1.pokemon.healthbar.getHeight() + 10, gRenderer);

    player1.pokemon.manaPointsbar.render(player1.pokemon.pokemonTexture.getWidth() * 3 / 2 - 20,
                                         player1.pokemon.pokemonTexture.getHeight() + 3 * player1.pokemon.manaPointsbar.getHeight(), gRenderer);

    icons[HEART].render(player1.pokemon.pokemonTexture.getWidth() * 3 / 2 - 20 - icons[HEART].getWidth(),
                        player1.pokemon.pokemonTexture.getHeight() + player1.pokemon.healthbar.getHeight() - icons[HEART].getHeight() / 4, gRenderer);

    icons[MANA_BOTTLE].render(player1.pokemon.pokemonTexture.getWidth() * 3 / 2 - 29 - icons[MANA_BOTTLE].getWidth(),
                              player1.pokemon.pokemonTexture.getHeight() + 3 * player1.pokemon.manaPointsbar.getHeight() - icons[MANA_BOTTLE].getWidth() / 4, gRenderer);

    player2.pokemon.healthbar.render(player2.pokemon.pokemonTexture.getWidth() + player2.pokemon.healthbar.getWidth() / 2 + 360,
                                     player2.pokemon.pokemonTexture.getHeight() + player2.pokemon.healthbar.getHeight() + 10, gRenderer);

    player2.pokemon.manaPointsbar.render(player2.pokemon.pokemonTexture.getWidth() + player2.pokemon.healthbar.getWidth() / 2 + 360,
                                         player2.pokemon.pokemonTexture.getHeight() + 3 * player2.pokemon.healthbar.getHeight(), gRenderer);

    icons[HEART].render(player2.pokemon.pokemonTexture.getWidth() + player2.pokemon.healthbar.getWidth() / 2 + 360 - icons[HEART].getWidth(),
                        player2.pokemon.pokemonTexture.getHeight() + player2.pokemon.healthbar.getHeight() - icons[HEART].getHeight() / 4, gRenderer);

    icons[MANA_BOTTLE].render(player2.pokemon.pokemonTexture.getWidth() + player2.pokemon.healthbar.getWidth() / 2 + 360 - 4 * icons[MANA_BOTTLE].getWidth() / 3 + 1,
                              player2.pokemon.pokemonTexture.getHeight() + player2.pokemon.healthbar.getHeight() + icons[MANA_BOTTLE].getHeight() + 5, gRenderer);

    texts[P1_GAME].render(10, 0, gRenderer);
    texts[P2_GAME].render(SCREEN_WIDTH - texts[P2_GAME].getWidth() - 10, 0, gRenderer);

    texts[HP].render(10, SCREEN_HEIGHT - texts[HP].getHeight(), gRenderer);
    player1.pokemon.hpText.render(10 + texts[HP].getWidth(), SCREEN_HEIGHT - texts[HP].getWidth(), gRenderer);
    texts[MANA].render(10 + texts[HP].getWidth() + player1.pokemon.hpText.getWidth() * 2, SCREEN_HEIGHT - texts[MANA].getHeight(), gRenderer);
    player1.pokemon.manaPointsText.render(10 + texts[HP].getWidth() + player1.pokemon.hpText.getWidth() * 2 + texts[MANA].getWidth(),
                                          SCREEN_HEIGHT - texts[MANA].getHeight(), gRenderer);

    texts[MANA].render(SCREEN_WIDTH - texts[MANA].getWidth() * 3 / 2, SCREEN_HEIGHT - texts[MANA].getHeight(), gRenderer);
    player2.pokemon.manaPointsText.render(SCREEN_WIDTH - texts[MANA].getWidth() / 2, SCREEN_HEIGHT - texts[MANA].getHeight(), gRenderer);
    texts[HP].render(SCREEN_WIDTH - texts[MANA].getWidth() * 3 / 2 - 4 * texts[HP].getWidth(), SCREEN_HEIGHT - texts[HP].getHeight(), gRenderer);
    player2.pokemon.hpText.render(SCREEN_WIDTH - texts[MANA].getWidth() * 3 / 2 - 3 * texts[HP].getWidth(), SCREEN_HEIGHT - texts[HP].getHeight(), gRenderer);

    icons[ARROW].render(0, SCREEN_HEIGHT / 2 + curState * 50 + 10, gRenderer);

    outputIconsInBattle(curState, currentPlayer);

    if (curPlayer == 1)
    {
        texts[P1_ATTACKS].render(SCREEN_WIDTH * 9 / 24, 0, gRenderer);
    }
    else if (curPlayer == 2)
    {
        texts[P2_ATTACKS].render(SCREEN_WIDTH * 9 / 24, 0, gRenderer);
    }

    if (curPlayer == 1)
    {
        setNewAlphaInBattle(player1, player2);
    }
    else
    {
        setNewAlphaInBattle(player2, player1);
    }
    currentPlayer.pokemon.pokemonAttacks[curState].attackDescriptionText.render(10, SCREEN_HEIGHT / 2 + 220, gRenderer);

    currentPlayer.pokemon.pokemonAttacks[FIRST_ATTACK].attackNameText.render(50, SCREEN_HEIGHT / 2, gRenderer);
    currentPlayer.pokemon.pokemonAttacks[SECOND_ATTACK].attackNameText.render(50, SCREEN_HEIGHT / 2 + 50, gRenderer);
    currentPlayer.pokemon.pokemonAttacks[THIRD_ATTACK].attackNameText.render(50, SCREEN_HEIGHT / 2 + 100, gRenderer);
    currentPlayer.pokemon.pokemonAttacks[SKIP_ATTACK].attackNameText.render(50, SCREEN_HEIGHT / 2 + 150, gRenderer);

    SDL_SetRenderDrawColor(gRenderer, 201, 15, 15, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(gRenderer, &tempRectangle1);
    SDL_RenderFillRect(gRenderer, &tempRectangle2);
    SDL_SetRenderDrawColor(gRenderer, 0, 66, 225, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(gRenderer, &tempRectangle3);
    SDL_RenderFillRect(gRenderer, &tempRectangle4);
    SDL_RenderPresent(gRenderer);

    if (curGlobalState != THE_BATTLE)
        SDL_Delay(3000);
}

bool battleEvent(SDL_Event &e, int &currentPlayer, Player &curPlayer, Player &anotherPlayer, int &currentState, int &curGlobalState)
{
    if (e.type == SDL_KEYDOWN)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_DOWN:
            if (currentState != SKIP_ATTACK)
            {
                currentState++;
                Mix_PlayChannel(-1, pressArrow, 0);
            }
            break;
        case SDLK_UP:
            if (currentState != FIRST_ATTACK)
            {
                currentState--;
                Mix_PlayChannel(-1, pressArrow, 0);
            }
            break;
        case SDLK_RETURN:
            bool flag = battleLogic(curPlayer, anotherPlayer, currentState, curGlobalState);
            if (flag)
            {
                if (currentState != SKIP_ATTACK)
                {
                    Mix_PlayChannel(-1, hit, 0);
                }
                else
                {
                    Mix_PlayChannel(-1, pressEnterMenu, 0);
                }
            }
            return flag;
            break;
        }

        if (currentState > SKIP_ATTACK)
            currentState = SKIP_ATTACK;
        if (currentState < FIRST_ATTACK)
            currentState = FIRST_ATTACK;
    }

    return false;
}

void endGame(double &angle)
{
    SDL_Point point = {texts[AUTHOR_NICKNAME].getWidth() / 2, texts[AUTHOR_NICKNAME].getHeight() / 2};
    texts[POKEMON_FIGHT].render(SCREEN_WIDTH / 2 - texts[POKEMON_FIGHT].getWidth() / 2, SCREEN_HEIGHT / 40, gRenderer);
    texts[GAME_WAS_MADE].render(SCREEN_WIDTH / 2 - texts[GAME_WAS_MADE].getWidth() / 2, SCREEN_HEIGHT / 3, gRenderer);
    texts[AUTHOR_NICKNAME].render(SCREEN_WIDTH / 2 - texts[AUTHOR_NICKNAME].getWidth() / 2, SCREEN_HEIGHT * 5 / 8,
                                  gRenderer, NULL, angle, &point, SDL_FLIP_NONE);
    texts[AUTHOR_NICKNAME].flashing();
    angle++;
    if (angle > 360)
        angle = 0;
}

void winner(Player &player, LTexture &win)
{
    SDL_Point point1 = {icons[CROWN].getWidth() / 2, icons[CROWN].getHeight() / 2};

    win.render(SCREEN_WIDTH / 2 - win.getWidth() / 2, SCREEN_HEIGHT / 40, gRenderer);
    player.pokemon.pokemonTexture.render((SCREEN_WIDTH - player.pokemon.pokemonTexture.getWidth()) / 2,
                                         (SCREEN_HEIGHT - player.pokemon.pokemonTexture.getHeight()) / 2, gRenderer);

    icons[CROWN].render((SCREEN_WIDTH - player.pokemon.pokemonTexture.getWidth()) / 2,
                        (SCREEN_HEIGHT - player.pokemon.pokemonTexture.getHeight()) / 2 - icons[CROWN].getHeight() / 2, gRenderer);

    icons[CROWN].render((SCREEN_WIDTH - player.pokemon.pokemonTexture.getWidth()) / 2,
                        (SCREEN_HEIGHT - player.pokemon.pokemonTexture.getHeight()) / 2 + icons[CROWN].getHeight(), gRenderer,
                        NULL, 180, &point1, SDL_FLIP_HORIZONTAL);

    icons[CROWN].render((SCREEN_WIDTH - player.pokemon.pokemonTexture.getWidth()) / 2 - icons[CROWN].getWidth() * 3 / 4,
                        (SCREEN_HEIGHT - player.pokemon.pokemonTexture.getHeight()) / 2 + icons[CROWN].getHeight() / 4, gRenderer, NULL, -90, &point1, SDL_FLIP_HORIZONTAL);

    icons[CROWN].render((SCREEN_WIDTH - player.pokemon.pokemonTexture.getWidth()) / 2 + icons[CROWN].getWidth() * 3 / 4,
                        (SCREEN_HEIGHT - player.pokemon.pokemonTexture.getHeight()) / 2 + icons[CROWN].getHeight() / 4, gRenderer, NULL, 90, &point1, SDL_FLIP_HORIZONTAL);

    texts[ENTER_TO_CONTINUE].render(SCREEN_WIDTH / 2 - texts[ENTER_TO_CONTINUE].getWidth() / 2, SCREEN_HEIGHT - texts[ENTER_TO_CONTINUE].getHeight() * 7 / 8, gRenderer);
}

void winnerEvent(SDL_Event &e, int &curGlobalState)
{
    if (e.type == SDL_KEYDOWN)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_c:
            curGlobalState++;
            Mix_HaltMusic();
            break;
        }
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

    double angle = 0.0;

    int tempCurrentState = currentState;

    while (!quit)
    {
        bool theBattle = false;

        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (currentState == START_GAME)
            {
                startGameEvent(e, tempCurrentState);
            }
            else if (currentState == CHOOSE_PLAYER1)
            {
                choosePlayerEvents(tempState1, e, tempCurrentState);
            }
            else if (currentState == CHOOSE_PLAYER2)
            {
                choosePlayerEvents(tempState2, e, tempCurrentState);
            }
            else if (currentState == THE_BATTLE)
            {

                if (currentPlayer == 1)
                {
                    theBattle = battleEvent(e, currentPlayer, PLAYER1, PLAYER2, tempStateBattle1, tempCurrentState);
                }
                else
                {
                    theBattle = battleEvent(e, currentPlayer, PLAYER2, PLAYER1, tempStateBattle2, tempCurrentState);
                }
            }
            else if (currentState == WINNER)
            {
                winnerEvent(e, tempCurrentState);
            }
        }

        switch (currentState)
        {
        case START_GAME:
            SDL_SetRenderDrawColor(gRenderer, 0, 0, 128, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(gRenderer);
            startGame(texts[ENTER_GAME]);
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
            if (PLAYER1.pokemon.pokemonTexture.isEmpty())
            {
                PLAYER1.pokemon = pokemons[tempState1];
            }
            if (PLAYER2.pokemon.pokemonTexture.isEmpty())
            {
                PLAYER2.pokemon = pokemons[tempState2];
            }

            if (Mix_PlayingMusic() == 0 && tempCurrentState == currentState)
            {
                Mix_PlayMusic(battleMusic, -1);
            }

            PLAYER1.pokemon.hpText.free();
            PLAYER1.pokemon.manaPointsText.free();
            PLAYER1.pokemon.hpText.loadFromRenderedText(std::to_string(PLAYER1.pokemon.getHp()), SDL_Color{201, 15, 15, SDL_ALPHA_OPAQUE}, gFontGame, gRenderer);
            PLAYER1.pokemon.manaPointsText.loadFromRenderedText(std::to_string(PLAYER1.pokemon.getManaPoints()), SDL_Color{64, 96, 255, SDL_ALPHA_OPAQUE}, gFontGame, gRenderer);

            PLAYER2.pokemon.hpText.free();
            PLAYER2.pokemon.manaPointsText.free();
            PLAYER2.pokemon.hpText.loadFromRenderedText(std::to_string(PLAYER2.pokemon.getHp()), SDL_Color{201, 15, 15, SDL_ALPHA_OPAQUE}, gFontGame, gRenderer);
            PLAYER2.pokemon.manaPointsText.loadFromRenderedText(std::to_string(PLAYER2.pokemon.getManaPoints()), SDL_Color{64, 96, 255, SDL_ALPHA_OPAQUE}, gFontGame, gRenderer);
            if (currentPlayer == 1)
            {
                battle(PLAYER1, PLAYER2, PLAYER1, tempStateBattle1, tempCurrentState, currentPlayer, 2);
                if (theBattle)
                {
                    tempStateBattle1 = 0;
                    currentPlayer = 2;
                }
            }
            else
            {
                battle(PLAYER1, PLAYER2, PLAYER2, tempStateBattle2, tempCurrentState, currentPlayer, 1);
                if (theBattle)
                {
                    tempStateBattle2 = 0;
                    currentPlayer = 1;
                }
            }
            recoverTextureAlpha(PLAYER1, PLAYER2);
            break;

        case WINNER:
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);
            if (Mix_PlayingMusic() == 0 && tempCurrentState == currentState)
            {
                Mix_PlayMusic(winnerMusic, -1);
            }

            if (PLAYER1.pokemon.getHp() != 0)
            {
                winner(PLAYER1, texts[PLAYER_ONE_WIN]);
            }
            else
            {
                winner(PLAYER2, texts[PLAYER_TWO_WIN]);
            }
            break;
        case END_GAME:
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);
            if (Mix_PlayingMusic() == 0)
            {
                Mix_PlayMusic(endMusic, -1);
            }
            endGame(angle);
            break;
        }

        if (tempCurrentState != THE_BATTLE)
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

        currentState = tempCurrentState;
    }

    close();
    return 0;
}
