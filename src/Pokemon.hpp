#pragma once
#include "LTexture.hpp"
#include <string>
#include "Attack.hpp"
#include <vector>

class Pokemon
{
public:
    Pokemon(std::string NAME = "", int HP = 0,
            int MANA = 0, int ATTACK_DAMAGE = 0, std::string ATTACK_NAME = "", int WASTE_MANA = 0, SDL_Renderer *gRenderer = NULL);
    ~Pokemon();
    int getHp() { return hp; }
    int getManaPoints() { return manaPoints; }
    std::string getAttackName();

    LTexture pokemonTexture;
    LTexture pokemonName;
    LTexture hpText;
    LTexture manaPointsText;
    LTexture attackDamageText;
    LTexture wasteManaText;

private:
    int hp, manaPoints;
    std::string name;
    Attack attack;
};