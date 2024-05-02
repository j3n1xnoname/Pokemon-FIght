#pragma once
#include <string>
#include "LTexture.hpp"

class Attack
{
public:
    int getAttackDamage() { return attackDamage; }
    std::string getAttackName() { return attackName; }
    std::string getAttackDescription() { return attackDescription; }
    int getWastedMana() { return wasteMana; }
    Attack(std::string ATTACK_NAME, std::string ATTACK_DESCRIPION, int ATTACK_DAMAGE = 0, int WASTE_MANA = 0);
    ~Attack();
    Attack();

    LTexture attackDamageText;
    LTexture attackNameText;
    LTexture wasteManaText;
    LTexture attackDescriptionText;

private:
    int attackDamage;
    std::string attackName;
    std::string attackDescription;
    int wasteMana;
};