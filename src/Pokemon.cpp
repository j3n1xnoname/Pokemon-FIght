#include "Pokemon.hpp"
#include "iostream"

Pokemon::Pokemon(std::string NAME, int HP, int MANA, int ATTACK_DAMAGE, std::string ATTACK_NAME, int WASTE_MANA, SDL_Renderer *gRenderer)
{
    name = NAME;
    hp = HP;
    manaPoints = MANA;
    attack = Attack{ATTACK_DAMAGE, ATTACK_NAME, WASTE_MANA};
}

Pokemon::~Pokemon()
{
    name = "";
    hp = 0;
    manaPoints = 0;
    pokemonTexture.free();
    pokemonName.free();
    hpText.free();
    manaPointsText.free();
    attackDamageText.free();
    wasteManaText.free();
}

std::string Pokemon::getAttackName()
{
    return attack.getAttackName();
}