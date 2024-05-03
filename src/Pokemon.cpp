#include "Pokemon.hpp"
#include "iostream"

Pokemon::Pokemon(std::string NAME, int HP, int MANA)
{
    name = NAME;
    hp = HP;
    manaPoints = MANA;
    fullHp = HP;
    fullManaPoints = MANA;
}

Pokemon::~Pokemon()
{
    name = "";
    hp = 0;
    manaPoints = 0;
    fullHp = 0;
    fullManaPoints = 0;
    pokemonTexture.free();
    pokemonTextureReversed.free();
    pokemonName.free();
    hpText.free();
    manaPointsText.free();
    attackDamageText.free();
    wasteManaText.free();
    manaPointsbar.free();
    healthbar.free();
}

std::string Pokemon::getPokemonName()
{
    return name;
}