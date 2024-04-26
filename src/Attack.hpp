#pragma once
#include <string>

class Attack
{
public:
    int getAttackDamage() { return attackDamage; }
    std::string getAttackName() { return attackName; }
    Attack(int ATTACK_DAMAGE = 0, std::string ATTACK_NAME = "", int WASTE_MANA = 0);
    ~Attack();

private:
    int attackDamage;
    std::string attackName;
    int wasteMana;
};