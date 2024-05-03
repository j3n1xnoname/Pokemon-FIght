#pragma once
#include "LTexture.hpp"
#include <string>
#include "Attack.hpp"
#include <vector>

class Pokemon
{
public:
    Pokemon(std::string NAME = "", int HP = 0, int MANA = 0);
    ~Pokemon();
    int getHp() { return hp; }
    int getFullHp() { return fullHp; }
    int getFullMana() { return fullManaPoints; }
    void setHp(int HP) { hp = HP; }
    int getManaPoints() { return manaPoints; }
    void setMana(int MANA) { manaPoints = MANA; }

    std::string getPokemonName();
    std::string getAttackName();

    LTexture pokemonTexture;
    LTexture pokemonTextureReversed;

    LTexture healthbar;
    LTexture manaPointsbar;

    LTexture pokemonName;
    LTexture hpText;

    LTexture manaPointsText;
    LTexture attackDamageText;
    LTexture wasteManaText;

    Attack pokemonAttacks[4];

private:
    int hp, manaPoints;
    int fullHp;
    int fullManaPoints;
    std::string name;
};