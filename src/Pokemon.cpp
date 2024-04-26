#include "Pokemon.hpp"
#include "iostream"

Pokemon::Pokemon(std::string NAME, int HP, int MANA, int ATTACK_DAMAGE, std::string ATTACK_NAME, int WASTE_MANA, SDL_Renderer *gRenderer)
{
    name = NAME;
    hp = HP;
    manaPoints = MANA;
    if (name == "Pikachu")
    {
        pokemonTexture.loadFromFile("/home/stas/SDL/Pokemon/assets/pikachu.png", gRenderer);
    }
    else if (name == "Charizard")
    {
        pokemonTexture.loadFromFile("/home/stas/SDL/Pokemon/assets/charizard.png", gRenderer);
    }
    else if (name == "Bulbasaur")
    {
        pokemonTexture.loadFromFile("/home/stas/SDL/Pokemon/assets/bulbasaur.png", gRenderer);
    }
    attack = Attack{ATTACK_DAMAGE, ATTACK_NAME, WASTE_MANA};
}

Pokemon::~Pokemon()
{
    name = "";
    hp = 0;
    manaPoints = 0;
    pokemonTexture.free();
}